class LunarMetal::World

  def initialize
    @width = 20
    @height = 20
    @depth = 20
    @size = @width * @height * @depth
    @tiledata = Array.new(@size, 0)
    @metadata1 = Array.new(@size, 0)
    @metadata2 = Array.new(@size, 0)
    @metadata3 = Array.new(@size, 0)
    @metadata4 = Array.new(@size, 0)
  end

end