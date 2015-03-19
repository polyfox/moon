#!/bin/bash
oldwd=$(pwd)
configfile="${oldwd}/mrb_config.rb"

clean-hard-mruby() {
  rm -vrf vendor/mruby/build
}

clean-hard() {
  rm -vrf vendor/sil/build
  rm -vrf vendor/soil/build
  rm -vrf vendor/nanovg/build
  rm -vrf vendor/freetype-gl/build
  rm -vrf vendor/glfw/build
  rm -vrf vendor/gorilla-audio/build/build
  clean-hard-mruby
}

clean-sil() {
  cd "${oldwd}"
  cd "vendor/sil" &&
  cd build &&
  make clean
  cd "${oldwd}"
}

clean-soil() {
  cd "vendor/soil" &&
  cd build &&
  make clean
  cd "${oldwd}"
}

clean-nanovg() {
  cd "vendor/nanovg" &&
  cd build &&
  make clean
  cd "${oldwd}"
}

clean-freetype-gl() {
  cd "vendor/freetype-gl" &&
  cd build &&
  make clean
  cd "${oldwd}"
}

clean-glfw() {
  cd "vendor/glfw" &&
  cd build &&
  make clean
  cd "${oldwd}"
}

clean-gorilla-audio() {
  cd "vendor/gorilla-audio" &&
  cd build/build &&
  make clean
  cd "${oldwd}"
}

clean-mruby() {
  cd "vendor/mruby" &&
  rake clean
  cd "${oldwd}"
}

clean() {
  clean-sil
  clean-soil
  clean-nanovg
  clean-freetype-gl
  clean-glfw
  clean-gorilla-audio
  clean-mruby
}

build() {
  local j=4
  cd "${oldwd}"
  cd "vendor/sil" &&
  (premake4 gmake &&
     cd build &&
     make -j ${j} SIL) || exit
  cd "${oldwd}"

  cd "vendor/soil" &&
  (premake4 gmake &&
     cd build &&
     make -j ${j} SOIL) || exit
  cd "${oldwd}"

  # build nanovg
  cd "vendor/nanovg" &&
  (premake4 gmake &&
   cd build &&
   make -j ${j} nanovg) || exit
  cd "${oldwd}"

  cd "vendor/freetype-gl" &&
  mkdir -p build &&
  cd build &&
  (cmake .. &&
     make -j ${j}) || exit
  cd "${oldwd}"

  cd "vendor/glfw" &&
  mkdir -p build &&
  cd build &&
  (cmake .. &&
     make -j ${j}) || exit
  cd "${oldwd}"

  cd "vendor/gorilla-audio" &&
  mkdir -p build/build &&
  cd build/build &&
  (cmake .. &&
     make -j ${j}) || exit
  cd "${oldwd}"

  cd "vendor/mruby" &&
  (MRUBY_CONFIG="${configfile}" rake -j ${j}) || exit
  cd "${oldwd}"
}

mtest() {
  cd "${oldwd}"
  cd "vendor/mruby" &&
  MRUBY_CONFIG="${configfile}" rake -j ${j} test
  cd "${oldwd}"
}

if [[ -z "$@" ]] ; then
  build
else
  while ! [[ -z "$@" ]] ; do
    case "$@" in
      "build")        build ;;
      "clean-hard")   clean-hard ;;
      "clean-mruby")  clean-mruby ;;
      "clean")        clean ;;
    esac
    shift
  done
fi
