#!/usr/bin/env bash
set -eux

usage() {
	echo -e "Usage: $0 <destination>"
	echo -e "\t<destination> Target directory" 1>&2
	exit 1
}

OS_DIR=~/dev/operating-system
if [ $# -ne 1 ]; then
	usage
	exit
fi
OS_DIR=$1

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
git clone git@gitlab.lrz.de:lil4/carFranzG.git genode/repos/car_project

echo "Configuration of build dir $OS_DIR successful"
