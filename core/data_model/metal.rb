#
# moon/core/data_model/metal.rb
#
# DataModel serves as the base class for all other Data objects in ES
# This was copied from the original Earthen source and updated to with moon.
module Moon
  module DataModel
    class Metal
      @@dmid = 0

      attr_reader :dmid          # DataModel ID

      def initialize(opts={})
        dm_set(opts) # since some subclasses like to overload the set method
        initialize_fields(opts.keys)
        @dmid = @@dmid += 1
        yield self if block_given?
        post_init
      end

      def post_init
        #
      end

      def initialize_field(key)
        field = self.class.all_fields.fetch(key)
        self[key] = field.make_default(self)
      end

      def initialize_fields(dont_init=[])
        self.class.all_fields.each do |k, field|
          next if dont_init.include?(k)
          self[k] = field.make_default(self)
        end
      end

      def set(opts)
        opts.each { |k, v| self[k] = v }
        self
      end

      def set!(opts)
        opts.each { |k, v| send("_#{k}_set", v) }
        self
      end

      ###
      # @param [Symbol] key
      # @return [Object]
      ###
      def [](key)
        send key
      end

      ###
      # @param [Symbol] key
      # @param [Object] value
      ###
      def []=(key, value)
        send "#{key}=", value
      end

      ###
      # A recursive version of to_h
      # @return [Hash<Symbol, Object>]
      ###
      def to_h
        hsh = {}
        self.class.all_fields.each_key { |k| hsh[k] = send(k) }
        hsh
      end

      ###
      # A recursive version of to_h
      # @return [Hash<Symbol, Object>]
      ###
      def to_h_r
        hsh = {}
        self.class.all_fields.each_key do |k|
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
        self.class.all_fields.each_key do |k|
          hsh[k] = export_obj(send(k))
        end
        hsh["&class"] = self.class.to_s
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
      # @return [self]
      ###
      def import(data)
        self.class.all_fields.each_key do |k|
          self[k] = import_obj(data[k.to_s])
        end
        self
      end

      ###
      # @return [self]
      ###
      def validate
        self.class.all_fields.each do |key, field|
          field.check_type(key, self[key])
        end
        self
      end

      ###
      # @return [self]
      ###
      def force_types
        self.class.all_fields.each do |k, field|
          value = self[k]
          type = field.type_class
          next if value.nil? && field.allow_nil?
          next if value.is_a?(type)
          self[k] =
          if type == Array      then value.to_a
          elsif type == Float   then value.to_f
          elsif type == Hash    then value.to_h
          elsif type == Integer then value.to_i
          elsif type == String  then value.to_s
          else
            custom_type_cast(k, value)
          end
        end
        self
      end

      ###
      # @param [Symbol] key
      # @param [Object] value
      ###
      def custom_type_cast(key, value)
        raise "#{key}, #{value}"
      end

      ###
      # @param [Hash] data
      ###
      def self.load(data)
        instance = new
        instance.import data
        instance
      end

      ###
      # @return [Array<Symbol>]
      ###
      def self.fields
        (@fields ||= {})
      end

      ###
      # Traverses all parent classes and returns every field every defined
      # in the object chain
      # @return [Array<Symbol>]
      ###
      def self.all_fields
        ancestors.reverse.each_with_object({}) do |klass, hash|
          hash.merge!(klass.fields) if klass.respond_to?(:fields)
        end
      end

      ###
      # define a new field
      # @return [Symbol]
      ###
      def self.field(sym, options)
        # if the default value is set to nil, and allow_nil hasn't already
        # been set, then assign it as true
        if options.key?(:default) && options[:default].nil?
          options[:allow_nil] = true unless options.key?(:allow_nil)
        end
        # if default value does not exist, but the field allows nil
        # set the default as nil
        if !options.key?(:default) && options[:allow_nil]
          options[:default] = nil
        end
        field = fields[sym.to_sym] = Field.new(options)

        setter = "_#{sym}_set"

        attr_accessor sym
        alias_method setter, "#{sym}="

        define_method "#{sym}=" do |obj|
          field.check_type(sym, obj)
          send(setter, obj)
        end

        sym.to_sym
      end

      ###
      # defines a new Array field
      # @return [Symbol]
      ###
      def self.array(sym, options)
        field sym, options.merge(type: [options.fetch(:type)])
      end

      ###
      # @param [Symbol] sym
      ###
      def self.remove_field(sym)
        fields.delete(sym.to_sym)
      end

      alias :dm_set :set
      alias :dm_set! :set!

      private :custom_type_cast
      private :initialize_fields
      private :export_obj
      private :import_obj
      private :post_init
    end
  end
end
