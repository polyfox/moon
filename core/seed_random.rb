module Moon
  class SeedRandom

    BINARY_DIGITS = %w[0 1]
    OCTAL_DIGITS  = %w[0 1 2 3 4 5 6 7]
    HEX_DIGITS    = %w[0 1 2 3 4 5 6 7 8 9 A B C D E F]
    BASE64_DIGITS = %w[A B C D E F G H I J K L M N O P Q R S T U V W X Y Z] +
                    %w[a b c d e f g h i j k l m n o p q r s t u v w x y z] +
                    %w[0 1 2 3 4 5 6 7 8 9 + /]

    attr_accessor :seed
    attr_accessor :random
    alias :set_seed :seed=
    private :set_seed

    def initialize(seed=rand(0xFFFFFFF))
      self.seed = seed
    end

    def seed=(_seed)
      set_seed _seed.to_i
      self.random = Random.new(self.seed)
    end

    def int(size)
      random.rand(size)
    end

    def sample(array)
      array[int(array.size)]
    end

    def binary(digits)
      digits.times.map { sample(BINARY_DIGITS) }.join("")
    end

    def octal(digits)
      digits.times.map { sample(OCTAL_DIGITS) }.join("")
    end

    def hex(digits)
      digits.times.map { sample(HEX_DIGITS) }.join("")
    end

    def base64(digits)
      digits.times.map { sample(BASE64_DIGITS) }.join("")
    end

    def to_h
      {
        seed: seed
      }
    end

    def export
      to_h.stringify_keys
    end

    def self.load(data)
      new data["seed"].to_i
    end

    ###
    # Per Session SeedRandom object
    ###
    def self.random
      @random ||= SeedRandom.new
    end

  end
end
