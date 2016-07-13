#!/bin/bash
if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  brew update || exit
  brew upgrade libyaml cmake
  brew install freetype glfw3 glew
  brew link freetype
fi

#gem update --system
#gem install bundler
#bundle install
