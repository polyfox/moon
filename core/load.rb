# require is broken, it will fail after closing the file
require './core/state.rb'
require './core/mouse.rb'
require './core/state_test.rb'
require './core/state_moon_player.rb'

State.push(State_Test)
