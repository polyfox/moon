#
# moon/core/states/moon_player.rb
#   just a simple draft of a Music player in Moon
#
# extension
class Input
  module Mouse

    def self.in_rect?(rect)
      in_area?(rect.x, rect.y, rect.width, rect.height)
    end

  end
end

# Maybe we could prefer: 
#   Music.new(filename, filetype)
#   to reduce the number of extra classes floating around.
#   Music::play would still be maintained though
class BGM

  @@last = nil
  @@last_pos = 0

  def initialize(filename, filetype)
    @filename = filename
    @filetype = filetype
    @pos = 0
  end

  def filename
    return @filename
  end

  def play(pos=0)
    unless @filename.empty?
      @pos = pos
      Music.play(@filename, @filetype) 
      Music.seek(@pos)
      @@last = self
      return true
    end
    return false
  end

  def resume
    play(@pos)
  end

  def pos
    return @pos
  end

  def stop
    BGM.stop
    @pos = 0
  end

  def pause
    BGM.pause
    @pos = @@last_pos
  end

  def self.playing?
    Music.playing?
  end

  def self.stopped?
    Music.stopped?
  end

  def self.finished?
    Music.finished?
  end

  def self.stop
    Music.stop
    @@last_pos = 0
  end

  def self.pause
    Music.stop
    @@last_pos = Music.pos
  end

  def self.resume
    @@last.play(@@last_pos) if @@last
  end

end

# small data requirement
# eventually this will be implemented in C++ 
class Rectangle
  attr_accessor :x, :y, :width, :height

  def initialize(x, y, width, height)
    @x      = x
    @y      = y
    @width  = width
    @height = height
  end
end

# containers
class Container < Rectangle

  def initialize(x, y, width, height)
    on_resize
    on_move
  end

  # callbacks
  def on_move
  end

  def on_resize
  end

  def x=(new_x)
    super(new_x)
    on_move
  end

  def y=(new_y)
    super(new_y)
    on_move
  end

  def width=(new_width)
    super(new_width)
    on_resize
  end

  def height=(new_height)
    super(new_height)
    on_resize
  end

  def pos
    return [x, y]
  end

  def size
    return [width, height]
  end

end

#
# the actual player container
class MoonPlayer < Container

  class Widget < Container

    attr_accessor :rx, :ry

    def initialize(rx, ry, width, height)
      super(0, 0, width, height)
      self.rx = rx
      self.ry = ry
    end

    def on_event(event, &func)
      @events ||= {}
      @events[event] = func
    end

    def event_list
      return @events.keys
    end

    def event?(symbol)
      return !!@events[symbol]
    end

    def call_event(symbol)
      @events[symbol].call(self) if event?(symbol)
    end

    def update
      if Input::Mouse.in_rect?(self)
        call_event(:mouse_over)
        keys = Input::Keys
        button = Input::Mouse::Buttons::LEFT
        if Input::Mouse.pressed?(button)
          if    Input::Mouse.modded?(button, keys::MOD_SHIFT)
            call_event(:mouse_focus_shift) 
          elsif Input::Mouse.modded?(button, keys::MOD_CONTROL)
            call_event(:mouse_focus_control) 
          elsif Input::Mouse.modded?(button, keys::MOD_ALT)
            call_event(:mouse_focus_alt) 
          elsif Input::Mouse.modded?(button, keys::MOD_SUPER)
            call_event(:mouse_focus_super) 
          else
            call_event(:mouse_focus_no_mod)
          end
          call_event(:mouse_focus)
        end
      end
    end

    def refresh_position(x, y)
      self.x = x + rx
      self.y = y + ry
    end

  end

  def initialize(x, y)
    # just for now, just for now
    @music = BGM.new("resources/CamelsNommingHay.ogg", "ogg")
    #@state = :stop

    @play    = Widget.new(  0,   0,  32,  32)
    @stop    = Widget.new( 34,   0,  32,  32)
    @seekbar = Widget.new( 66,  16, 128,  16)
    @seek    = Widget.new( 66,  16,  16,  16)

    @play.on_event(:mouse_focus_no_mod) do |widget|
      play_music(true)
    end

    @play.on_event(:mouse_focus_control) do |widget|
      puts "With CONTROL"
      play_music(false)
    end

    @play.on_event(:mouse_focus_alt) do |widget|
      puts "With ALT"
    end

    @play.on_event(:mouse_focus_shift) do |widget|
      puts "With SHIFT"
      pause_music
    end

    @play.on_event(:mouse_focus_super) do |widget|
      puts "With SUPER"
    end

    #@stop.on_event(:mouse_over) {} 

    @stop.on_event(:mouse_focus) do |widget|
      stop_music
    end

    super(x, y, 96, 32)
    init_spriteset
  end

  def stop_music
    puts ">> Stopping Music"
    @music.stop
  end

  def pause_music
    @music.pause
    puts ">> Paused: #{@music.filename}"
  end

  def play_music(resume=false)
    if resume 
      if @music.resume 
        puts ">> Now Resuming: #{@music.filename}"
      end
    else 
      if @music.play
        puts ">> Now Playing: #{@music.filename}"
      end
    end
  end

  def init_spriteset
    @spritesheet_16x16 = Spritesheet.new("resources/media_buttons_16x16.png", 16, 16)
    @spritesheet_32x32 = Spritesheet.new("resources/media_buttons_32x32.png", 32, 32)
    @spritesheet_128x16 = Spritesheet.new("resources/media_buttons_128x16.png", 128, 16)
  end

  def update_seek
    l = Music.length.to_f
    l = 1.0 if l < 1 
    sx = (@seekbar.width - @seek.width) * (Music.pos / l)
    @seek.refresh_position(x + sx, y)
  end

  def render
    @spritesheet_32x32.render(@play.x, @play.y, Music.playing? ? 5 : 1)
    @spritesheet_32x32.render(@stop.x, @stop.y, Music.stopped? ? 4 : 0)
    @spritesheet_128x16.render(@seekbar.x, @seekbar.y, Music.finished? ? 9 : 8)
    @spritesheet_16x16.render(@seek.x, @seek.y, Music.playing? ? 10 : 11)
  end

  def update
    # is the mouse inside the music player
    #   just a anti-lag thing: avoids useless logic
    if Input::Mouse.in_rect?(self)
      @play.update
      @stop.update
    end
    if Input.pressed?(Input::Keys::ESCAPE)
      stop_music
    elsif Input.pressed?(Input::Keys::SPACE)
      play_music
    end
    update_seek
  end

  def on_move
    @play.refresh_position(x, y)
    @stop.refresh_position(x, y)
    @seekbar.refresh_position(x, y)
    update_seek
  end

end

#
# and finally the state
class State_MoonPlayer < State

  # TODO:
  #  optional:
  #   Sample at pos (can be used to simulate meters and such)
  def init   
    @music_hud = MusicActivityHud.new(0, 0)
    @player = MoonPlayer.new(0, 0)
    @player.x = 96
    @player.y = 128
    super
  end

  def render
    super
    @music_hud.render
    @player.render
  end

  def update
    @player.update
    @ticks += 1
    #puts Input::Mouse.pos if @ticks % 5 == 0
    super
  end

end