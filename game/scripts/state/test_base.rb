class State::TestBase < State

  class AssertFailure < StandardError

  end

  def assert(str, obj)
    unless obj
      raise AssertFailure, "[#{str}] #{obj}"
    end
  end

  def assert_equal(str, obj, other)
    unless obj == other
      raise AssertFailure, "[#{str}] #{obj} was not equal to #{other}"
    end
  end

  def init
    super
    begin
      run_tests
    rescue => ex
      puts "run_tests has failed: #{ex.inspect}"
    end
  end

  def test(name)
    begin
      yield
    rescue => ex
      puts "Test(#{name}) has failed: #{ex.inspect}"
    end
  end

  def run_tests
    # replace.me
  end

end