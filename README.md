# IDSO1070A Protocol Library (for Linux)

This is a reverse engineered implementation of the protocol used to communicate with the mixed signal oscilloscope IDSO1070A from Hantek.
It's possible to communicate with the device over WiFi (TCP) as well as USB.

## Directories

* cmake: Helper scripts for cmake
* lib: Implementation of the protocol itself
* src: Simple program using the library (at the moment just an example)

## Build instructions

Just run the ```build.sh```.
There is also a preconfigured task for building (CTRL-SHIFT-B) in vscode and a launch.json to debug(F5).

## Dependencies

* GCC
* CMake: v3.0
* libboost: v1.58.0

## Attention

This is not a production ready version, there's still a lot of stuff to do. If you're interested to support this library, just contact me.
