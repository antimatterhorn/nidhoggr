# nidhoggr

```bash
git pull
git submodule update --init --recursive

mkdir build

Nidhoggr/
|-- src/
|   |-- Math/
|   |   |-- vectorMath.cc
|   |   |-- vectorMath.py
|   |   |-- ...
|   |-- ...
|-- tests/
|-- build/

inside build directory->
cmake ..
make

```

for now, hard link the test files into the build/src directory to use them. 