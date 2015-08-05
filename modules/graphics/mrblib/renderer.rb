module Moon
  class Renderer
    def render(shader, vbo, texture, transform, mode)
      apply_transform shader, transform
      bind_texture shader, texture
      vbo.render mode
    end
  end
end
