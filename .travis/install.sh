#!/bin/bash
if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  brew update || exit
  for package in libyaml freetype glfw3 cmake ; do
    brew outdated "${package}" || brew upgrade "${package}" || exit
  done

  whereis libfreetype
  brew link freetype
fi

gem update --system
gem install bundler
bundle install
