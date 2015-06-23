module Moon
  module Shadable
    module ClassMethods
      attr_accessor :default_shader
    end

    def shader
      @shader || self.class.default_shader
    end

    def self.included(mod)
      mod.extend ClassMethods
    end
  end
end
