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
      key = [name, *args]
      (@cache[branch_name] ||= {})[key] ||= @constructor[branch_name][name].(*args)
    end
    yield (@constructor[branch_name] ||= {})
  end

  class << self
    private :branch
  end

end
