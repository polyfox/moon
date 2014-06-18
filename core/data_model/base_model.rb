#
# core/data_model/base_model.rb
#   by IceDragon
# DataModel serves as the base class for all other Data objects in ES
# This was copied from the original Earthen source and updated to with moon.
module Moon
  class BaseModel

    class MissingOption < StandardError

      def initialize(type)
        super("missing option #{type}")
      end

    end

    ###
    # @return [Array<Symbol>]
    ###
    def self.fields
      (@fields ||= [])
    end

    ###
    # Traverses all parent classes and returns every field every defined
    # in the object chain
    # @return [Array<Symbol>]
    ###
    def self.all_fields
      ancestors.reverse.each_with_object([]) do |c, r|
        r.concat c.fields if c.respond_to?(:fields)
      end + fields
    end

    ###
    # define a new field
    # @return [Symbol]
    ###
    def self.field(sym, options)
      raise MissingOption.new(:type) unless options.key?(:type)
      fields << [sym.to_sym, options]

      type = options[:type]
      allow_nil = options[:allow_nil]
      setter = "_#{sym}_set"

      attr_accessor sym
      alias_method setter, "#{sym}="

      define_method "#{sym}=" do |obj|
        # validate that obj is of type
        if type.is_a?(Module)
          unless obj.is_a?(type) || (allow_nil && (obj == nil))
            raise TypeError,
                  "[#{sym}] wrong type #{obj.class.inspect} (expected #{type.inspect})"
          end
        # validate that obj is an Array of type
        elsif type.is_a?(Array)
          unless obj.is_a?(Array) || (allow_nil && (obj == nil))
            raise TypeError,
                  "[#{sym}] wrong type #{obj.class.inspect} (expected Array)"
          end
          unless obj.all? { |o| type.any? { |t| o.is_a?(t) } }
            str = type.map { |s| s.inspect }.join(", ")
            raise TypeError,
                  "[#{sym}] wrong type #{obj.class.inspect} (expected #{str})"
          end
        end
        send(setter, obj)
      end

      sym.to_sym
    end

    @@dmid = 0

    attr_reader :dmid          # DataModel ID
    field :id,   type: Integer,  default: 0         # ID
    field :name, type: String,   default: proc {""} # Name of this model
    field :note, type: String,   default: proc {""} # A string for describing this DataModel
    field :tags, type: [String], default: proc {[]} # Used for lookups
    field :meta, type: Hash,     default: proc {{}} # Meta Data, String Values and String Keys

    def initialize(opts={})
      opts.each { |k, v| self[k] = v }
      initialize_fields(opts.keys)
      @dmid = @@dmid += 1
    end

    def initialize_fields(dont_init=[])
      self.class.all_fields.each do |k, options|
        next if dont_init.include?(k)

        case v = options[:default]
        when Proc
          self[k] = v.call options[:type], self
        else
          self[k] = v
        end
      end
    end

    def [](key)
      send key
    end

    def []=(key, value)
      send "#{key}=", value
    end

    ###
    # A recursive version of to_h
    # @return [Hash<Symbol, Object>]
    ###
    def to_h
      hsh = {}
      self.class.all_fields.each { |k, d| hsh[k] = send(k) }
      hsh
    end

    ###
    # A recursive version of to_h
    # @return [Hash<Symbol, Object>]
    ###
    def to_h_r
      hsh = {}
      self.class.all_fields.each do |k, d|
        obj = send(k)
        if obj.is_a?(Array)
          obj = obj.map do |o|
            o.respond_to?(:to_h) ? o.to_h : o
          end
        elsif obj.is_a?(Hash)
          obj = obj.each_with_object({}) do |a, hash|
            k, v = a
            hash[k] = v.respond_to?(:to_h) ? v.to_h : v
          end
        else
          obj = obj.to_h if obj.respond_to?(:to_h)
        end
        hsh[k] = obj
      end
      hsh
    end

    ###
    # @return [Hash|Array]
    ###
    def export_obj(obj)
      if obj.is_a?(Array)
        obj.map { |o| export_obj(o) }
      elsif obj.is_a?(Hash)
        obj.each_with_object({}) do |a, hash|
          k, v = *a
          hash[k] = export_obj(v)
        end
      else
        obj.respond_to?(:export) ? obj.export : obj
      end
    end

    ###
    # @return [Hash]
    ###
    def export
      hsh = {}
      self.class.all_fields.each do |k, d|
        hsh[k] = export_obj send(k)
      end
      hsh.stringify_keys
    end

    ###
    # @param [Object] obj
    ###
    def import_obj(obj)
      if obj.is_a?(Array)
        obj.map { |o| import_obj(o) }
      elsif obj.is_a?(Hash)
        if obj.key?("&class")
          safe_obj = obj.dup
          klass_path = safe_obj.delete("&class")
          klass = Object.const_get(klass_path)
          klass.load safe_obj
        else
          obj.each_with_object({}) do |a, hash|
            k, v = *a
            hash[k] = import_obj(v)
          end
        end
      else
        obj
      end
    end

    ###
    # @param [Hash] data
    ###
    def import(data)
      self.class.all_fields.each do |k, d|
        self[k] = import_obj(data[k.to_s])
      end
    end

    ###
    #
    ###
    def self.load(data)
      instance = new
      instance.import data
      instance
    end

    private :initialize_fields
    private :export_obj
    private :import_obj
  end
end
