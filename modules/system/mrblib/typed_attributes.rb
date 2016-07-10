module Moon
  # Extension for defining attributes of a specific type (class)
  # @example
  #   class MyClass
  #     # color only accepts Vector4s
  #     attribute :color, Vector4
  #     # l is able to accept either an A or a B
  #     attribute :l, A, B
  #   end
  module TypedAttributes
    # Defines a type checked attribute.
    #
    # @param [Symbol] name
    # @param [Array<Class>] klasses
    # @return [Symbol] attribute name
    # @!macro [attach] attribute
    #   @!attribute $1
    #     @return [$2] Returns the value of attribute $1.
    def attribute(name, *klasses)
      attr_reader name
      varname = "@#{name}"
      define_method((name.to_s + "=").to_sym) do |value|
        unless klasses.any? { |klass| klass === value }
          raise TypeError,
            "wrong argument type #{value.class} for #{name} attribute (expected #{klasses.join(" or ")})"
        end
        instance_variable_set(varname, value)
      end
      name.to_sym
    end
  end
end
