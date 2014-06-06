# I can be shot for this afterwards...
module ScreenElement

  ###
  # Convert a screen position to a relative position in the Container
  # @overload screen_to_relative(x, y)
  # @overload screen_to_relative(vec2)
  # @return [Vector2]
  ###
  def screen_to_relative(*args)
    vec2 = Vector2[*args]
    vec2.x -= x
    vec2.y -= y
    vec2
  end

  ###
  # Convert a relative position in the Container to a screen position
  # @overload relative_to_screen(x, y)
  # @overload relative_to_screen(vec2)
  # @return [Vector2]
  ###
  def relative_to_screen(*args)
    vec2 = Vector2[*args]
    vec2.x += x
    vec2.y += y
    vec2
  end

  ###
  # Determines if position is inside the Container
  # @overload pos_inside?(x, y)
  # @overload pos_inside?(vec2)
  # @return [Boolean]
  ###
  def pos_inside?(*args)
    px, py = *Vector2.extract(args.size > 1 ? args : args.first)
    px.between?(x, x2) && py.between?(y, y2)
  end

  ###
  # Determines if position is inside the Container relatively
  # @overload pos_inside?(x, y)
  # @overload pos_inside?(vec2)
  # @return [Boolean]
  ###
  def relative_pos_inside?(*args)
    px, py = *Vector2.extract(args.size > 1 ? args : args.first)
    px.between?(0, width) && py.between?(0, height)
  end

  ###
  # @return [Rect] bounds
  ###
  def bounds
    Moon::Rect.new x, y, width, height
  end

end