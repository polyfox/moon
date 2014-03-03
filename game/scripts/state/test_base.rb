class State::TestBase < State

  class AssertFailure < StandardError

  end

  def assert(str, obj)
    unless obj
      raise AssertFailure, "[#{str}] \n  #{obj}"
    end
    return true
  end

  def assert_equal(str, obj, other)
    unless obj == other
      raise AssertFailure, "[#{str}] \n  #{obj} was not equal to #{other}"
    end
    return true
  end

  def assert_class_is_kind_of(str, obj, klass)
    unless obj.kind_of?(klass)
      raise AssertFailure, "[#{str}] \n  #{obj} was not an instance of #{klass}"
    end
    return true
  end

  def assert_exception(str, exception)
    begin
      yield
    rescue => ex
      unless ex.kind_of?(exception)
        raise AssertFailure, "[#{str}] \n  #{ex.class} was raised instead of #{exception}"
      end
      return true
    end
    raise AssertFailure, "[#{str}] \n  #{exception} was not raised"
  end

  def init
    super
    begin
      run_tests
    rescue => ex
      puts "run_tests has failed: \n  #{ex.inspect}"
    end
    State.pop
  end

  def test(name)
    puts "Running Test(#{name})"
    begin
      yield
    rescue => ex
      puts "Test(#{name}) has failed: \n  #{ex.inspect}"
    end
  end

  def run_tests
    # replace.me
  end

end