###
# entry script for moon
###
require 'scripts/cache'
require 'scripts/block/block_core'
require 'scripts/state'
require 'scripts/music_activity_hud'

#State.push(State::ModifierTest)  #
#State.push(State::MoonPlayer)    #
#State.push(State::MusicActivity) #
#State.push(State::PullBlock)     # a small demonstration of the Block_Link
#State.push(State::Snake)         # a small game of Snake
#State.push(State::TilemapTest)   # demonstrates basic Tilemap usage
#State.push(State::Test)          # common testing state
#State.push(State::SpriteTest)
State.push(State::SpriteClipTest)
#State.push(State::KeyHoldTest)   #
#State.push(State::MouseEvents)
State.push(State::CoreTest)
#State.push(State::IntegrityTest)