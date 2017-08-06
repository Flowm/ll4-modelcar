LL4 Team Model Car
===
Actuaction of a model car based on simulation data from SpeedDreams.
All components in the model car (Pandaboard, RaspberryPI) are running Genode with Fiasco.OC.

# Architecture
Control data is received on a MQTT topic, processed on the ECU (pandaboard) and then passed via MQTT to the high level servo controller (Raspberry PI).
It comunicates with the low level servo controller (pololu maestro) over a serial port which then controls the servos with a PWM signal.

# Install
Execute `setup.sh` and follow the printed instructions.

# Build
## PandaBoard
```
make setup
make run
```

Alternatively from genode directory:
```
PROJECT=car_panda GENODE_TARGET=focnados_panda make build_dir
PROJECT=car_panda GENODE_TARGET=focnados_panda make run
```

## Raspberry PI
```
make setup
make run
```

Alternatively from genode directory:
```
PROJECT=car_rpi GENODE_TARGET=focnados_rpi make build_dir
PROJECT=car_rpi GENODE_TARGET=focnados_rpi make run
```
