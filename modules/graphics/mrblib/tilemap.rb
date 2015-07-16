module Moon
  # This may need to be rewritten in C/++
  class Tilemap
    attr_reader :w
    attr_reader :h
    # @return [Moon::Spritesheet]
    attr_accessor :tileset
    # @return [Moon::DataMatrix]
    attr_accessor :data
    # @return [Moon::DataMatrix]
    attr_accessor :data_zmap
    # @return [Array<Float>]
    attr_accessor :layer_opacity
    # @return [Boolean]
    attr_accessor :repeat_map
    # restricts rendering inside view
    # @return [Moon::Rect]
    attr_accessor :view
    # selects a section of the map_data to render
    # @return [Moon::Cuboid]
    attr_accessor :selection
    # @return [Vector2]
    attr_reader :tilesize
    # @return [Vector2]
    attr_reader :datasize

    ##
    #
    private def initialize
      @vbo = VertexBuffer.new
      @tileset       = nil
      @data          = nil
      @data_zmap     = nil
      @layer_opacity = nil
      @repeat_map    = false
      @view          = nil
      @selection     = nil
      @tilesize      = Vector2.new(0, 0)
      @datasize      = Vector2.new(0, 0)
    end

    def refresh_size
      @w, @h = @datasize.x * @tilesize.x, @datasize.y * @tilesize.y
    end

    def refresh_data
      if @data
        @datasize = Vector2.new(@data.xsize, @data.ysize)
      else
        @datasize = Vector2.new
      end
      refresh_size
    end

    def refresh_tileset
      if @tileset
        @tilesize = Vector2.new(@tileset.w, @tileset.h)
      else
        @tilesize = Vector2.new
      end
      refresh_size
    end

    # @param [Moon::DataMatrix] data
    def data=(data)
      @data = data
      refresh_data
    end

    # @param [Moon::Spritesheet] tileset
    def tileset=(tileset)
      @tileset = tileset
      refresh_tileset
    end

    ##
    # @param [Integer] x
    # @param [Integer] y
    # @param [Integer] z
    # @param [Hash<Symbol, Object>] options
    private def render_content(x, y, z, options)
      return unless @data
      return unless @tileset

      cell_w = @tilesize.x
      cell_h = @tilesize.y

      dox = 0
      doy = 0
      doz = 0
      cols = @data.xsize
      rows = @data.ysize
      layers = @data.zsize

      vx  = nil
      vx2 = nil
      vy  = nil
      vy2 = nil

      dox, doy, doz, cols, rows, layers = *@selection if @selection

      if @view
        vx = @view.x
        vx2 = @view.x2
        vy = @view.y
        vy2 = @view.y2
      end

      # we loop by layer
      layers.times do |l|

        dz = l + doz # offset data z index
        if @repeat_map
          dz %= @data.zsize
        else
          next if dz < 0 || dz >= @data.zsize
        end

        opacity = @layer_opacity ? @layer_opacity[dz] : 1.0
        opacity *= options.fetch(:opacity, 1.0)
        render_ops = options.merge(opacity: opacity)

        rnz = z

        # and then by row
        rows.times do |i|

          dy = i + doy # offset data y index
          if @repeat_map
            dy %= @data.ysize
          else
            next if dy < 0 || dy >= @data.ysize
          end

          rny = y + i * cell_h
          next if rny < vy || rny > vy2 if vy && vy2

          # and then render by cell
          cols.times do |j|

            dx = j + dox # offset data x index
            if @repeat_map
              dx %= @data.xsize
            else
              next if dx < 0 || dx >= @data.xsize
            end

            tile_id = @data[dx, dy, dz]
            # if -1 or less, then its a negative tile
            # and therefore should not be rendered
            next if tile_id < 0

            rnx = x + j * cell_w
            next if rnx < vx || rnx > vx2 if vx && vx2

            zm = @data_zmap ? @data_zmap[dx, dy, dz] : 0
            @tileset.render rnx, rny, rnz + zm, tile_id, render_ops
          end
        end
      end
    end
  end
end
