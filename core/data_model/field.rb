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

      def check_array_type(key, value)
        unless value.is_a?(Array)
          raise TypeError,
                "[#{key}] wrong type #{value.class.inspect} (expected Array)"
        end
        value.each_with_index do |obj, i|
          unless @type.any? { |t| obj.is_a?(t) }
            str = @type.map { |s| s.inspect }.join(", ")
            raise TypeError,
                  "[#{key}[#{i}]] wrong content type #{obj.class.inspect} (expected #{str})"
          end
        end
      end

      def check_hash_type(key, value)
        unless value.is_a?(Hash)
          raise TypeError,
                "[#{key}] wrong type #{value.class.inspect} (expected Hash)"
        end
      end

      def check_normal_type(key, value)
        unless value.is_a?(type)
          raise TypeError,
                "[#{key}] wrong type #{value.class.inspect} (expected #{@type.inspect})"
        end
      end

      def check_type(key, value)
        if allow_nil && value.nil?
          return
        elsif !allow_nil && value.nil?
          raise TypeError, ":#{key} shall not be nil"
        end
        # validate that obj is an Array and contains correct types
        if @type.is_a?(Array)
          check_array_type(key, value)
        # validate that value is a Hash of key type and value type
        elsif @type.is_a?(Hash)
          check_hash_type(key, value)
        # validate that value is of type
        elsif @type.is_a?(Module)
          check_normal_type(key, value)
        end
      end

      alias :allow_nil? :allow_nil
    end
  end
end
