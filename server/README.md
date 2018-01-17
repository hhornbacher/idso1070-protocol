# Server

This is an example application for how to use this library. The server cann be controlled by a REST API, the sampling data can be streamed in raw binary format over TCP.

There is also a gnuradio flowgraph as client to visualize the sampled data: `client.grc` / `client.py`

## REST endpoints

The server binds to port 8800 for API calls.

* `GET /status`: get general connection and device status
* `GET /device`: get device configuration
* `GET /trigger`: get trigger configuration
* `GET /channel/1`: get channel 1 configuration
* `GET /channel/2`: get channel 2 configuration

## Sample streams

The server will try to open two ports: `8810` (Channel 1 data) and `8820` (Channel 2 data). The stream consists of the raw 16bit integer values read from the device.