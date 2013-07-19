#
# moon/core/states/moon_player.rb
#   just a simple draft of a Music player in Moon
#
# extension
class Input
  module Mouse

    def self.in_rect?(rect)
      x().between?(rect.x, rect.width) && y().between?(rect.y, rect.height)
    end

  end
end

# Maybe we could prefer: 
#   Music.new(filename, filetype)
#   to reduce the number of extra classes floating around.
#   Music::play would still be maintained though
class BGM

  def initialize(filename, filetype)
    @filename = filename
    @filetype = filetype
  end

  def play
    Music.play(@filename, @filetype) unless @filename.empty?
  end

end

# small data requirement
# eventually this will be implemented in C++ 
class Rectangle

  def initialize(x, y, width, height)
    @x      = x
    @y      = y
    @width  = width
    @height = height
  end

  def x
    return @x
  end

  def y
    return @y
  end

  def width
    return @width
  end

  def height
    return @height
  end

  def x=(new_x)
    @x = new_x
  end

  def y=(new_y)
    @y = new_y
  end

  def width=(new_width)
    @width = new_width
  end

  def height=(new_height)
    @height = new_height
  end

end

# containers
class Container < Rectangle

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

end

#
# the actual player container
class MoonPlayer < Container

  def initialize(x, y, width, height)
    super(x, y, width, height)
    # just for now, just for now
    @music = BGM.new("resources/CamelsNommingHay.ogg", "ogg")
    @state = :stop
  end

  def render

  end

  def update
    # is the mouse inside the music player
    #   just a anti-lag thing: avoids useless logic
    if Input::Mouse.in_rect?(self)
      if Input::Mouse.in_rect?(@play_rect)
        if Input::Mouse.press?()
          @state = @state == :play ? :pause : :play 
        end
      elsif Input::Mouse.in_rect?(@stop_rect)
      end
    end
  end

end

#
# and finally the state
class State_MoonPlayer < State

  def init
    @player = MoonPlayer.new
  end

  # suggestion:
  #   we should add a #render function to States, and keep graphic oriented
  #   code out of the logical code
  def render
    @player.render
  end

  def update
    @player.update
    render # self-explanatory?
  end

end