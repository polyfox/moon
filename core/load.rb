#
# core/load.rb
#   Entry script for Moon's mrb
require 'core/vector2'
require 'core/rectangle'
require 'core/mouse'
require 'core/gui'
require 'core/music_activity_hud'
require 'core/state'

#State.push(State_KeyHoldTest)
#State.push(State_ModifierTest)
#State.push(State_MoonPlayer)
#State.push(State_MusicActivity)
#State.push(State_PullBlock)
#State.push(State_Snake)
State.push(State_Test)
