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
      puts "[Cache:#{branch_name}] GET #{name}, #{args.join(", ")}"
      key = [name, *args]
      constructor = @constructor[branch_name]
      if constructor.is_a?(Proc)
        (@cache[branch_name] ||= {})[key] ||= constructor.(name, *args)
      else
        (@cache[branch_name] ||= {})[key] ||= constructor[name].(*args)
      end
    end
    @constructor[branch_name] = yield
  end

  class << self
    private :branch
  end

end
