#!/bin/bash

set -e

# $1 -> Mounted directory
if [ -z "$1" ]; then
    echo "Syntax: <Mounted directory>"
    exit 1
fi

echo "Cleaning up..."

sudo umount -l "${1}"
sudo losetup -d /dev/loop101
sudo losetup -d /dev/loop102
