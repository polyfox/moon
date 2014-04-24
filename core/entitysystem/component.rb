# Component as mixin
module Component

  module ComponentClass

    def field(name, data)
      (@fields ||= {})[name] = data

      attr_reader name unless method_defined?(name)
      attr_writer name unless method_defined?(name.to_s+"=")
      name
    end

    def fields
      @fields ||= {}
      if superclass.respond_to?(:fields)
        superclass.fields.merge(@fields)
      else
        @fields
      end
    end

  end

  def setup(options={})
    self.class.fields.each do |key, data|
      send("#{key}=", data[:default]) if data.key?(:default)
      send("#{key}=", options[key]) if options.key?(key)
    end
  end

  def to_h # predefine to_h for fields
    self.class.fields.inject({}) do |result, keyval| # res, (k, v) doesn't work?!
      (key, data) = *keyval
      result[key] = self.send(key)
      result
    end
  end

  def export
    to_h.merge(class: self.class.to_s).stringify_keys
  end

  def import(data)
    setup(data)
    self
  end

  def self.included(mod)
    mod.extend ComponentClass
  end

  def self.load(data)
    Object.const_get(data["class"]).new(data.symbolize_keys)
  end

  private :setup

end
