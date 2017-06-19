#!/usr/bin/env bash
set -ex

git clone https://github.com/argos-research/testbed.git

git clone git@gitlab.lrz.de:lil4/carFranzG.git

git clone git@github.com:Flowm/operating-system.git
cd operating-system
./provision.sh
