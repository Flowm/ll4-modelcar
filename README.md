LL4 Team Model Car
===
Actuaction of a model car based on simulation data from SpeedDreams.
All components in the model car (Pandaboard, RaspberryPI) are running Genode with Fiasco.OC.

# Architecture
Control data is received on a MQTT topic, processed on the ECU (pandaboard) and then passed to the servo controller (Raspberry PI).


# Install
Execute setup.sh

# Build
## PandaBoard
```
PROJECT=car_panda GENODE_TARGET=focnados_panda make build_dir
PROJECT=car_panda GENODE_TARGET=focnados_panda make run
```

## Raspberry PI
```
PROJECT=car_rpi GENODE_TARGET=focnados_rpi make build_dir
PROJECT=car_rpi GENODE_TARGET=focnados_rpi make run
```
