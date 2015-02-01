#!/bin/bash
oldwd=$(pwd)
configfile="${oldwd}/mrb_config.rb"

cd "vendor/nanovg"
premake4 gmake
cd build
make nanovg
cd "${oldwd}"

cd "vendor/mruby"
./minirake MRUBY_CONFIG="${configfile}"
cd "${oldwd}"
