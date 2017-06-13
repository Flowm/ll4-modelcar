#!/usr/bin/env bash

#export DEBIAN_FRONTEND=noninteractive
#sudo apt-get update
#sudo apt-get upgrade
#sudo apt-get install -y libncurses5-dev texinfo autogen autoconf2.64 g++ libexpat1-dev flex bison gperf cmake libxml2-dev libtool zlib1g-dev libglib2.0-dev make pkg-config gawk subversion expect git libxml2-utils syslinux xsltproc yasm iasl lynx unzip qemu alsa-base alsa-utils pulseaudio pulseaudio-utils ubuntu-desktop tftpd-hpa

INSTALL_DIR=/var/tmp/modelcar
cd $INSTALL_DIR

git clone https://github.com/argos-research/operating-system.git

rm -rf $INSTALL_DIR/genode/contrib/

git submodule init
git submodule update

tar xfj genode/genode-toolchain-15.05-x86_64.tar.bz2

wget https://nextcloud.os.in.tum.de/s/KVfFOeRXVszFROl/download --no-check-certificate -O libports.tar.bz2
tar xvjC genode/ -f libports.tar.bz2

make
