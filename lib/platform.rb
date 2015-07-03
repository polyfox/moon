module Platform
  def self.cygwin?
    RUBY_PLATFORM.include?('cygwin')
  end

  def self.mingw?
    RUBY_PLATFORM.include?('mingw')
  end

  def self.msvcr?
    RUBY_PLATFORM.include?('msvcr')
  end

  def self.windows?
    cygwin? || mingw? || msvcr?
  end

  def self.linux?
    RUBY_PLATFORM.include?('linux')
  end

  def self.darwin?
    RUBY_PLATFORM.include?('darwin')
  end

  def self.unix?
    linux? || darwin?
  end
end
