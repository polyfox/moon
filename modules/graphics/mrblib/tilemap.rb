module Moon
  # An optimized implementation for rendering a tilemap. The VBO is cached and
  # we only execute one draw call per iteration to render the entire map 
  # (basically no overhead).
  class Tilemap
    extend TypedAttributes
    include Shadable

    # @return [Float]
    attr_reader :w

    # @return [Float]
    attr_reader :h

    # @return [Vector2] Taken from the tileset #w and #h
    attr_reader :tilesize

    # @return [Vector3] Used for denoting the size of the data and data_zmap
    attribute :datasize, Moon::Vector3
    private :datasize=

    attribute :tileset, Spritesheet

    # @return [Array<Integer>]
    attribute :data, Array
    private :data=

    # @return [Array<Float>]
    attribute :data_zmap, Array, nil
    private :data_zmap=

    # @return [Array<Float>]
    attribute :layer_opacity, Array, nil

    attribute :shader, Shader
    attribute :texture, Texture
    attribute :opacity, Float
    attribute :angle, Numeric
    attribute :origin,  Vector2
    attribute :color,  Vector4
    attribute :tone,  Vector4

    # (see #set)
    def initialize(options = {})
      @vbo             = VertexBuffer.new(VertexBuffer::DYNAMIC_DRAW)
      @tilesize        = Vector2.new(0, 0)
      @w               = 0
      @h               = 0
      @datasize        = Vector3.new(0, 0, 0)
      @data            = []
      @data_zmap       = nil
      @shader          = self.class.default_shader
      @tileset         = nil
      @layer_opacity   = nil
      @origin          = Vector2.new(0, 0)
      @angle           = 0.0
      @opacity         = 1.0
      @color           = Vector4.new(1, 1, 1, 1)
      @tone            = Vector4.new(0, 0, 0, 1)
      @transform       = Matrix4.new
      @rotation_matrix = Matrix4.new
      @mode = OpenGL::TRIANGLES
      set options unless options.empty?
    end

    private def refresh_size
      @w, @h = @datasize.x * @tilesize.x, @datasize.y * @tilesize.y
    end

    private def refresh_tileset
      @tilesize = Vector2.new(@tileset.w, @tileset.h)
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

      @vbo.clear

      # we loop by layer
      layers.times do |dz|
        # recalculate the layer opacity
        opacity = (@layer_opacity ? @layer_opacity[dz] : 1.0)
        render_state = { opacity: opacity }
        rnz = 0
        layer = dz * cols * rows
        # and then by row
        rows.times do |dy|
          rny = dy * cell_h
          row = dy * cols
          rwl = row + layer
          # and then render by column
          cols.times do |dx|
            data_index = dx + rwl
            tile_id = @data[data_index]
            # if the tile_id is -1 or less, then this tile is disabled
            # and therefore should not be rendered.
            next if tile_id < 0
            rnx = dx * cell_w
            cell_z = @data_zmap ? @data_zmap[data_index] : 0
            @tileset.copy_quad_to @vbo, rnx, rny, rnz + cell_z, tile_id, render_state
          end
        end
      end
      self
    end

    alias :set_tileset :tileset=
    private :set_tileset
    # @param [Spritesheet] tileset
    def tileset=(tileset)
      set_tileset tileset
      generate_buffers
    end

    alias :set_layer_opacity :layer_opacity=
    private :set_layer_opacity
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
      self.tone      = options.fetch(:tone,      @tone)
      self.color     = options.fetch(:color,     @color)
      self.origin    = options.fetch(:origin,    @origin)
      self.angle     = options.fetch(:angle,     @angle)
      self.opacity   = options.fetch(:opacity,   @opacity)
      self.datasize  = options.fetch(:datasize,  @datasize)
      self.data      = options.fetch(:data,      @data)
      self.data_zmap = options.fetch(:data_zmap, @data_zmap)

      self.texture = tileset.texture
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

    # Renders the tilemap on screen at the specified coordinates.
    #
    # @param [Integer] x
    # @param [Integer] y
    # @param [Integer] z
    def render(x, y, z)
      @rotation_matrix.clear
      @rotation_matrix.rotate!(@angle, [0, 0, 1])
      @rotation_matrix.translate!(-@origin.x, -@origin.y, 0)
      @transform.clear
      @transform.translate!(x, y, z)
      transform = @transform * @rotation_matrix

      @shader.use
      @shader.set_uniform 'opacity', @opacity
      @shader.set_uniform 'color', @color
      @shader.set_uniform 'tone', @tone
      Renderer.instance.render(@shader, @vbo, @texture, transform, @mode)
    end
  end
end
