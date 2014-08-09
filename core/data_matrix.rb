# moon/core/data_matrix.rb
#   aka. 3d table
module Moon
  class DataMatrix
    include Enumerable

    attr_reader :xsize
    attr_reader :ysize
    attr_reader :zsize
    attr_reader :data
    attr_accessor :default

    def initialize(xsize, ysize, zsize, options={})
      @xsize = xsize.to_i
      @ysize = ysize.to_i
      @zsize = zsize.to_i
      @default = options.fetch(:default, 0)
      create_data
      yield self if block_given?
    end

    def create_data
      @data = Array.new(@xsize * @ysize * @zsize, @default)
      #@data = Array.new(@zsize) { Array.new(@ysize) { Array.new(@xsize, @default) } }
    end

    def initialize_copy(org)
      super org
      create_data
      #map_with_xyz { |_, x, y, z| org.data[z][y][x] }
      map_with_xyz { |_, x, y, z| org.data[x + y * @xsize + z * @xsize * @ysize] }
    end

    def size
      Vector3.new @xsize, @ysize, @zsize
    end

    def rect
      Moon::Rect.new 0, 0, @xsize, @ysize
    end

    def cuboid
      Cuboid.new 0, 0, 0, @xsize, @ysize, @zsize
    end

    def subsample(*args)
      cx, cy, cz, cw, ch, cd = *Cuboid.extract(args.size > 1 ? args : args.first)
      result = self.class.new(cw, ch, cd, default: @default)
      result.zsize.times do |z|
        dz = cz + z
        result.ysize.times do |y|
          dy = cy + y
          result.xsize.times do |x|
            result[x, y, z] = self[x + cx, dy, dz]
          end
        end
      end
      result
    end

    def [](x, y, z)
      x = x.to_i; y = y.to_i; z = z.to_i
      return @default if ((x < 0) || (x >= @xsize)) ||
                         ((y < 0) || (y >= @ysize)) ||
                         ((z < 0) || (z >= @zsize))
      @data[x + y * @xsize + z * @xsize * @ysize]
      #@data[z][y][x]
    end

    def []=(x, y, z, n)
      x = x.to_i; y = y.to_i; z = z.to_i; n = n.to_i
      return if ((x < 0) || (x >= @xsize)) ||
                ((y < 0) || (y >= @ysize)) ||
                ((z < 0) || (z >= @zsize))
      @data[x + y * @xsize + z * @xsize * @ysize] = n
      #@data[z][y][x] = n
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
            #yield @data[z][y][x], x, y, z
            yield @data[x + y * @xsize + z * @xsize * @ysize], x, y, z
          end
        end
      end
    end

    def map_with_xyz
      each_with_xyz do |n, x, y, z|
        index = x + y * @xsize + z * @xsize * @ysize
        #@data[z][y][x] = yield n, x, y, z
        @data[index] = yield n, x, y, z
      end
    end

    def fill(n)
      map_with_xyz { |old_n, x, y, z| n }
    end

    def clear(n=0)
      fill(n)
    end

    def pillar_a(x, y)
      @zsize.times.map { |z| self[x, y, z] }
    end

    def layer(z)
      layer_data = @data[z * @xsize * @ysize, @xsize * @ysize]
      table = Table.new(0, 0)
      table.change_data(layer_data, @xsize, @ysize)
      table
    end

    def resize(xsize, ysize, zsize)
      oxsize, oysize, ozsize = *size
      @xsize, @ysize, @zsize = xsize, ysize, zsize
      old_data = @data
      create_data
      map_with_xyz do |n, x, y, z|
        if x < oxsize && y < oysize && z < ozsize
          old_data[x + y * oxsize + z * oxsize * oysize]
        else
          @default
        end
      end
    end

    def to_s
      result = ""
      #@zsize.times do |z|
      #  @ysize.times do |y|
      #    result.concat(@data[z][y].join(", "))
      #    result.concat("\n")
      #  end
      #  result.concat("\n")
      #end
      @zsize.times do |z|
        @ysize.times do |y|
          result.concat(@data[y * @xsize + z * @xsize * @ysize, @xsize].join(", "))
          result.concat("\n")
        end
        result.concat("\n")
      end
      return result
    end

    def to_h
      {
        xsize: @xsize,
        ysize: @ysize,
        zsize: @zsize,
        default: @default,
        data: @data
      }
    end

    def export
      to_h.merge("&class" => self.class.to_s).stringify_keys
    end

    def import(data)
      @default = data["default"]
      @xsize = data["xsize"]
      @ysize = data["ysize"]
      @zsize = data["zsize"]
      @data = data["data"]
      self
    end

    def self.load(data)
      instance = new data["xsize"], data["ysize"], data["zsize"],
                     default: data["default"]
      instance.import data
      instance
    end

    #protected :data
    private :create_data

  end
end
