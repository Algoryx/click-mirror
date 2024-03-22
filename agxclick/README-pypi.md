# agxclick

agxclick uses [pclick](https://pypi.org/project/pclick/), [AGX Dynamics](https://www.algoryx.se/agx-dynamics/) and [agxBrick](https://pypi.org/project/agxBrick/) to a implement a simulation application that implements Click out of the box for a provided Brick model.

- You can use click_application.py (see below) to load any Brick model and it will find the robots and connect their signals to Click. See Brick Model Requirements below for more info.
- You can Inherit agxclick.ClickApplication and override it's methods to customize your own application.
- You can connect with any Click client to the simulation.

The flow is the same as for Click

1. Client controller connects and sends HandshakeInit
2. Server responds with Handshake
3. Client receives Handshake and validates the setup.
4. Client sends ControlMessage
5. Server steps Simulation, and responds with SensorMessage
6. The loop 4-5 is repeated.

NOTE: The Controller step and the simulation step is in full sync, meaning that the simulation will only progress on ControlMessages.

## Install

Prerequisites: AGX and agxBrick

```bash
# Latest version
pip install agxclick -U
# Specific version
pip install agxclick==0.3.1
```

## Usage Examples

Visit the [GitHub repo](https://github.com/algoryx/click-mirror/agxClick/README.md) for usage examples.

## License

  [Apache License 2.0](https://github.com/algoryx/click-mirror/LICENSE)
