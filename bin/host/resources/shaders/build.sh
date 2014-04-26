#!/bin/bash

if [ -z $PP ]; then
  if type gcc > /dev/null ; then
    PP=gcc
    echo "Using GCC"
  elif type clang > /dev/null ; then
    # clang leaves weird spacing before the varying be warned
    PP=clang
    echo "Using Clang"
  else
    echo "No usable preprocessor found"
    exit
  fi
fi

compile_shader() {
  local glslv=${1}
  local infile=${2}
  local outfile=${3}
  local tmpfile="${outfile}.tmp"

  $PP -nostdinc -E -xc -D__GLSL_VERSION__=$glslv -include common.h "${infile}" -o "${tmpfile}"

  ###
  # Clean up:
  # The PP eats all the new lines if the -P option is used, this leaves
  # jumbled code and is annoying at times.
  #
  # So whats happening here? Its removing the PPs line markers,
  # and the replacing single spaces at the start of a line with nothing.
  #
  # This is done for the layout args in 120, since they leave an extra
  # space before the varying/attribute declarations.
  ###
  sed "/^#/d" "${tmpfile}" | sed "s/^\s\b//g" > "${outfile}"

  ##
  # remove the tmpfile
  rm -f "${tmpfile}"
}


rm -rf "210"
rm -rf "330"
mkdir -p "210"
mkdir -p "330"

for shader in "quad.frag" "quad.vert" "text.vert" "text.frag" ; do
  ##
  # build the 120 shaders
  compile_shader 120 "src/${shader}" "210/${shader}"
  ##
  # build the 330 shaders
  compile_shader 330 "src/${shader}" "330/${shader}"
done