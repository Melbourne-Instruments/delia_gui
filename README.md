# DELIA GUI App #

DELIA GUI for the Raspberry Pi (64-bit), using QT5.

### Building the DELIA GUI app ###

To clone the DELIA GUI app the --recurse-submodules flag must be used:

$ git clone --recurse-submodules https://github.com/Melbourne-Instruments/delia_gui.git

The DELIA GUI app uses QMake as its build system, which creates the makefile from the 
monique_gui.pro file. To create the makefile:

$ qmake

Once created, make is used to build the DELIA GUI app:

$ make

To cross compile for the Raspberry Pi, the Melbourne Instruments SDK *must* be used.
It is recommended to install the SDK in the /opt folder on your Ubuntu PC.

Once this has been done, source the environment script to set-up the build environment, for example:

$ source /opt/delia/1.0.0/environment-setup-cortexa72-elk-linux

### Dependancies ###

  * QT5

---
Copyright 2023-2024 Melbourne Instruments, Australia.
