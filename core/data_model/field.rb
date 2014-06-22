module Moon
  module DataModel
    class Field

      attr_reader :type
      attr_reader :default
      attr_reader :allow_nil

      def initialize(options)
        @type = options.fetch(:type)
        @default = options[:default]
        @allow_nil = !!options[:allow_nil]
      end

      def make_default(selfie)
        @default.is_a?(Proc) ? @default.call(@type, selfie) : @default
      end

      def check_type(key, value)
        # validate that obj is an Array of type
        if @type.is_a?(Array)
          unless value.is_a?(Array) || (allow_nil && (value == nil))
            raise TypeError,
                  "[#{key}] wrong type #{value.class.inspect} (expected Array)"
          end
          unless value.all? { |o| @type.any? { |t| o.is_a?(t) } }
            str = @type.map { |s| s.inspect }.join(", ")
            raise TypeError,
                  "[#{key}[]] wrong type #{value.class.inspect} (expected #{str})"
          end
        # validate that value is a Hash of key type and value type
        elsif @type.is_a?(Hash)
          unless value.is_a?(Hash) || (allow_nil && (value == nil))
            raise TypeError,
                  "[#{key}] wrong type #{value.class.inspect} (expected Hash)"
          end

        # validate that value is of type
        elsif @type.is_a?(Module)
          unless value.is_a?(type) || (allow_nil && (value == nil))
            raise TypeError,
                  "[#{key}] wrong type #{value.class.inspect} (expected #{@type.inspect})"
          end
        end
      end

      alias :allow_nil? :allow_nil
    end
  end
end
