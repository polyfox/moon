#
# core/load.rb
#   Entry script for Moon's mrb
require './core/state.rb'
require './core/mouse.rb'
require './core/music_activity_hud.rb'
require './core/state_key_hold_test.rb'
require './core/state_modifier_test.rb'
require './core/state_moon_player.rb'
require './core/state_music_activity.rb'
require './core/state_test.rb'

#State.push(State_KeyHoldTest)
State.push(State_ModifierTest)
#State.push(State_MoonPlayer)
#State.push(State_MusicActivity)
#State.push(State_Test)
