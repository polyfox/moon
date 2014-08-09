#
# moon/core/table.rb
#
module Moon
  class Table
    include Enumerable

    attr_reader :xsize
    attr_reader :ysize

    attr_accessor :default

    def initialize(xsize, ysize, options={})
      @xsize = xsize.to_i
      @ysize = ysize.to_i
      @default = options.fetch(:default, 0)
      create_data
      yield self if block_given?
    end

    def create_data
      @data = Array.new(@xsize * @ysize, @default)
      #@data = Array.new(@ysize) { Array.new(@xsize, @default) }
    end

    def initialize_copy(org)
      super org
      create_data
      #map_with_xy { |_, x, y| org.data[y][x] }
      map_with_xy { |_, x, y| org.data[x + y * @xsize] }
    end

    def change_data(data_p, xsize, ysize)
      @xsize = xsize
      @ysize = ysize
      @data  = data_p
    end

    def size
      Vector2.new @xsize, @ysize
    end

    def rect
      Moon::Rect.new 0, 0, @xsize, @ysize
    end

    def cuboid
      Cuboid.new 0, 0, 0, @xsize, @ysize, 1
    end

    def subsample(*args)
      rx, ry, rw, rh = *Rect.extract(args.size > 1 ? args : args.first)
      result = self.class.new(rw, rh, default: @default)
      result.ysize.times do |y|
        dy = y + ry
        result.xsize.times do |x|
          result[x, y] = self[x + rx, dy]
        end
      end
      result
    end

    def [](x, y)
      x = x.to_i; y = y.to_i
      return @default if (x < 0 || x >= @xsize) ||
                         (y < 0 || y >= @ysize)
      @data[x + y * @xsize]
      #@data[y][x]
    end

    def []=(x, y, n)
      x = x.to_i; y = y.to_i; n = n.to_i
      return if (x < 0 || x >= @xsize) ||
                (y < 0 || y >= @ysize)
      @data[x + y * @xsize] = n
      #@data[y][x] = n
    end

    ###
    # Because sometimes its too damn troublesom to convert an index to the
    # proper coords
    ###
    def set_by_index(i, value)
      self[i % xsize, i / xsize] = value
    end

    def each
      @data.each do |x|
        yield x
      end
    end

    def each_row
      @xsize.times do |x|
        yield @data[x * @xsize, @xsize]
      end
    end

    def each_with_xy
      @ysize.times do |y|
        @xsize.times do |x|
          yield @data[x + y * @xsize], x, y
          #yield @data[y][x], x, y
        end
      end
    end

    def map_with_xy
      each_with_xy do |n, x, y|
        index = x + y * @xsize
        @data[index] = yield @data[index], x, y
        #@data[y][x] = yield @data[y][x], x, y
      end
    end

    def fill(n)
      map_with_xy { |old_n, x, y| n }
    end

    def clear(n=0)
      fill(n)
    end

    def row(y)
      @data[y * @xsize, @xsize]
      #@data[y]
    end

    def row_count
      @ysize
    end

    def resize(xsize, ysize)
      oxsize, oysize = *size
      @xsize, @ysize = xsize, ysize
      old_data = @data
      create_data
      map_with_xy do |n, x, y|
        if x < oxsize && y < oysize
          old_data[x + y * oxsize]
        else
          @default
        end
      end
    end

    def to_s
      result = ""
      #@ysize.times do |y|
      #  result.concat(@data[y].join(", "))
      #  result.concat("\n")
      #end
      @ysize.times do |y|
        result.concat(@data[y * @xsize, @xsize].join(", "))
        result.concat("\n")
      end
      return result
    end

    def to_h
      {
        xsize: @xsize,
        ysize: @ysize,
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
      @data = data["data"]
      self
    end

    def self.load(data)
      instance = new data["xsize"], data["ysize"], default: data["default"]
      instance.import data
      instance
    end

    private :create_data
  end
end
