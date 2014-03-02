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
      prep_test
      do_test
    rescue
      STDERR.puts "Test aborted!"
    end
  end

  def prep_test
  end

  def do_test
    # replace.me
  end

end