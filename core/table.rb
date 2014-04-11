#
# moon/core/table.rb
#
class Table

  include Enumerable

  attr_reader :xsize
  attr_reader :ysize

  def initialize(xsize, ysize)
    @xsize = xsize
    @ysize = ysize
    create_data
    yield self if block_given?
  end

  def create_data
    @data = Array.new(@ysize) { Array.new(@xsize, 0) }
  end

  def initialize_copy(org)
    super org
    create_data
    map_with_xy { |_, x, y| org.data[y][x] }
  end

  def change_data(data_p, xsize, ysize)
    @xsize = xsize
    @ysize = ysize
    @data  = data_p
  end

  def size
    Vector2.new @xsize, @ysize
  end

  def [](x, y)
    x = x.to_i; y = y.to_i
    return 0 if (x < 0 || x >= @xsize) ||
                (y < 0 || y >= @ysize)
    @data[y][x]
  end

  def []=(x, y, n)
    x = x.to_i; y = y.to_i; n = n.to_i
    return if (x < 0 || x >= @xsize) ||
              (y < 0 || y >= @zsize)
    @data[y][x] = n
  end

  ###
  # Because sometimes its too damn troublesom to convert an index to the
  # proper coords
  ###
  def set_by_index(i, value)
    self[i % xsize, i / xsize] = value
  end

  def each
    @data.each do |row|
      row.each do |n|
        yield n
      end
    end
  end

  def each_with_xy
    @ysize.times do |y|
      @xsize.times do |x|
        @data[y][x] = yield @data[y][x], x, y
      end
    end
  end

  def map_with_xy
    each_with_xy do |n, x, y|
      @data[y][x] = yield @data[y][x], x, y
    end
  end

  def fill(n)
    map_with_xy { |old_n, x, y| n }
  end

  def clear(n=0)
    fill(n)
  end

  def row(y)
    @data[y]
  end

  def row_count
    @data.size
  end

  def to_s
    result = ""
    for y in 0...@ysize
      result.concat(@data[y].join(", "))
      result.concat("\n")
    end
    return result
  end

end