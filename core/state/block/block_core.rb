
##
# Node
#   Reactor like module, children will react to a parent Node
module Node

  attr_accessor :child

  def init_node
    @child = nil
  end

  def emit
    @child.react(self) if @child
  end

  def react(parent)
    # overwrite in subclass
  end

end
  
##
# Stackable
#   Makes an object stackable in a sense.
#   How the stacking is handled is all up to the object in question
module Stackable

  def stack_next
    nil # overwrite in subclass
  end

  def stack_next_set(obj)
    # overwrite in subclass
  end

  ##
  # stack_top
  #   if the stack supports bi-directional behaviour, please do overwrite this
  def stack_top
    self # okay maybe I'll implement this
  end

  def stack_bottom
    if obj = stack_next
      return obj.stack_bottom
    else
      return self
    end
  end

  def stack_bottom_parent
    if obj = stack_next
      if obj == obj.stack_bottom
        return self
      else
        return obj.stack_bottom_parent
      end
    else
      return self
    end
  end

  def stack_map(ary=[])
    ary.push(self)
    if obj = stack_next
      return obj.stack_map(ary)
    else
      return ary
    end
  end

  def stack
    stack_map([])
  end

  def stack_children
    if obj = stack_next
      return stack_next.stack
    else
      return []
    end
  end

  def stack_break
    obj = stack_next
    stack_next_set(nil)
    return obj
  end

  def stack_push(obj)
    stack_bottom.stack_next_set(obj)
  end

  def stack_pop(obj)
    obj = stack_bottom_parent
    res = obj.stack_bottom
    obj.stack_next_set(nil)
    return res
  end

  def stack_each(&block)
    unless block_given?
      return to_enum(:stack_each)
    end
    block.call(self)
    if obj = stack_next
      obj.stack_each(&block)
    end
  end

  def stack_size
    stack.size
  end

  def stack_factorial(n)
    if n > 0
      if obj = stack_next
        obj.stack_factorial(n - 1)
      else
        return nil
      end
    else
      return self
    end
  end

  def stack_at(index)
    if index > 0
      stack_factorial(index)
    else
      stack[index]
    end
  end

end

##
# Block
#   blocks are programmed to follow their parent block:
#     When the parent block moves, it notifies the child block.
#     that child block will take the position of parent block, the parent
#     block will then move ahead
class Block 

  include Node
  include Stackable
  include Enumerable

  attr_accessor :x, :y

  def initialize(x=0, y=0)
    @x = x
    @y = y
    init_node
  end

  def pos?(x, y)
    @x == x && @y == y
  end

  def pos
    Vector2[@x, @y]
  end

  def move_straight(dir)
    emit
    case dir
    when 2 
      @x, @y = @x, @y + 1
    when 4 
      @x, @y = @x - 1, @y
    when 6 
      @x, @y = @x + 1, @y
    when 8 
      @x, @y = @x, @y - 1
    end
  end

  def react(parent)
    emit
    @x = parent.x
    @y = parent.y
  end

  def stack_next
    @child
  end

  def stack_next_set(obj)
    @child = obj
  end

  def each(&block)
    stack_each(&block)
  end

  alias :[] :stack_at

end