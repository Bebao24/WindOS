#!/bin/bash

set -e

# $1 -> Image path, $2 -> Mount directory, $3 -> Target system
if [ -z $1 ] || [ -z $2 ] || [ -z $3 ]; then
    echo "Syntax: <Image path> <Mount directory> <Target system>"
    exit 1
fi

SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

if test -f /dev/loop101 || test -f /dev/loop102; then
    echo "/dev/loop101 or /dev/loop102 aren't avaliable!"
    exit 1
fi

LIMINE_DIR="${SCRIPTPATH}/../../bootloader/limine"
LIMINE_EXEC="${LIMINE_DIR}/limine"

if [ ! -f "${LIMINE_EXEC}" ]; then
    echo "Please compile limine first!"
    exit 1
fi

dd if=/dev/zero of=$1 bs=512 count=93750
parted $1 mklabel msdos
parted $1 mkpart primary fat32 2048s 100%
parted $1 set 1 boot on
$LIMINE_EXEC bios-install $1

sudo losetup /dev/loop101 $1
sudo losetup /dev/loop102 $1 -o 1048576 # 1 MB

sudo mkfs.fat -F 32 /dev/loop102
sudo fatlabel /dev/loop102 LIMINE

sudo mkdir -p $2
sudo mount /dev/loop102 $2

sudo mkdir -p "${2}/boot/limine" "${2}/boot/EFI/BOOT"
sudo cp "$LIMINE_DIR/limine-bios.sys" "${2}/boot/limine/"
sudo cp "$LIMINE_DIR/BOOTX64.EFI" "${2}/boot/EFI/BOOT"
sudo cp "$LIMINE_DIR/BOOTIA32.EFI" "${2}/boot/EFI/BOOT"

sudo cp -r ${SCRIPTPATH}/../../target/* "${2}/"

CURRENT_DIRECTORY=$(dirname "$0")
chmod +x "${CURRENT_DIRECTORY}/cleanup.sh"
"${CURRENT_DIRECTORY}/cleanup.sh" "${2}"


