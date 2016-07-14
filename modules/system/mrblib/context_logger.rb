module Moon
  # Internal Logger used by moon classes and systems
  # It implements a very generic subset of the Logger methods
  class ContextLogger
    # The logging level
    module Level
      # Low-level information, mostly for developers.
      DEBUG = 0
      # Generic (useful) information about system operation.
      INFO = 1
      # A warning.
      WARN = 2
      # A handleable error condition.
      ERROR = 3
      # An unhandleable error that results in a program crash.
      FATAL = 4
      # An unknown message that should always be logged.
      UNKNOWN = 5
      # @return [Hash<Integer, String>]
      LEVEL_TO_NAME = {
        DEBUG => 'DEBUG',
        INFO => 'INFO',
        WARN => 'WARN',
        ERROR => 'ERROR',
        FATAL => 'FATAL',
        UNKNOWN => 'UNKNOWN'
      }
      # @return [Hash<String, Integer>]
      NAME_TO_LEVEL = LEVEL_TO_NAME.invert
    end

    class << self
      # @!attribute [rw] context_separator
      #   @return [String] default '/'
      attr_accessor :context_separator
    end

    self.context_separator = '/'

    # @return [IO, #puts]
    attr_accessor :device
    # @return [Integer]
    attr_reader :level
    # @return [String]
    attr_accessor :context
    protected :context=

    # @param [IO, #puts] device
    # @param [String] ctx
    # @param [String, Symbol, Integer] lvl
    def initialize(device, ctx = nil, lvl = Level::DEBUG)
      @device = device
      @context = ctx
      self.level = lvl
    end

    # Change the logger's level
    #
    # @param [String, Symbol, Integer] lvl
    def level=(lvl)
      @level = if lvl.is_a?(Integer)
        lvl
      else
        Level::NAME_TO_LEVEL.fetch(lvl.to_s.upcase)
      end
    end

    def initialize_copy(other)
      @device = other.device
      @context = other.context.dup
      @level = other.level
    end

    private def context_separator
      self.class.context_separator
    end

    # Writes the specified message to the device, if the level is above the
    # current logging level
    #
    # @param [Integer] lvl logging level
    # @param [String] message message to log
    private def write(lvl, msg)
      return if lvl < @level
      level_name = Level::LEVEL_TO_NAME.fetch(lvl)
      if @context
        @device.puts "#{level_name}: [#{@context}] #{msg}"
      else
        @device.puts "#{level_name}: #{msg}"
      end
    end

    # @param [String] msg message to log
    def debug(msg)
      write Level::DEBUG, msg
    end

    # @param [String] msg message to log
    def info(msg)
      write Level::INFO, msg
    end

    # @param [String] msg message to log
    def error(msg)
      write Level::ERROR, msg
    end

    # @param [String] msg message to log
    def warn(msg)
      write Level::WARN, msg
    end

    # @param [String] msg message to log
    def fatal(msg)
      write Level::FATAL, msg
    end

    # @param [String] msg message to log
    def unknown(msg)
      write Level::UNKNOWN, msg
    end

    # Creates a new logger with the context joined to the current
    #
    # @param [String] context
    # @return [Moon::ContextLogger] new instance with context appended
    def new(context)
      dup.tap do |logger|
        logger.context = "#{logger.context}#{context_separator}#{context}"
      end
    end
  end
end
