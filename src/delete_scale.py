#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Script for deleting a Scale filesystem."""

import argparse

from util import util
from util.util import tprint
from classes.node import Node

__author__ = "Marc-Andre Vef"
__email__ = "vef@uni-mainz.de"


class DeleteScaleProto(object):
    """
    DeleteScalaProto object is a bootstrap script that deletes a complete IBM Spectrum Scale cluster, 
    including file system, NSD, and nodes from the cluster.
    
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

    def init_system(self, **steps):
        """Main function triggering all steps necessary for Scale deletion

        Args:
            steps (Dict[str:bool]): Dictionary of all parts that should be executed during the init of Scale.
        """
        exec_all = True

        if any(v for v in steps.itervalues()):
            exec_all = False
            # set dependencies
            if steps.get('delfs'):
                steps['umountfs'] = True
            if steps.get('delnsd'):
                steps['umountfs'] = True
                steps['delfs'] = True
        # execute functions that are enabled
        if exec_all or steps.get('umountfs'):
            self.umount_fs()
        if exec_all or steps.get('delfs'):
            self.del_fs()
        if exec_all or steps.get('delnsd'):
            self.del_nsds()
            self.verify_nsds()
        if exec_all or steps.get('shutdown'):
            self.shutdown_scale()
        # There does not exist a 'only' remove the cluster. If the cluster is removed every step has to be done before.
        if exec_all:
            self.del_cluster()
            self.verify_nodes()

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

    def del_cluster(self):
        """Function removes all nodes from Scale cluster"""
        tprint('Removing all nodes from the cluster...')
        cmd = 'mmdelnode -a'
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % cmd)
        else:
            util.exec_shell(cmd)
        tprint('All nodes removed from the cluster.\n')

    def verify_nodes(self):
        """Function verifies that nodes are removed from the Scale cluster

        Returns:
            bool: True if successful, False otherwise."""
        pass

    def del_nsds(self):
        """Function deletes NSDs based on the earlier generated stanze build"""
        tprint('Removing Scale NSDs based on stanza file...')
        # generate all NSD names for all nodes
        nsd_names = [node.gen_scale_nsd_names() for node in self._nodes if len(node.devices) > 0]
        # flatten list and convert into a semi colon separated string
        nsd_names = ';'.join([i for sublist in nsd_names for i in sublist])
        cmd = 'mmdelnsd \'%s\'' % nsd_names
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % cmd)
        else:
            util.exec_shell(cmd)
        tprint('NSDs created.\n')

    def verify_nsds(self):
        """Function verifies that all nsds were successfully created"""
        pass

    def shutdown_scale(self):
        """Function stops daemons of all nodes belonging to the cluster"""
        tprint('Shutdown Spectrum Scale...')
        cmd = 'mmshutdown -a'
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % cmd)
        else:
            util.exec_shell(cmd)
        tprint('Shutdown complete.\n')

    def del_fs(self):
        """Function removes file system with specified name"""
        tprint('Removing file system %s...' % self._fsname)
        # TODO version must be removed if cluster is configured properly...
        cmd = 'mmdelfs %s --version=4.1.1.0' % self._fsname
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % cmd)
        else:
            util.exec_shell(cmd)
        tprint('Deletion complete.\n')
        pass

    def umount_fs(self):
        """Function unmounts the file system on the specified mountpoint"""
        tprint('Unmounting file system %s at /gpfs/%s...' % (self._fsname, self._fsname))
        cmd = 'mmumount %s all' % self._fsname
        # Execute command
        if self._pretend:
            tprint('Would execute if not pretending:\n\t%s' % cmd)
        else:
            util.exec_shell(cmd)
        # Just give it some time to propagate the command through the cluster
        # time.sleep(5)
        tprint('Unmounting complete.')


if __name__ == "__main__":
    # Init parser
    parser = argparse.ArgumentParser(description='This script deletes a Spectrum Scale cluster, including NSDs, \
and file system. \nAll based on a configuration file and executed \
in order automatically. \nThe user may also only specify parts of \
the process to be executed.', formatter_class=argparse.RawTextHelpFormatter)
    # positional arguments
    parser.add_argument('conf', type=str,
                        help='Path to the config file of the temporary cluster')
    # optional arguments
    parser.add_argument('-p', '--pretend', action='store_true',
                        help='''No execution of file system commands
The stanza file will be created''')
    parser.add_argument('--umntfs', action='store_true',
                        help='''Unmounts _only_ the file system for the given file system name''')
    parser.add_argument('--delfs', action='store_true',
                        help='''Removes _only_ the file system for the given configuration
Note: File system will be unmounted as well''')
    parser.add_argument('--delnsd', action='store_true',
                        help='''Removes _only_ the NSDs for the given configuration
Note: The specified file system will be removed as well
Cluster remains intact''')
    parser.add_argument('--shutdown', action='store_true',
                        help='Startup Scale _only_')
    args = parser.parse_args()
    # create prototype object
    scale = DeleteScaleProto(args.conf, args.pretend)
    scale.init_system(delnsd=args.delnsd, shutdown=args.shutdown,
                      delfs=args.delfs, umountfs=args.umntfs)

    print '\nNothing left to do; exiting. :)'
