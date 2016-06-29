module Moon
  class SoundBuffer
    def initialize(*args)
      if args.size == 2
        initialize_create(*args)
      elsif args.size == 1
        initialize_by_filename(args.first)
      else
        raise ArgumentError, "expected wrong argument count, (expected 1 or 2)"
      end
    end
  end
end
