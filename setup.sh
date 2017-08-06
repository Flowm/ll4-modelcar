#!/usr/bin/env bash
set -eu

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

echo
echo "Configuration of build repo $OS_DIR successful"
echo "A version of this repository with configured build environment was cloned to $OS_DIR/genode/repos/car_project"
echo

echo "Please add the following lines to $OS_DIR/Makefile:"
echo "- To libports target:"
echo "	./genode/tool/ports/prepare_port zlib"
echo "	./genode/tool/ports/prepare_port openssl"
echo "	./genode/tool/ports/prepare_port libprotobuf"
echo "	./genode/tool/ports/prepare_port libmosquitto"
echo "- To build_dir target:"
echo "	printf 'REPOSITORIES += \$\$(GENODE_DIR)/repos/genode-world\n' >> \$(VAGRANT_BUILD_CONF)"
echo "	printf 'REPOSITORIES += \$\$(GENODE_DIR)/repos/car_project\n' >> \$(VAGRANT_BUILD_CONF)"
