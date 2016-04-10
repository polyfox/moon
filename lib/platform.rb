class Platform
  def initialize(platform = RUBY_PLATFORM)
    @platform_name = platform
  end

  def include?(str)
    @platform_name.include?(str)
  end

  def cygwin?
    include?('cygwin')
  end

  def mingw?
    include?('mingw')
  end

  def msvcr?
    include?('msvcr')
  end

  def windows?
    cygwin? || mingw? || msvcr?
  end

  def linux?
    include?('linux')
  end

  def darwin?
    include?('darwin')
  end

  def unix?
    linux? || darwin?
  end
end
