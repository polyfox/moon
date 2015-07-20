module Moon
  class Tilemap
    include Shadable

    # @!attribute [r] :w
    #   @return [Float]
    attr_reader :w

    # @!attribute [r] :h
    #   @return [Float]
    attr_reader :h

    # @!attribute :tilesize
    #   @return [Vector2] Taken from the tileset #w and #h
    attr_reader :tilesize

    # @!attribute :datasize
    #   @return [Vector3] Used for denoting the size of the data and data_zmap
    attribute :datasize, Moon::Vector3
    private :datasize=

    # @!attribute :tileset
    #   @return [Spritesheet]
    attribute :tileset, Spritesheet

    # @!attribute [r] :data
    #   @return [Array<Integer>]
    attribute :data, Array
    private :data=

    # @!attribute :data_zmap
    #   @return [Array<Float>]
    attribute :data_zmap, Array, nil
    private :data_zmap=

    # @!attribute :layer_opacity
    #   @return [Array<Float>]
    attribute :layer_opacity, Array, nil

    # @!attribute :shader
    #   @return [Shader]
    attribute :shader, Shader

    # (see #set)
    def initialize(options = {})
      @vbo = VertexBuffer.new
      @tilesize      = Vector2.new(0, 0)
      @w             = 0
      @h             = 0
      @datasize      = Vector3.new(0, 0, 0)
      @data          = []
      @data_zmap     = nil
      @shader        = self.class.default_shader
      @tileset       = nil
      @layer_opacity = nil
      @origin        = Vector2.new(0, 0)
      @angle         = 0
      @opacity       = 1.0
      set options unless options.empty?
    end

    private def refresh_size
      @w, @h = @datasize.x * @tilesize.x, @datasize.y * @tilesize.y
    end

    private def refresh_tileset
      if @tileset
        @tilesize = Vector2.new(@tileset.w, @tileset.h)
      else
        @tilesize = Vector2.new
      end
      refresh_size
    end

    private def generate_buffers
      refresh_tileset
      refresh_size
      cell_w = @tilesize.x.to_i
      cell_h = @tilesize.y.to_i
      cols = @datasize.x.to_i
      rows = @datasize.y.to_i
      layers = @datasize.z.to_i

      # we loop by layer
      layers.times do |dz|
        # recalculate the layer opacity
        opacity = (@layer_opacity ? @layer_opacity[dz] : 1.0)
        render_state = { opacity: opacity }
        rnz = z
        layer = dz * @cols * @rows
        # and then by row
        rows.times do |dy|
          rny = y + dy * cell_h
          row = dy * @cols
          rwl = row + layer
          # and then render by column
          cols.times do |dx|
            data_index = dx + rwl
            tile_id = @data[data_index]
            # if the tile_id is -1 or less, then this tile is disabled
            # and therefore should not be rendered.
            next if tile_id < 0
            rnx = x + j * cell_w
            cell_z = @data_zmap ? @data_zmap[data_index] : 0
            @tileset.push_into @vbo, rnx, rny, rnz + cell_z, tile_id, render_state
          end
        end
      end
    end

    alias :set_tileset :tileset=
    # @param [Spritesheet] tileset
    def tileset=(tileset)
      set_tileset tileset
      generate_buffers
    end

    alias :set_layer_opacity :layer_opacity=
    # @param [Array<Float>] layer_opacity
    def layer_opacity=(layer_opacity)
      set_layer_opacity layer_opacity
      generate_buffers
    end

    # @param [Hash<Symbol>] options
    # @option options [Spritesheet] :tileset
    # @option options [Array<Float>] :layer_opacity
    # @option options [Shader] :shader
    # @option options [Vector2] :origin
    # @option options [Float] :angle
    # @option options [Float] :opacity
    # @option options [Vector3] :datasize
    # @option options [Array<Integer>] :data
    # @option options [Array<Float>] :data_zmap
    # @return [self]
    def set(options)
      # set attributes
      set_tileset options.fetch(:tileset, @tileset)
      set_layer_opacity options.fetch(:layer_opacity, @layer_opacity)
      self.shader    = options.fetch(:shader,        @shader)
      self.origin    = options.fetch(:origin,    @origin)
      self.angle     = options.fetch(:angle,     @angle)
      self.opacity   = options.fetch(:opacity,   @opacity)
      self.datasize  = options.fetch(:datasize,  @datasize)
      self.data      = options.fetch(:data,      @data)
      self.data_zmap = options.fetch(:data_zmap, @data_zmap)

      # check data lengths
      @datalength = datasize.x.to_i * datasize.y.to_i * datasize.z.to_i
      if @datalength != @data.size
        raise SizeError, "data size mismatch."
      end
      if @data_zmap && @datalength != @data_zmap.size
        raise SizeError, "data_zmap size mismatch."
      end

      # regenerate buffers
      generate_buffers
    end
  end
end
