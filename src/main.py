# -*- coding: utf-8 -*-
# Script for initializing a Scale filesystem
# Marc-Andre Vef
# Version 0.1 (08/20/2015)

import sys
import os

from util import util

class ScaleProto(object):

    def init_system(self):
        """Main function triggering all steps necessary for Scale initiation"""
        pass

    def parse_input(self):
        """Function parses user input (nodes, mgrnodes, storage) and puts them into appropriate data structures"""
        pass

    def verify_input(self):
        """Function verifies given input for correctness.

        Returns:
            bool: True if successful, False otherwise."""
        pass

    def add_nodes(self):
        """Function adds the nodes to Scale cluster including mgr nodes"""
        pass

    def verify_nodes(self):
        """Function verifies that nodes are added to the Scale cluster as specified

        Returns:
            bool: True if successful, False otherwise."""
        pass

    def build_stanzas(self):
        """Function builds stanzas from the user input, required for generating NSDs later"""
        pass

    def create_nsds(self):
        """Function creates NSDs from the earlier generated stanze build"""
        pass

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
    if len(sys.argv) in [1, 2, 3]:
        print usage
        sys.exit(1)

    # parsing input
    mpi_processes, hosts, hostfile, mdt_path, iterations, depth, branches, items, wd, fcreate_only, unique_dir, static, create_only, remove_only, mdtest_verbosity, restart_gpfs, warmup, disks = None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, False, False, ''
    while len(sys.argv) > 1:
        option = sys.argv[1]
        del sys.argv[1]
        if option == '-host':
            hosts = sys.argv[1]
            del sys.argv[1]
        elif option == '-hostfile':
            hostfile = sys.argv[1]
            del sys.argv[1]
            if not os.path.exists(hostfile):
                print 'ERR: Path to host file is invalid. Please make sure to give the right path.'
                sys.exit(1)
        elif option == '-mdt_path':
            mdt_path = util.rm_trailing_slash(sys.argv[1])
            del sys.argv[1]
            if not os.path.exists(mdt_path):
                print 'ERR: Path to mdtest executable is invalid. Please make sure to give the right path.'
                sys.exit(1)
    mdtest = MDTest(mdt_path, iterations, depth, branches, wd, items, mdtest_verbosity, create_only, remove_only,
                    fcreate_only, unique_dir, static, disks, warmup, restart_gpfs, hosts, hostfile)
    mdtest.exec_periodictest()
    print '\nNothing left to do; exiting. :)'