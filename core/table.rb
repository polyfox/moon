#
# moon/core/table.rb
#
class Table

  include Enumerable

  def initialize(xsize, ysize)
    @xsize = xsize
    @ysize = ysize
    @data = Array.new(@ysize) { Array.new(@xsize, 0) }
  end

  def [](x, y)
    @data[y][x] || 0
  end

  def []=(x, y, n)
    @data[y][x] = n
  end

  def each
    for y in 0...@ysize
      for x in 0...@xsize
        yield(@data[y][x], x, y) # may change this to just yield(data)
      end
    end
  end

  def map!
    each do |n, x, y|
      @data[y][x] = yield(n, x, y)
    end
  end

  def fill(n)
    map! { |old_n, x, y| n }
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