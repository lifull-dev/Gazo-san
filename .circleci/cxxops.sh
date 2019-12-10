#!/bin/bash

mkdir -p include
curl -OLsS 'https://raw.githubusercontent.com/jarro2783/cxxopts/master/include/cxxopts.hpp'
mv cxxopts.hpp include/