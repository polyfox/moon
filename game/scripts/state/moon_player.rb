#
# moon/core/states/moon_player.rb
#   just a simple draft of a Music player in Moon.
#   This State is used to demonstrate various functions in the Moon engine.
#
# extension

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

  def basename
    i = @filename.size
    while i > 0
      if @filename[i] == "/"
        i += 1
        break
      end
      i -= 1
    end
    return @filename[i, @filename.size - i]
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

#
# the actual player container
class MoonPlayer < Container

  include Moon

  def initialize(x, y)
    # just for now, just for now
    @music = BGM.new("resources/CamelsNommingHay.ogg", "ogg")
    #@state = :stop

    @play    = Widget.new(self,  0,   0,  32,  32)
    @stop    = Widget.new(self, 34,   0,  32,  32)
    @title   = Widget.new(self, 66,   0, 128,  16)
    @seekbar = Widget.new(self, 66,  16, 128,  16)
    @seek    = Widget.new(self, 66,  16,  16,  16)

    @play.on_event(:mouse_focus_no_mod) do |widget|
      Music.playing? ? pause_music : play_music(true)
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

    @widgets = [@play, @stop, @title, @seekbar, @seek]

    ## max_by lamba function
    max_by = ->(ary, &func) do
      return ary.max { |a, b| func.(a) <=> func.(b) }
    end

    ## min_by lamba function
    min_by = ->(ary, &func) do
      return ary.min { |a, b| func.(a) <=> func.(b) }
    end

    # premitive way to find the covered area of a set of Rectangles
    ## x
    wid = min_by.(@widgets) { |w| w.rx }
    x1 = wid.rx
    wid = max_by.(@widgets) { |w| w.rx2 }
    x2 = wid.rx2

    ## y
    wid = min_by.(@widgets) { |w| w.ry }
    y1 = wid.ry
    wid = max_by.(@widgets) { |w| w.ry2 }
    y2 = wid.ry2

    super(x, y, (x2 - x1).abs, (y2 - y1).abs)
    init_spriteset
    init_text
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
    @spritesheet_16x16  = Spritesheet.new("resources/media_buttons_16x16.png", 16, 16)
    @spritesheet_32x32  = Spritesheet.new("resources/media_buttons_32x32.png", 32, 32)
    @spritesheet_128x16 = Spritesheet.new("resources/media_buttons_128x16.png", 128, 16)
  end

  def init_text
    @text = Font.new("resources/fonts/ipaexg00201/ipaexg.ttf", 12)
  end

  def update_seek
    l = Music.length.to_f
    l = 1.0 if l < 1
    sx = (@seekbar.width - @seek.width) * (Music.pos / l)
    @seek.refresh_position(x + sx, y)
  end

  def render
    @spritesheet_32x32.render(@play.x, @play.y, 0, Music.playing? ? 5 : 1)
    @spritesheet_32x32.render(@stop.x, @stop.y, 0, Music.stopped? ? 4 : 0)
    @spritesheet_128x16.render(@title.x, @title.y, 0, 10)
    @spritesheet_128x16.render(@seekbar.x, @seekbar.y, 0, Music.finished? ? 9 : 8)
    @spritesheet_16x16.render(@seek.x, @seek.y, 0, Music.playing? ? 10 : 11)
    @text.draw_text(@title.x, @title.y + @text.size, @music.basename)
  end

  def update
    # is the mouse inside the music player
    #   just a anti-lag thing: avoids useless logic
    if Input::Mouse.in_rect?(self)
      @play.update
      @stop.update
    end
    if Input::Keyboard.triggered?(Input::Keyboard::Keys::ESCAPE)
      stop_music
    elsif Input::Keyboard.triggered?(Input::Keyboard::Keys::SPACE)
      play_music
    end
    update_seek
  end

  def on_move
    @widgets.each { |wid| wid.refresh_position(x, y) }
    update_seek
  end

end

#
# and finally the state
class State::MoonPlayer < State

  include Moon

  # TODO:
  #  optional:
  #   Sample at pos (can be used to simulate meters and such)
  def init
    @music_hud = MusicActivityHud.new(0, 0, 0)
    @player = MoonPlayer.new(96, 128)
    puts @player.pos

    # evil stuff don't do this
    @button_hud = State_KeyHoldTest::Hud_KeyHold.new(Input::Mouse, Input::Mouse::Buttons::LEFT, 8, 8)
    @button_hud.x = 0
    @button_hud.y = @player.y + @player.height
    super
  end

  def render
    super
    @button_hud.render
    @music_hud.render
    @player.render
  end

  def update
    @player.update
    #puts Input::Mouse.pos if @ticks % 5 == 0
    super
  end

end