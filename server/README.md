# Server

This is an example application for how to use this library. The server cann be controlle with a REST API, the sampling data can be streamed in raw binary format over TCP.

There is also a gnuradio flowgraph as client to visualize the sampled data: `client.grc` / `client.py`

## REST endpoints

* `GET /status`: get general connection and device status
* `GET /device`: get device configuration
* `GET /trigger`: get trigger configuration
* `GET /channel/1`: get channel 1 configuration
* `GET /channel/2`: get channel 2 configuration