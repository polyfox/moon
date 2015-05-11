module Platform
  def self.linux?
    RUBY_PLATFORM.include?('linux')
  end

  def self.darwin?
    RUBY_PLATFORM.include?('darwin')
  end
end
