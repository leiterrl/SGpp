#!/bin/bash

# exit when any command fails
set -e

PY_VER="cp37-cp37m"

/opt/python/"${PY_VER}"/bin/pip install --no-cache-dir scons

/opt/python/"${PY_VER}"/bin/scons -j2 SG_ALL=0 SG_BASE=1 SG_PYTHON=1
