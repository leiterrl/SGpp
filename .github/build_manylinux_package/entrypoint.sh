#!/bin/bash

# exit when any command fails
set -e

PY_VER="cp37-cp37m"

export PATH=/opt/python/"${PY_VER}"/bin:$PATH
python --version

pip install --upgrade --no-cache-dir pip
pip install --no-cache-dir scons numpy

cd /github/workspace

/opt/python/"${PY_VER}"/bin/scons -j2 SG_ALL=0 SG_BASE=1 SG_PYTHON=1 CHECK_STYLE=0

export LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH

yum install -y doxygen

scons -j8 SG_JAVA=0 RUN_BOOST_TESTS=0 CHECK_STYLE=0 PYDOC=1 LINKFLAGS=-s

python setup.py bdist_wheel

auditwheel repair dist/pysgpp-3.4.0-py3-none-any.whl --plat manylinux2014_x86_64

