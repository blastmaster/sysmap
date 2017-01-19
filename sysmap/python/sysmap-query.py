#! /usr/bin/env python

import sys
import argparse

import pprint

from sysmap.database import SystemMapDatabase

__author__ = "Sebastian Oeste"
__email__ = "sebastian.oeste@tu-dresden.de"


def parse_arguments():

    parser = argparse.ArgumentParser(description="Query sysmap informations from database.")
    parser.add_argument('-c', '--conf', type=str, help="Configuration file for the database connection and authentification.")
    parser.add_argument('nodes', type=str, nargs='+', help="The set of nodes that should be processed in the query.")
    parser.add_argument('facts', type=str, nargs='+', help="The set of facts that should queried in the set of hosts.")

    return parser.parse_args()


def main():
    args = parse_arguments()
    pprint.pprint(args)


main()
