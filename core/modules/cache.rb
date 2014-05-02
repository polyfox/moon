#
# moon/core/modules/cache.rb
#
module Cache

  @cache = {}
  @constructor = {}

  extend self

  def clear(branch_name=nil)
    if branch_name
      if cache = @cache[branch_name]
        cache.clear
      end
    else
      @cache.clear
    end
  end

  def branch(branch_name)
    define_method(branch_name) do |name, *args|
      puts "[Cache:#{branch_name}] GET #{name.inspect}, #{args.join(", ")}"
      key = [name, *args]
      constructor = @constructor[branch_name]
      case constructor
      when Proc
        (@cache[branch_name] ||= {})[key] ||= constructor.(name, *args)
      when Hash
        func = constructor[name]
        unless func
          raise RuntimeError,
                "[Cache:#{branch_name}] ERR #{name.inspect}, #{args.join(", ")} (no loader)"
        end
        (@cache[branch_name] ||= {})[key] ||= func.(*args)
      else
        raise IndexError,
              "[Cache:#{branch_name}] ERR #{name.inspect}, #{args.join(", ")} (no constructor)"
      end
    end
    @constructor[branch_name] = yield
  end

  class << self
    private :branch
  end

end
