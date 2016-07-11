module Moon
  # This mixin is used to add a shader attribute to objects that falls back to
  # a default shader for that class.
  module Shadable
    module ClassMethods
      # The default shader fallback for this class.
      # Will be used if object's shader is nil
      attr_accessor :default_shader
    end

    # @!attribute [r] shader
    # @return [Shader]
    def shader
      @shader || self.class.default_shader
    end

    def self.included(mod)
      mod.extend ClassMethods
    end
  end
end
