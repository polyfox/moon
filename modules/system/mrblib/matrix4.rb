module Moon
  class Matrix4
    # @!method initialize(*args)
    #   @overload initialize
    #     Initializes the matrix with all components set to 1.0f
    #   @overload initialize(num)
    #     Initializes the matrix with all components set as num
    #     @param [Numeric] num
    #   @overload initialize(mat4)
    #     Initializes the matrix from another matrix
    #     @param [Moon::Matrix4] mat4
    #   @overload initialize(vec4a, vec4b, vec4c, vec4d)
    #     Initializes the matrix from given rows
    #     @param [Moon::Vector4] vec4a
    #     @param [Moon::Vector4] vec4b
    #     @param [Moon::Vector4] vec4c
    #     @param [Moon::Vector4] vec4d
    #   @overload initialize(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16)
    #     Initializes the matrix given all the components
    #     @param [Numeric] n1
    #     @param [Numeric] n2
    #     @param [Numeric] n3
    #     @param [Numeric] n4
    #     @param [Numeric] n5
    #     @param [Numeric] n6
    #     @param [Numeric] n7
    #     @param [Numeric] n8
    #     @param [Numeric] n9
    #     @param [Numeric] n10
    #     @param [Numeric] n11
    #     @param [Numeric] n12
    #     @param [Numeric] n13
    #     @param [Numeric] n14
    #     @param [Numeric] n15
    #     @param [Numeric] n16
    #   @return [self]

    # @!method [](*args)
    #   Retrieves value at given row and col
    #   @overload [](row)
    #     @param [Integer] row
    #     @return [Moon::Vector4]
    #   @overload [](row, col)
    #     @param [Integer] row
    #     @param [Integer] col
    #     @return [Float] value

    # @!method []=(*args)
    #   @overload [](row, value)
    #     @param [Integer] row
    #     @param [Moon::Vector4] value
    #     @return [self]
    #   @overload [](row, col, value)
    #     @param [Integer] row
    #     @param [Integer] col
    #     @param [Numeric] value
    #     @return [self]

    # @!method add(other)
    #   Adds the other to self
    #   @param [Numeric, Vector4, Matrix4] other
    #   @return [self]

    # @!method sub(other)
    #   Subtracts the other from self
    #   @param [Numeric, Vector4, Matrix4] other
    #   @return [self]

    # @!method mul(other)
    #   Multiplies the self with other
    #   @param [Numeric, Vector4, Matrix4] other
    #   @return [self]

    # @!method div(other)
    #   Divides the self by other
    #   @param [Numeric, Vector4, Matrix4] other
    #   @return [self]

    # @!method +(other)
    #   @see #add
    #   @return [Matrix4]

    # @!method -(other)
    #   @see #sub
    #   @return [Matrix4]

    # @!method *(other)
    #   @see #mul
    #   @return [Matrix4]

    # @!method /(other)
    #   @see #div
    #   @return [Matrix4]

    # @!method translate!(*args)
    #   Translates self by the given parameters
    #   @overload translate!(vec3)
    #     @param [Moon::Vector3] vec3
    #   @overload translate!(x, y, z)
    #     @param [Numeric] x
    #     @param [Numeric] y
    #     @param [Numeric] z
    #   @return [self]

    # @!method translate(*args)
    #   Translates self by the given parameters and returns a new Matrix
    #   @overload translate!(vec3)
    #     @param [Moon::Vector3] vec3
    #   @overload translate!(x, y, z)
    #     @param [Numeric] x
    #     @param [Numeric] y
    #     @param [Numeric] z
    #   @return [Matrix4]

    # @!method rotate!(*args)
    #   Rotates self by the given parameters
    #   @overload rotate!(angle, axis)
    #     @param [Numeric] angle
    #     @param [Moon::Vector3] axis
    #   @overload rotate!(angle, x, y, z)
    #     @param [Numeric] angle
    #     @param [Numeric] x
    #     @param [Numeric] y
    #     @param [Numeric] z
    #   @return [self]

    # @!method rotate(*args)
    #   Rotates self by the given parameters
    #   @overload rotate(angle, axis)
    #     @param [Numeric] angle
    #     @param [Moon::Vector3] axis
    #   @overload rotate(angle, x, y, z)
    #     @param [Numeric] angle
    #     @param [Numeric] x
    #     @param [Numeric] y
    #     @param [Numeric] z
    #   @return [Matrix4]

    # @!method scale!(*args)
    #   Scales self by the given parameters
    #   @overload scale!(vec3)
    #     @param [Moon::Vector3] vec3
    #   @overload scale!(x, y, z)
    #     @param [Numeric] x
    #     @param [Numeric] y
    #     @param [Numeric] z
    #   @return [self]

    # @!method scale(*args)
    #   Scales self by the given parameters and returns a new Matrix
    #   @overload scale!(vec3)
    #     @param [Moon::Vector3] vec3
    #   @overload scale!(x, y, z)
    #     @param [Numeric] x
    #     @param [Numeric] y
    #     @param [Numeric] z
    #   @return [Matrix4]
  end
end
