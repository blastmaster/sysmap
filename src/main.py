#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Script for initializing a Scale filesystem."""

import sys
import os
import time

from util import util
from util.util import tprint
from classes.node import Node

__author__ = "Marc-Andre Vef"
__version__ = "0.1"
__email__ = "vef@uni-mainz.de"

pretend = True

class ScaleProto(object):
    """
    ScalaProto object is a bootstrap script that initializes a complete IBM Spectrum Scale cluster.

    Args:
        conf_path (str): Path to the configuration file used for the cluster.

    Attributes:
        _nodes (List[Node]): List of all nodes and their devices within the cluster.
        _stanza_path (str): Contains the path to the stanza file for creating the file system
    """

    def __init__(self, conf_path):
        self._nodes = []
        self._stanza_path = None
        self.parse_input(conf_path)
        self.verify_input()

    def init_system(self):
        """Main function triggering all steps necessary for Scale initiation"""
        self.cr_cluster()
        self.verify_nodes() # TODO
        self.build_stanza()
        self.create_nsds()
        self.verify_nsds() # TODO
        self.startup_scale() # TODO
        self.create_fs() # TODO
        self.mount_fs() # TODO

    def parse_input(self, conf_path):
        """Function parses user input (nodes, mgrnodes, storage) and puts them into appropriate data structures

        Cluster: nodenames, mgrnodes (nsdnodes), quorumnodes 0<1<7
        Stanza: nodes with their connected devices in /dev

        """
        tprint('Parsing input from configuration file...', nobreak=True)
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
                    node.devices = line.strip().split('=')[1].split(',')
                else:
                    print 'WARN: Nodeblock given in linenumber %d is skipped. Containing invalid property: \n%s.' \
                        % (l_no, line)
                    skip = True
        if node is not None and skip is False:
            self._nodes.append(node)
        print '[DONE]'

    def verify_input(self):
        """Function verifies given input for correctness.

        Raises exception if invalid.
        """
        tprint('Verifying input...', nobreak=True)
        mgr_count = 0
        for node in self._nodes:
            if node.nodename == '':
                print 'WARN: Node with devices:\n %s \n has no node name and will be ignored' \
                      % str(node.devices)[1:-1]
            if len(node.devices) == 0 or (len(node.devices) > 0 and node.devices[0] == ''):
                print 'WARN: Node %s has no devices configured and will be ignored.' % node.nodename
            if node.manager is True:
                mgr_count += 1
        if mgr_count == 0:
            raise Exception('ERR: No manager nodes are configured! Exiting...')
        print '[DONE]'

    def cr_cluster(self):
        """Function adds the nodes to Scale cluster including mgr nodes"""
        tprint('Creating Scale cluster...')
        mmcrcluster = 'mmcrcluster -N '
        # Add nodes to command
        for node in self._nodes:
            mmcrcluster += '%s:quorum' % node.nodename
            mmcrcluster += '-manager,' if node.manager else '-client,'
        # Add primary node. Take the first node in the list self.nodes. also remove last comma
        mmcrcluster = '%s -p %s' % (mmcrcluster[0:-1], self._nodes[0].nodename)

        # add ssh and scp binaries for execution
        ssh_path = util.exec_shell('which ssh', suppress_output=True)
        if not os.path.exists(ssh_path):
            raise Exception('ERR: ssh not found in path %s. Exiting...' % ssh_path)
        scp_path = util.exec_shell('which scp', suppress_output=True)
        if not os.path.exists(scp_path):
            raise Exception('ERR: scp not found in path %s. Exiting...' % scp_path)
        mmcrcluster = '%s -r %s -R %s' % (mmcrcluster, ssh_path, scp_path)
        # Execute command
        if pretend:
            tprint('Would execute if not pretending:\n\t%s' % mmcrcluster)
        else:
            util.exec_shell(mmcrcluster)
        tprint('Cluster created.')

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
                f.write(node.gen_nsds())
        tprint('Stanza file created at %s' % self._stanza_path)

    def create_nsds(self):
        """Function creates NSDs from the earlier generated stanze build"""
        tprint('Creating Scale NSDs from stanza file...')
        mmcrnsd = 'mmcrnsd -F %s' % self._stanza_path
        # Execute command
        if pretend:
            tprint('Would execute if not pretending:\n\t%s' % mmcrnsd)
        else:
            util.exec_shell(mmcrnsd)
        tprint('NSDs created.')

    def verify_nsds(self):
        """Function verifies that all nsds were successfully created"""
        pass

    def startup_scale(self):
        """Function starts daemons of all nodes belonging to the cluster"""
        pass

    def create_fs(self):
        """Function creates file system with specified parameters on the specified nsd"""
        pass

    def mount_fs(self):
        """Function mounts the file system on the specified mountpoint"""
        pass





if __name__ == "__main__":
    usage = """
        MANUAL
        This python script is written specifically for testing file create performance with the help of modified mdtest for periodic output.
        It creates a unique directory with results in the directory where the script is located. Tests will always run in a single directory
        USAGE   :   python python_mdtest.py [option]
        Option  :
            -host       hosts that are used with MPI. ex: host1,host2,host3 . This parameter will override -hostfile

            * are always mandatory
            Example: python python_periodic_mdtest.py -host bison04,bison05,bison07,bison08 -mdt_path /gpfs/createtest/mdtest/mdtest -d /gpfs/createtest -F -C -r -files 524288 -staticdir -disks 4HDDs
        """

    scale = ScaleProto("example.conf")
    scale.init_system()


    # if len(sys.argv) in [1, 2, 3]:
    #     print usage
    #     sys.exit(1)

    # parsing input
    # mpi_processes, hosts, hostfile, mdt_path, iterations, depth, branches, items, wd, fcreate_only, unique_dir, static, create_only, remove_only, mdtest_verbosity, restart_gpfs, warmup, disks = None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, False, False, ''
    # while len(sys.argv) > 1:
    #     option = sys.argv[1]
    #     del sys.argv[1]
    #     if option == '-host':
    #         hosts = sys.argv[1]
    #         del sys.argv[1]
    #     elif option == '-hostfile':
    #         hostfile = sys.argv[1]
    #         del sys.argv[1]
    #         if not os.path.exists(hostfile):
    #             print 'ERR: Path to host file is invalid. Please make sure to give the right path.'
    #             sys.exit(1)
    #     elif option == '-mdt_path':
    #         mdt_path = util.rm_trailing_slash(sys.argv[1])
    #         del sys.argv[1]
    #         if not os.path.exists(mdt_path):
    #             print 'ERR: Path to mdtest executable is invalid. Please make sure to give the right path.'
    #             sys.exit(1)
    # mdtest = MDTest(mdt_path, iterations, depth, branches, wd, items, mdtest_verbosity, create_only, remove_only,
    #                 fcreate_only, unique_dir, static, disks, warmup, restart_gpfs, hosts, hostfile)
    # mdtest.exec_periodictest()
    print '\nNothing left to do; exiting. :)'