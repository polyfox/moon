#
# moon/core/modules/cache.rb
#
module Cache

  @cache = {}
  @constructor = {}

  extend self

  def branch(branch_name)
    define_method(branch_name) do |name, *args|
      key = [name, *args]
      (@cache[branch_name] ||= {})[key] ||= @constructor[branch_name][name].(*args)
    end
    hsh = @constructor[branch_name] ||= {}
    yield hsh
  end

  class << self
    private :branch
  end

end
