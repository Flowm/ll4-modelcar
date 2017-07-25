Git repo for team modelCar
===

# Setup
Execute setup.sh

# Build
## PandaBoard
PROJECT=car_panda GENODE_TARGET=focnados_panda make build_dir
PROJECT=car_panda GENODE_TARGET=focnados_panda make

## Raspberry PI
PROJECT=car_rpi GENODE_TARGET=focnados_rpi make build_dir
PROJECT=car_rpi GENODE_TARGET=focnados_rpi make
