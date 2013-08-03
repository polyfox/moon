#
# moon/core/state/snake
#   
class Snake < Block

  class Body < Snake
  end

  def grow
    t = tail
    stack_push(Body.new(t.x, t.y))
  end

  alias :tail :stack_bottom
  alias :length :stack_size

end

class Glob < Vector2

  attr_accessor :is_main
  attr_accessor :points

  def initialize(*args)
    super(*args)
    @points = 0
  end

  def pos?(x, y)
    self.x == x && self.y == y
  end

end

class State_Snake < State

  def init
    super
    @width  = (Window.width / 16).to_i # Window#width / 16
    @height = (Window.height / 16).to_i # Window#height / 16
    puts "Board Size is: #{@width} x #{@height}"
    init_snake
    init_spriteset
    @cell_width = @spritesheet.cell_width
    @cell_height = @spritesheet.cell_height
    setup
  end

  def init_snake
    @snake = Snake.new(0, 0)
  end

  def init_spriteset
    @spritesheet = Spritesheet.new("resources/media_buttons_16x16.png", 16, 16)
  end 

  def setup
    @dir = 6
    @time = 15
    @points = 0
    @globs = []
    add_main_glob # add the very first main glob
  end

  def available_pos
    return rand(@width), rand(@height)
  end

  def unoccupied_pos?(x, y)
    for obj in (@globs + @snake.stack)
      return false if obj.pos?(x, y)
    end
    return true
  end

  def add_glob
    while pos = available_pos
      break if unoccupied_pos?(*pos)
    end
    glob = Glob.new(*pos)
    @globs.push(glob)
    return glob
  end

  def add_main_glob
    add_glob.is_main = true
  end

  def new_bonus_glob
    add_glob.is_main = false
  end

  def render
    super
    last_i = @snake.length - 1
    for glob in @globs
      @spritesheet.render(glob.x * @cell_width, glob.y * @cell_height, 0, 4)
    end
    @snake.each_with_index do |body, i|
      x, y = body.x, body.y
      if i == 0
        sp_i = 0
      elsif i == last_i
        sp_i = 1
      else
        sp_i = 2
      end
      @spritesheet.render(x * @cell_width, y * @cell_height, 1, sp_i)
    end
  end

  def update
    super
    if Input.pressed?(Input::Keys::DOWN)
      @dir = 2
    elsif Input.pressed?(Input::Keys::LEFT)
      @dir = 4
    elsif Input.pressed?(Input::Keys::RIGHT)
      @dir = 6
    elsif Input.pressed?(Input::Keys::UP)
      @dir = 8
    end
    if @ticks % @time == 0
      @snake.move_straight(@dir)
      @snake.x %= @width 
      @snake.y %= @height
      if solve_collision
        check_globs
        @points += 1
      end
    end
  end

  def gameover
    puts "Gameover"
    puts "Your Score: #{@points}"
    State.pop
  end

  def solve_collision
    for body in @snake.stack_children
      if @snake.pos?(body.x, body.y)
        gameover
        return false
      end
    end
    return true
  end

  def check_globs
    removed_globs = []
    for glob in @globs
      if glob.pos?(@snake.x, @snake.y)
        @points += glob.points
        if glob.is_main
          @snake.grow
          add_main_glob   
        end
        removed_globs.push(glob)
      end
    end
    removed_globs.each do |glob|
      @globs.delete(glob)
    end
  end

end