# What is it?

The Hadou-CAN is a USB to CAN FD adapter supporting the newest CAN standard, CAN FD. It uses the standard Lawicel text mode protocol and is compatible with slcand on Linux.

The electrically isolated CAN side protects your computer in high power robotics applications, in distributed networks where the ground offset may be large, or anywhere common mode noise is significant.

The first-in-class PPS timesync feature allows any number of Hadou-CAN to synchronize the timestamps on messages from separate, isolated CAN buses. A Hadou-CAN may be initialized as a master source of the PPS sync signal, or as a slave to receive sync signals and track the master clock. Incoming packets are automatically tagged with the synchonized timestamps, so that debugging large systems with multiple, isolated CAN buses has never been easier.

# Supported Platforms
* Linux
* Windows

# What makes it special?
* CAN FD support at 12 Mbps
* A timesync input/output allows timestamps to match between several networks 
* Electrical isolation between the USB and CAN bus sides to protect your equipment
* Bootloader with support for loading custom user programs
* Blue extruded Al case option
* Jumpers to add and remove termination resistors
* Automatic slew rate control for low EMI or high speed

# Connectivity
* USB: type B, USB 2.0 device
* CAN: DE-9
* Timesync: SMA, 3.3V 50 ohm

# Features coming soon
* Non-ISO 11898-1 format support
* HW ID filters & masks
* Enhancements to the USB stack

# Documentation

User Guide
https://drive.google.com/open?id=1MtYEAVDF2ImoV_6nmfjP3o9sGMMwxck2wRqLRCWhaTU

Datasheet
https://drive.google.com/open?id=1wTyMhcp9QPFBzmYxsuicrPzenbGGubfI90Ha8eGbijM

Quickstart Guide
https://drive.google.com/open?id=1zZpIIZvTiL2FCnMaklSey-5uLwT95EifO-A8F-x2TWk

# Store

https://www.tindie.com/products/suburbanembedded/hadou-can-usb-can-fd-adapter/

# Copyright

Copyright (c) 2018-2019 Suburban Embedded

# License

Licensed under the terms of the 3-Clause BSD license. See LICENSE for details.
