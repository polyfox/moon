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

  echo "Compiling $infile > $outfile"
  $PP -I. -nostdinc -E -P -xc \
      -D__GLSL_VERSION__=$glslv \
      -DGL_ES=$build_for_gl_es \
      -include common.h "${infile}" -o "${tmpfile}"

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
  #sed "/^#\s/d" "${tmpfile}" | sed "s/^\s\b//g" > "${outfile}"
  sed "s/^\s\b//g" "${tmpfile}" > "${outfile}"
  #cp $tmpfile $outfile

  ##
  # remove the tmpfile
  rm -f "${tmpfile}"
}

build_for_gl_es=0
for glsl in 120 330 ; do
  rm -rf "${glsl}" && mkdir -p "${glsl}"
  for shader in "quad.frag" "quad.vert" "text.vert" "text.frag" ; do
    compile_shader $glsl "src/${shader}" "${glsl}/${shader}"
  done
done

## ES Shaders
#build_for_gl_es=1
#for glsl in 100 200 ; do
#  rm -rf "es${glsl}" && mkdir -p "es${glsl}"
#  for shader in "quad.frag" "quad.vert" "text.vert" "text.frag" ; do
#    compile_shader $glsl "src/${shader}" "es${glsl}/${shader}"
#  done
#done
