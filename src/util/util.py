#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Script contains different useful functions
# Marc-Andre Vef
# Version 0.1 (06/19/2015)

import os
import sys
import shutil
import time
from subprocess import Popen, PIPE


def rm_trailing_slash(path):
    """Removes the trailing slash from a given path"""
    return path[:-1] if path[-1] == '/' else path


def create_dir(path):
    """creates a directory at the paths location. Must be an absolute path"""
    try:
        if not os.path.exists(path):
            os.makedirs(path)
    except OSError as e:
        print 'Error: Output directory could not be created.'
        print e.strerror
        sys.exit(1)


def rm_rf(path):
    """rm -rf path"""
    try:
        shutil.rmtree(path)
    except shutil.Error as e:
        print 'Warning: Could not delete path %s' % path
        print e.strerror


def rm_file(path):
    """remove a file"""
    try:
        os.remove(path)
    except OSError as e:
        print 'Warning: Could not delete file %s' % path
        print e.strerror


def tprint(toprint, nobreak=False):
    """Function adds current time to input and prints it to the terminal

    Args:
        toprint (str): The string to be printed.
        nobreak (bool): True if no line break should occur after print. Defaults to False.

    """
    curr_time = time.strftime('[%H:%M:%S]')
    if nobreak:
        print '%s\t%s' %(curr_time, toprint),
    else:
        print '%s\t%s' %(curr_time, toprint)


def exec_shell(cmd, suppress_output=False):
    """Function executes a cmd on the shell

    Args:
        cmd (str): The command to be executed.
        suppress_output (bool): Surpresses command output. Defaults to False.

    Returns:
        (str): Last line of executed command output.

    Raises:
        OSError: If command could not be executed.
    """
    try:
        if cmd == '':
            raise OSError("Command string is empty.")
        # simulate change directory to where mdtest executable is located
        modified_cmd = '%s' % cmd
        p = Popen(modified_cmd, shell=True, stdout=PIPE, stderr=PIPE)
        # Poll process for new output until finished
        output = ''
        while True:
            nextline = p.stdout.readline()
            if nextline == '' and p.poll() is not None:
                break
            output = nextline
            if not suppress_output:
                curr_time = time.strftime('[%H:%M:%S]  ')
                sys.stdout.write('%s%s' % (curr_time, nextline))
                sys.stdout.flush()
        stdout, stderr = p.communicate()
        return output.strip()
    except OSError as e:
        print 'ERR when executing shell command'
        print e.strerror
