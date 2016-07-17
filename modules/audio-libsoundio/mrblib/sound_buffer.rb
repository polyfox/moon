module Moon
  class SoundBuffer
    private :initialize_create
    private :initialize_by_filename

    def initialize(*args)
      if args.size == 1
        initialize_by_filename(args.first)
      elsif args.size >= 2
        initialize_create(*args)
      else
        raise ArgumentError, "expected wrong argument count, (expected 1, 2 or 3)"
      end
    end
  end
end
