#!/bin/sh

# Copyright (c) Deptrum Inc. All Rights Reserved.

# Check if user is root/running with sudo
if [ "$(id -u)" -ne 0 ]; then
    echo "This script requires root privileges. Please enter your password:"
    exec sudo "$0" "$@"
    exit $?
fi

ORIG_PATH=`pwd`
cd `dirname $0`
SCRIPT_PATH=`pwd`
cd $ORIG_PATH

if [ "`uname -s`" != "Darwin" ]; then
    # Install udev rules for USB device
    cp ${SCRIPT_PATH}/99-deptrum-libusb.rules /etc/udev/rules.d/99-deptrum-libusb.rules

    # resload udev rules
    udevadm control --reload && udevadm trigger

    echo "usb rules file install at /etc/udev/rules.d/99-deptrum-libusb.rules"
fi
echo "exit"
