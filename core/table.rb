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
    @data = Array.new(@ysize) { Array.new(@xsize, 0) }
    yield self if block_given?
  end

  def change_data(data_p, xsize, ysize)
    @xsize = xsize
    @ysize = ysize
    @data  = data_p
  end

  def [](x, y)
    return 0 if (x < 0 || x >= @xsize) ||
                (y < 0 || y >= @xsize)
    @data[y][x]
  end

  def []=(x, y, n)
    return if (x < 0 || x >= @xsize) ||
              (y < 0 || y >= @xsize)
    @data[y][x] = n
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

  def clear
    fill(0)
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