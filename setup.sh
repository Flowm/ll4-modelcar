#!/usr/bin/env bash
set -eux

OS_DIR=~/dev/operating-system

# Clone genode os repo
if ! [ -d "$OS_DIR" ]; then
    mkdir -p ${OS_DIR%/*}
    git clone https://github.com/argos-research/operating-system $OS_DIR
fi
cd $OS_DIR

# Initial setup
./provision.sh

# Clone additional reposistories
git clone https://github.com/argos-research/genode-world.git genode/repos/genode-world
git clone git@gitlab.lrz.de:lil4/carFranzG.git genode/repos/car-project

echo "Configuration of build dir $OS_DIR successful"
