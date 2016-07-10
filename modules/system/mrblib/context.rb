module Moon
  # Utility class for code execution, all code evaluated in a Context will be
  # retained.
  # Currently unstable, still on the TODO list.
  class Context
    alias :each_symbol_always_yield :each_symbol
    # @yieldparam [Symbol] symbol
    def each_symbol(&block)
      return to_enum :each_symbol unless block_given?
      each_symbol_always_yield(&block)
    end

    def symbols
      each_symbol.to_a
    end
  end
end
