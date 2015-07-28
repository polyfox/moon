class Platform
  attr_reader :platform_name
  attr_reader :toolchain

  def initialize(platform_name, toolchain)
    @native = platform_name == nil
    @platform_name = platform_name || RUBY_PLATFORM
    @toolchain = toolchain
  end

  def native?
    @native
  end

  def cygwin?
    platform_name.include?('cygwin')
  end

  def mingw?
    platform_name.include?('mingw')
  end

  def msvcr?
    platform_name.include?('msvcr')
  end

  def windows?
    cygwin? || mingw? || msvcr?
  end

  def linux?
    platform_name.include?('linux')
  end

  def darwin?
    platform_name.include?('darwin')
  end

  def unix?
    linux? || darwin?
  end
end
