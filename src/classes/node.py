#!/usr/bin/env python
# -*- coding: utf-8 -*-

__author__ = "Marc-Andre Vef"
__version__ = "1.0"
__email__ = "vef@uni-mainz.de"


class Node(object):
    """Node object describes a node in a cluster and its devices.

    Args:
        nodename (str): Node name in the cluster. Defaults to "".
        devices (List[str]): List of storage devices accessible by the node. Defaults to [].
        manager (Optional[bool]): If assigned manager node. Defaults to False.

    Attributes:
        _nodename (str): Node name in the cluster.
        _devices (List[str]): List of storage devices accessible by the node.
        _manager (bool): If assigned manager node.
        _nsds (List[str]): List of stanza strings, including line breaks

    """
    def __init__(self, nodename="", devices=None, manager=False):
        self._nodename = nodename
        if devices is None:
            devices = []
        self._devices = devices
        self._manager = manager
        self._nsds = []

    def gen_nsds(self):
        """
        Generates a list of stanza strings for the given node object for all devices.
        Example for a generated NSD per device:
        %nsd:
        device=/dev/ssd
        nsd=TMPSCALE_ARGON_SSD
        servers=argon
        usage=dataAndMetadata
        failureGroup=-1
        pool=system

        Returns:
            List[str]: List of stanza strings, including line breaks

        """
        self._nsds = []
        for device in self._devices:
            nsd = '%%nsd:\n' \
                  'device=%s\n' \
                  'nsd=TMPSCALE_%s_%s\n' \
                  'servers=%s\n' \
                  'usage=dataAndMetadata\n' \
                  'failureGroup=-1\n' \
                  'pool=system\n' % (device, self._nodename.upper(), device.split('/')[-1].upper(), self._nodename)
            self._nsds.append(nsd)
        return ''.join(self._nsds)

    @property
    def nodename(self):
        """str: Get or set the node name of the node object."""
        return self._nodename

    @nodename.setter
    def nodename(self, nodename):
        self._nodename = nodename

    @property
    def devices(self):
        """List[str]: Get or set the devices of the node"""
        return self._devices

    @devices.setter
    def devices(self, devices):
        self._devices = devices

    @property
    def manager(self):
        """bool: Get or set if manager is assigned a manager node"""
        return self._manager

    @manager.setter
    def manager(self, manager):
        self._manager = manager
