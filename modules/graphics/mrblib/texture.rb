module Moon
  class Texture
    def draw
      ctx = TextureContext.new
      yield ctx
    ensure
      ctx.destroy if ctx
    end
  end
end
