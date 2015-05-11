# [![Moon](https://raw.githubusercontent.com/polyfox/moon/master/moon-logo.png)](https://raw.githubusercontent.com/polyfox/moon/master/moon-logo.png) Moon
[![Build Status](https://travis-ci.org/polyfox/moon.svg?branch=master)](https://travis-ci.org/polyfox/moon)
[![Code Climate](https://codeclimate.com/github/polyfox/moon/badges/gpa.svg)](https://codeclimate.com/github/polyfox/moon)

## Introduction
Moon is a experimental prototyping 2D Game Engine written in C++ and scripted using [mruby](https://github.com/mruby/mruby).


## Supported
Currently testing is done linux, moon may not run or even compile on other systems. If you managed to get Moon running on your system, please open a PR and share your changes to get it built.


## Requirements
While moon attempts to vendor most of its dependencies, here is a list of 
things you'll need before getting started.


### Dependencies
```
# building
premake4 
cmake >= 2.8.9
ruby >= 1.9.2

# runtime
OpenAL
OpenGL

# For dependency docs
doxygen

# for mruby-yaml
yaml
```


### Features required
In order to use Moon, a OpenGL 2.1 or greater, capable system is required,
we will make no effort to support anything less than 2.1.


## Building

```shell
# if you don't already have moon locally
git clone --recursive https://github.com/polyfox/moon.git

cd moon
# if you don't have bundler installed
gem install bundler
# make sure you have all the nice things moon needs to build
bundle update

# finally execute the build process and watch it go
bundle exec rake build
```

The build process is still clunky and your build may fail.

## Updating
You can update Moon at anytime by grabbing the latest commits from master
and updating the submodules if needs be:

```shell
git pull origin master
# in case we added new submodules, you can initialize them during the update
git submodule update --init
```


## Mruby
Moon is broken into 4 mrbgems, `system`, `graphics`, `audio`, `engine`.
`graphics`, and `audio` require `system`, while `engine` requires the other 3.

Moon's `graphics` and `audio` can be used in your own projects outside of moon.


## License
TODO


## Problems
Please report any problems by [opening an issue](https://github.com/polyfox/moon/issues/new).
