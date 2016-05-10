#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import time

from util import util
from util.util import tprint
from classes.node import Node

__author__ = "Marc-Andre Vef"
__version__ = "0.5"
__email__ = "vef@uni-mainz.de"


class CreateScaleProto(object):
    """
    CreateScalaProto object is a bootstrap script that initializes a complete IBM Spectrum Scale cluster.

    ######################################################
    TODO NOT YET TESTED DUE TO THE LACK OF A TEST SYSTEM
    might require sleeps between steps due to asynchronous distribution of configurations through the cluster
    ######################################################

    Args:
        conf_path (str): Path to the configuration file used for the cluster.

    Attributes:
        _nodes (List[Node]): List of all nodes and their devices within the cluster.
        _stanza_path (str): Contains the path to the stanza file for creating the file system
    """

    def __init__(self, conf_path, pretend):
        self._nodes = []
        self._stanza_path = None
        self._fsname = 'scaletmpfs'
        self._pretend = pretend
        self.parse_input(conf_path)
        self.verify_input()

    def init_system(self, **only):
        """Main function triggering all steps necessary for Scale initiation

        Args:
            only (Dict[str:bool]): Dictionary of all parts that should be executed during the init of Scale.
        """
        exec_all = True
        if len(only) > 0:
            exec_all = False
            # set dependencies
            if only.get('crnsd_only'):
                only['build_stanza_only'] = True
            if only.get('crfs_only'):
                only['build_stanza_only'] = True
                only['crnsd_only'] = True
            if only.get('mountfs_only'):
                only['startup_only'] = True
        # execute functions that are enabled
        if exec_all or only.get('crcluster_only'):
            self.cr_cluster()
            self.verify_nodes() # TODO
        if exec_all or only.get('build_stanza_only'):
            self.build_stanza()
        if exec_all or only.get('crnsd_only'):
            self.create_nsds()
            self.verify_nsds() # TODO
        if exec_all or only.get('startup_only'):
            self.startup_scale()
        if exec_all or only.get('crfs_only'):
            self.create_fs()
        if exec_all or only.get('mountfs_only'):
            self.mount_fs()

    def parse_input(self, conf_path):
        """Function parses user input (nodes, mgrnodes, storage) and puts them into appropriate data structures

        Args:
            conf_path (str): The path to the configuration file for the cluster and their devices.

        """
        tprint('Parsing input from configuration file...')
        with open(conf_path, 'r') as f:
            node = None
            skip = False
            for l_no, line in enumerate(f.readlines()):
                if line.strip() == '%node':
                    if node is not None and skip is False:
                        self._nodes.append(node)
                    node = Node()
                    skip = False
                elif line.startswith('#') or line.startswith('\n') or skip is True:
                    continue
                elif line.startswith('node'):
                    n = line.strip().split('=')[1].split(':')
                    mgr = True if len(n) > 1 and n[1] == 'mgr' else False
                    node.nodename = n[0]
                    node.manager = mgr
                elif line.startswith('devices'):
                    devices = line.strip().split('=')[1].split(',')
                    node.devices = [] if devices == 'None' or devices == ['None'] else devices
                elif line.startswith('fsname'):
                    self._fsname = line.strip().split('=')[1]
                else:
                    print 'WARN: Linenumber %d is skipped. Containing invalid property: \n%s.' \
                        % (l_no, line)
                    skip = True
        if node is not None and skip is False:
            self._nodes.append(node)
        tprint('[DONE]\n')

    def verify_input(self):
        """Function verifies given input for correctness.

        Raises exception if invalid.
        """
        tprint('Verifying input...')
        mgr_count = 0
        for node in self._nodes:
            if node.nodename == '':
                tprint('WARN: Node with devices:\n %s \n has no node name and will be ignored'
                       % str(node.devices)[1:-1])
            if len(node.devices) == 0 or (len(node.devices) > 0 and node.devices[0] == ''):
                tprint('WARN: Node %s has no devices configured but will be part of the cluster.' % node.nodename)
            if node.manager is True:
                mgr_count += 1
        if mgr_count == 0:
            raise Exception('ERR: No manager nodes are configured! Exiting...')
        tprint('[DONE]\n')

    def cr_cluster(self):
        """Function adds the nodes to Scale cluster including mgr nodes"""
        tprint('Creating Scale cluster...')
        mmcrcluster = 'mmcrcluster -N '
        # Add nodes to command
        for node in self._nodes:
            mmcrcluster += '%s:quorum' % node.nodename
            mmcrcluster += '-manager,' if node.manager else '-client,'
        # Add primary node. Take the first node in the list self.nodes. also remove last comma
        mmcrcluster = '%s -p %s -C' % (mmcrcluster[0:-1], self._nodes[0].nodename)

        # add ssh and scp binaries for execution
        ssh_path = util.check_shell_out(util.exec_shell('which ssh', suppress_output=True))
        if not os.path.exists(ssh_path):
            raise Exception('ERR: ssh not found in path %s. Exiting...' % ssh_path)
        scp_path = util.check_shell_out(util.exec_shell('which scp', suppress_output=True))
        if not os.path.exists(scp_path):
            raise Exception('ERR: scp not found in path %s. Exiting...' % scp_path)
        mmcrcluster = '%s -r %s -R %s' % (mmcrcluster, ssh_path, scp_path)
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % mmcrcluster)
        else:
            util.check_shell_out(util.exec_shell(mmcrcluster))
        tprint('Cluster created.\n')

    def verify_nodes(self):
        """Function verifies that nodes are added to the Scale cluster as specified

        Returns:
            bool: True if successful, False otherwise."""
        pass

    def build_stanza(self):
        """Function builds stanzas from the user input, required for generating NSDs later
        Stanzafile:
        %nsd:
        device=/dev/****
        nsd=NSDNAME
        servers=server01,server02
        usage=dataAndMetadata
        failureGroup=101
        pool=system
        """
        tprint('Creating stanza files for all nodes and devices...')
        self._stanza_path = '/tmp/scaleinit_stanza.%s' % time.strftime('%d%m%H.%M.%S')
        with open(self._stanza_path, 'w') as f:
            for node in self._nodes:
                if len(node.devices) != 0:
                    f.write(node.gen_scale_nsds())
        tprint('Stanza file created at %s\n' % self._stanza_path)

    def create_nsds(self):
        """Function creates NSDs from the earlier generated stanze build"""
        tprint('Creating Scale NSDs from stanza file...')
        mmcrnsd = 'mmcrnsd -F %s' % self._stanza_path
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % mmcrnsd)
        else:
            util.check_shell_out(util.exec_shell(mmcrnsd))
        tprint('NSDs created.\n')

    def verify_nsds(self):
        """Function verifies that all nsds were successfully created"""
        pass

    def startup_scale(self):
        """Function starts daemons of all nodes belonging to the cluster"""
        tprint('Start up Spectrum Scale...')
        mmstartup = 'mmstartup -a'
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % mmstartup)
        else:
            util.check_shell_out(util.exec_shell(mmstartup))
        tprint('Startup complete.\n')

    def create_fs(self):
        """Function creates file system with specified parameters on the specified nsd"""
        tprint('Creating file system %s with default parameters...' % self._fsname)
        # TODO version must be removed if cluster is configured properly...
        mmcrfs = 'mmcrfs /dev/%s -F %s --version=4.1.1.0' % (self._fsname, self._stanza_path)
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % mmcrfs)
        else:
            util.check_shell_out(util.exec_shell(mmcrfs))
        tprint('Creating complete.\n')
        pass

    def mount_fs(self):
        """Function mounts the file system on the specified mountpoint"""
        tprint('Mounting file system %s at /gpfs/%s...' % (self._fsname, self._fsname))
        mmmount = 'mmmount %s all' % self._fsname
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % mmmount)
        else:
            util.check_shell_out(util.exec_shell(mmmount))
        # Just give it some time to propagate the command through the cluster
        # time.sleep(5)
        tprint('Mounting complete.')


if __name__ == "__main__":
    usage = """
        MANUAL

        This script initiates a Spectrum Scale cluster, including NSDs, and file system. All based on a configuration
        file and executed in order automatically. The user may also only specify parts of the process to be executed.

        USAGE   :   python create_scale.py [option]
        Option  :
            -conf           *Path to the config file of the temporary cluster.
            -pretend        No execution of Scale commands. However, the stanza file will be created.
            -cluster        Creates _only_ the Scale cluster for the given configuration.
            -stanza         Build _only_ the stanza file for the given configuration.
            -nsd            Creates _only_ the nsd file for the given configuration.
                            Note: The stanza file will be generated as well.
            -startup        Startup Scale _only_.
            -fs             Creates _only_ the file system for the given configuration.
                            Note: Stanza file and nsd creation will be done as well.
            -mountfs        Mounts _only_ the file system for the given file system name in the configuration file.
                            Note: Scale will be started.

            * are always mandatory
            Example: python create_scale.py -conf ~/path/to/conf_file -cluster
        """

    # parsing input
    conf, run_all, crcluster_only, build_stanza_only, crnsd_only, startup_only, crfs_only, mountfs_only, pretend = \
        '', True, False, False, False, False, False, False, False

    while len(sys.argv) > 1:
        option = sys.argv[1]
        del sys.argv[1]
        if option == '-conf':
            conf = sys.argv[1]
            del sys.argv[1]
        elif option == '-cluster':
            crcluster_only = True
            run_all = False
        elif option == '-stanza':
            build_stanza_only = True
            run_all = False
        elif option == '-nsd':
            crnsd_only = True
            build_stanza_only = True
            run_all = False
        elif option == '-startup':
            startup_only = True
            run_all = False
        elif option == '-fs':
            crfs_only = True
            build_stanza_only = True
            crnsd_only = True
            run_all = False
        elif option == '-mountfs':
            mountfs_only = True
            run_all = False
        elif option == '-pretend':
            pretend = True
        else:
            raise Exception('Unexpected Argument "%s". Exiting...' % option)
    if conf == '':
        raise Exception('No path to configuration file given. It is mandatory. Exiting...')

    scale = CreateScaleProto(conf, pretend)
    if run_all:
        scale.init_system()
    else:
        scale.init_system(crcluster_only=crcluster_only, build_stanza_only=build_stanza_only, crnsd_only=crnsd_only,
                          startup_only=startup_only, crfs_only=crfs_only, mountfs_only=mountfs_only)

    print '\nNothing left to do; exiting. :)'
