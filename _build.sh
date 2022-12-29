#!/bin/bash

rm -rf build bin/exe
mkdir build && cd build && cmake .. && make