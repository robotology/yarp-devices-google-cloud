![YARP logo](doc/images/yarp-robot-24.png?raw=true "yarp-device-template")

Yarp device for google-cloud-cpp vocal interaction devices
=====================

This repository contains the YARP plugin for google-cloud-cpp vocal interaction devices.

:construction: This repository is currently work in progress. :construction:
:construction: The software contained is this repository is currently under testing. :construction: APIs may change without any warning. :construction: This code should be not used before its first official release :construction:

Documentation
-------------

Documentation of the individual devices is provided in the official Yarp documentation page:
[![YARP documentation](https://img.shields.io/badge/Documentation-yarp.it-19c2d8.svg)](https://yarp.it/latest/group__dev__impl.html)


Installation
-------------

### Dependencies
These devices all depend from [google-cloud-cpp](https://github.com/googleapis/google-cloud-cpp/tree/main)
To make them easily buildable without having to build Google libraries from source, the version needed to compile
the repo is [V2.12](https://github.com/googleapis/google-cloud-cpp/releases/tag/v2.12.0) which binaries are available in `mamba` packages.

If you do not have it already installed, follow [these instructions](https://github.com/conda-forge/miniforge#mambaforge)

After that run the following command:
~~~
mamba install google-cloud-cpp
~~~

### Build with pure CMake commands

~~~
# Configure, compile and install
cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=<install_prefix>
cmake --build build
cmake --build build --target install
~~~

CI Status
---------

:construction: This repository is currently work in progress. :construction:

[![Build Status](https://github.com/robotology/yarp-device-template/workflows/CI%20Workflow/badge.svg)](https://github.com/robotology/yarp-device-template/actions?query=workflow%3A%22CI+Workflow%22)

License
---------

:construction: This repository is currently work in progress. :construction:

Maintainers
--------------
This repository is maintained by:

| | |
|:---:|:---:|
| [<img src="https://github.com/elandini84.png" width="40">](https://github.com/elandini84) | [@elandini84](https://github.com/elandini84) |
