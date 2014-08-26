module YAML
  def self.load_file(filename)
    load(File.read(filename))
  end

  def self.save_file(filename, obj)
    File.open(filename, "w") { |f| f.write(dump(obj)) }
  end
end
