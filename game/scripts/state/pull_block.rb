#
# moon/core/state/pull_block
#   A block chasing its tail, what more could you want
class State_PullBlock < State

  def init
    super
    @block_node = Block_Link.new(0, 0)
    6.times do |i|
      block = Block_Link.new(0, 0)#@block_node.x - (i + 1), @block_node.y)
      @block_node.stack_push(block)
    end
    ary = @block_node.stack
    #@movement = []
    @movement = [6, 6, 2, 2, 4, 4, 8, 8]
    @time = 15
    [6, 6, 2, 2, 4, 4, 8, 8, 6].each do |dir|
      @block_node.move_straight(dir)
    end
    puts "There are #{@block_node.stack_size} blocks in the stack"
    init_spriteset
  end

  def init_spriteset
    @spritesheet = Spritesheet.new("resources/media_buttons_32x32.png", 32, 32)
  end

  def render
    super
    last_i = @block_node.stack_size - 1
    @block_node.each_with_index do |block, i|
      x, y = block.x, block.y
      if i == 0
        sp_i = 0
      elsif i == last_i
        sp_i = 1
      else
        sp_i = 2
      end
      @spritesheet.render(x * @spritesheet.cell_width,
                          y * @spritesheet.cell_height, 0, sp_i)
    end
  end

  def update
    super
    unless @movement.empty?
      if @ticks % @time == 0
        dir = @movement[(@ticks / @time) % @movement.size]
        @block_node.move_straight(dir)
      end
    end
    if Input::Keyboard.pressed?(Input::Keyboard::Keys::DOWN)
      @block_node.move_straight(2)
    elsif Input::Keyboard.pressed?(Input::Keyboard::Keys::LEFT)
      @block_node.move_straight(4)
    elsif Input::Keyboard.pressed?(Input::Keyboard::Keys::RIGHT)
      @block_node.move_straight(6)
    elsif Input::Keyboard.pressed?(Input::Keyboard::Keys::UP)
      @block_node.move_straight(8)
    end
  end

end