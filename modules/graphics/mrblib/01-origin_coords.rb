module Moon
  module OriginCoords
    extend TypedAttributes

    attribute :origin,  Vector2

    def ox
      origin.x
    end

    def oy
      origin.y
    end

    def ox= x
      origin.x = x
    end

    def oy= y
      origin.y = y
    end
  end
end
