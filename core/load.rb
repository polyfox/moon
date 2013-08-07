#
# core/load.rb
#   Entry script for Moon's mrb
require 'core/vector2'
require 'core/rectangle'
require 'core/table'       # 2d Table
require 'core/data_matrix' # 3d Table
require 'core/tilemap'
require 'core/tone'
require 'core/mouse'
require 'core/gui'
require 'core/music_activity_hud'
require 'core/state'

#State.push(State_KeyHoldTest)   #
#State.push(State_ModifierTest)  #
#State.push(State_MoonPlayer)    #
#State.push(State_MusicActivity) #
#State.push(State_PullBlock)     # a small demonstration of the Block_Link
#State.push(State_Snake)         # a small game of Snake
 State.push(State_TilemapTest)   # demonstrates basic Tilemap usage
#State.push(State_Test)          # common testing state
