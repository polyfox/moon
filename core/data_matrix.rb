#
# moon/core/data_matrix.rb
#   aka. 3d table
class DataMatrix

  include Enumerable

  attr_reader :xsize
  attr_reader :ysize
  attr_reader :zsize

  def initialize(xsize, ysize, zsize)
    @xsize = xsize
    @ysize = ysize
    @zsize = zsize
    @data = Array.new(@zsize) { Array.new(@ysize) { Array.new(@xsize, 0) } }
    yield self if block_given?
  end

  def [](x, y, z)
    @data[z][y][x] || 0
  end

  def []=(x, y, z, n)
    @data[z][y][x] = n
  end

  def each
    @data.each do |layer|
      layer.each do |row|
        row.each do |n|
          yield n
        end
      end
    end
  end

  def each_with_xyz
    @zsize.times do |z|
      @ysize.times do |y|
        @xsize.times do |x|
          yield @data[z][y][x], x, y, z
        end
      end
    end
  end

  def map_with_xyz
    each_with_xyz do |n, x, y, z|
      @data[z][y][x] = yield @data[z][y][x], x, y, z
    end
  end

  def fill(n)
    map_with_xyz { |old_n, x, y, z| n }
  end

  def clear
    fill(0)
  end

  def layer(z)
    layer_data = @data[z]
    tab = Table.new(0, 0)
    tab.change_data(layer_data, @xsize, @ysize)
    return tab
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