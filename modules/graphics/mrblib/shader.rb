module Moon
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
