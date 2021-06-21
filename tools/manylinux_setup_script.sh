#!/bin/bash

# exit when any command fails
set -e

scons -j2 SG_ALL=0 SG_BASE=1 SG_PYTHON=1
