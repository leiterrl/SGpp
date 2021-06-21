#!/bin/bash

# exit when any command fails
set -e

PY_VER="cp37-cp37m"

export PATH=/opt/python/"${PY_VER}"/bin:$PATH

/opt/python/"${PY_VER}"/bin/pip install --upgrade --no-cache-dir pip
/opt/python/"${PY_VER}"/bin/pip install --no-cache-dir scons numpy

/opt/python/"${PY_VER}"/bin/scons -j2 SG_ALL=0 SG_BASE=1 SG_PYTHON=1 CHECK_STYLE=0
