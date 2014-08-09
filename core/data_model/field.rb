module Moon
  module DataModel
    class Field
      @@validator = nil

      attr_reader :type
      attr_reader :default
      attr_reader :allow_nil
      alias :allow_nil? :allow_nil

      def initialize(options)
        @type = options.fetch(:type)
        @default = options[:default]
        @allow_nil = !!options[:allow_nil]
      end

      def type_class(type=@type)
        if type.is_a?(Hash)
          Hash
        elsif type.is_a?(Array)
          Array
        else
          type
        end
      end

      def make_default(selfie=nil)
        @default.is_a?(Proc) ? @default.call(@type, selfie) : @default
      end

      def check_type(key, value, quiet=false)
        @@validator.check_type(@type, key, value, quiet: quiet, allow_nil: @allow_nil)
      end

      def self.validator
        @@validator
      end

      def self.validator=(validator)
        @@validator = validator
      end
    end
  end
end
