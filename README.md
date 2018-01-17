# IDSO1070 Protocol Library (for Linux)

This is a reverse engineered implementation of the protocol used to communicate with the mixed signal oscilloscope IDSO1070 from Hantek.
It's possible to communicate with the device over WiFi (TCP) as well as USB.

USB ID: ```0483:5740 [STMicroelectronics STM32F407]```

## Dependencies

* GCC
* CMake: v3.5
* Boost: v1.58.0

On Ubuntu/Mint you can run ```apt install libboost-all-dev cmake gcc``` to install the dependencies.

## Build instructions

Just run the ```build.sh``` or execute following commands in your shell:

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

There is also a preconfigured task for building (CTRL-SHIFT-B) in vscode and a launch.json to debug(F5).

## Directories

* cmake: Helper scripts for cmake
* include: Library headers
* lib: Implementation of the protocol itself
* server: Simple tcp server using this library

## Attention

This is not a production ready version, there's still a lot of stuff to do. If you're interested to support this library, just contact me.

## Links

* Product page: http://www.hantek.com/en/ProductDetail_2_10165.html
* [sigrok: Hardware photos & information only, not supported by the library](https://sigrok.org/wiki/Hantek_iDSO1070)