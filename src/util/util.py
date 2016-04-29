# Script contains different useful functions
# Marc-Andre Vef
# Version 0.1 (06/19/2015)

import os
import tarfile
import sys
import shutil


def timer(start, end):
    """Returns a string of elapsed time between two timestamps"""
    hours, rem = divmod(end - start, 3600)
    minutes, seconds = divmod(rem, 60)
    return str('{0:0>2}:{1:0>2}:{2:05.2f}'.format(int(hours), int(minutes), seconds))


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


def file_len(fname):
    i = 0
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1


def cr_compressed_tar(source, compression='bz2'):
    """creates a tar with compression of gzip or bzip2 (default)
    @return True if compression sucessful, False if given compression string is invalid"""
    if compression not in ['bz2', 'gz']:
        return False
    # Was done with with clause, but tar file of earlier python versions doesn't support closing with with.
    try:
        tar = tarfile.open('%s.tar.%s' % (source, compression), 'w:%s' % compression)
        tar.add(source, arcname=os.path.basename(source))
    except tarfile.TarError as e:
        print e.message
    finally:
        tar.close()
    return True


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


def fprint(input, console=True, filename='', mode='a'):
    """Function prints input to console by default and also to file if specified"""
    if console:
        print input
    if filename != '':
        with open(filename, mode) as file:
            file.write(input)
