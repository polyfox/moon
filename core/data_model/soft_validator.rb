module Moon
  module DataModel
    module SoftValidator
      def check_array_type(type, key, value, options={})
        unless value.is_a?(Array)
          if options[:quiet]
            return false
          else
            raise TypeError,
                  "[#{key}] wrong type #{value.class.inspect} (expected Array)"
          end
        end
        true
      end

      def check_hash_type(type, key, value, options={})
        unless value.is_a?(Hash)
          if options[:quiet]
            return false
          else
            raise TypeError,
                  "[#{key}] wrong type #{value.class.inspect} (expected Hash)"
          end
        end
        true
      end

      def check_normal_type(type, key, value, options={})
        unless value.is_a?(type)
          if options[:quiet]
            return false
          else
            raise TypeError,
                  "[#{key}] wrong type #{value.class.inspect} (expected #{type.inspect})"
          end
        end
        true
      end

      def check_type(type, key, value, options={})
        if options[:allow_nil] && value.nil?
          return true
        elsif !options[:allow_nil] && value.nil?
          if options[:quiet]
            return false
          else
            raise TypeError, ":#{key} shall not be nil"
          end
        end
        # validate that obj is an Array and contains correct types
        if type.is_a?(Array)
          check_array_type(type, key, value, options)
        # validate that value is a Hash of key type and value type
        elsif type.is_a?(Hash)
          check_hash_type(type, key, value, options)
        # validate that value is of type
        elsif type.is_a?(Module)
          check_normal_type(type, key, value, options)
        else
          true
        end
      end

      private :check_hash_type
      private :check_array_type
      private :check_normal_type

      extend self
    end
  end
end
