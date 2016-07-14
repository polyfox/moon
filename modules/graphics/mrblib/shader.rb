module Moon
  # Shader is a ruby representation of OpenGL/GLSL shaders. All renderable
  # objects use a shader (and publicly expose it), so you can go ahead and
  # implement your own shader derivatives for custom visual effects.
  #
  # @todo Document the attribute layout on the shaders
  class Shader
    # Loads a shader from the given filenames
    #
    # @param [String] vertex_filename
    # @param [String] fragment_filename
    # @return [Shader]
    def self.load(vertex_filename, fragment_filename)
      vertex_content = File.read(vertex_filename)
      fragment_content = File.read(fragment_filename)
      new vertex_content, fragment_content
    end
  end
end
