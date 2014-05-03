module Moon
  class Texture

    def draw
      tex = self.dup
      cxt = Moon::Texture::Context.new(tex)
      yield cxt
      cxt.destroy!
      return tex
    end

  end
end