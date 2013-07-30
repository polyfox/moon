#
# core/load.rb
#   Entry script for Moon's mrb
require 'core/state'
require 'core/mouse'
require 'core/music_activity_hud'
require 'core/state_key_hold_test'
#require 'core/state_modifier_test'
require 'core/state_moon_player'
require 'core/state_music_activity'
require 'core/state_test'

#State.push(State_KeyHoldTest)
#State.push(State_ModifierTest)
#State.push(State_MoonPlayer)
#State.push(State_MusicActivity)
State.push(State_Test)
