# Click

The main idea behind click is to enable a non-OpenPLX controller talking to an OpenPLX enabled [AGX Dynamics](https://www.algoryx.se/agx-dynamics/) Simulation in a way configurable by OpenPLX.


There are three main considerations

1. How the controller can send controls and receive sensor values in a similar fashion regardless of environment, ie real or sim.
2. How OpenPLX adds sensors or topology on top of urdf, and how this is communicated to the controller.
3. How to communicate controls and sensors in an effective way.

The current solution is to introduce a Handshake, which enables the simulation to tell the controller what to expect in terms of how to control and what sensor data is being sent.

A typical flow is

1. Client controller connects and sends HandshakeInit
2. Server responds with Handshake
3. Client receives Handshake and validates the setup.
4. Client sends Controls
5. Server responds with Sensors
6. The loop 4-5 is repeated.

## Run demo

```bash
python3.9 -m pclick.demo.server --trace
```

```bash
python3 -m pclick.demo.client
```

## Use

```python
import pclick
```

## Usage Examples

Visit the [GitHub repo](https://github.com/algoryx/click-mirror) for more info and usage examples.

## License

[Apache License 2.0](https://github.com/algoryx/click-mirror/LICENSE)
