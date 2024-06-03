# nidhoggr

```
                               
     _   _ _                  
 ___|_|_| | |_ ___ ___ ___ ___
|   | | . |   | . | . | . |  _|
|_|_|_|___|_|_|___|_  |_  |_|  
      v0.7.0      |___|___|    

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
Requires libsilo-dev so "sudo apt-get install libsilo-dev"
if you don't already have it.

Hopefully you already have python somewhere...

Consult the Classes.md files in src subdirectories for interface guides.