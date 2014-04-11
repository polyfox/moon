#
# moon/core/data_matrix.rb
#   aka. 3d table
class DataMatrix

  include Enumerable

  attr_reader :xsize
  attr_reader :ysize
  attr_reader :zsize
  attr_reader :data

  def initialize(xsize, ysize, zsize)
    @xsize = xsize
    @ysize = ysize
    @zsize = zsize
    create_data
    yield self if block_given?
  end

  def create_data
    @data = Array.new(@zsize) { Array.new(@ysize) { Array.new(@xsize, 0) } }
  end

  def initialize_copy(org)
    super org
    create_data
    map_with_xyz { |_, x, y, z| org.data[z][y][x] }
  end

  def [](x, y, z)
    x = x.to_i; y = y.to_i; z = z.to_i
    return 0 if ((x < 0) || (x >= @xsize)) ||
                ((y < 0) || (y >= @ysize)) ||
                ((z < 0) || (z >= @zsize))
    @data[z][y][x]
  end

  def []=(x, y, z, n)
    x = x.to_i; y = y.to_i; z = z.to_i; n = n.to_i
    return if ((x < 0) || (x >= @xsize)) ||
              ((y < 0) || (y >= @ysize)) ||
              ((z < 0) || (z >= @zsize))
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

  def clear(n=0)
    fill(n)
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

  #protected :data
  private :create_data

end