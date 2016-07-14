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

    # Returns the active shader for self
    # @return [Shader]
    def shader
      @shader || self.class.default_shader
    end

    # Attaches Shadable class methods to the target module or class
    #
    # @param [Module] mod
    def self.included(mod)
      mod.extend ClassMethods
    end
  end
end
