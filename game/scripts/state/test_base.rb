class State::TestBase < State

  class AssertFailure < StandardError

  end

  class Logger

    attr_reader :indent

    def initialize
      @indent = 0
    end

    def dec
      @indent -= 1
      raise "Indent became less than 0" if @indent < 0
    end

    def inc
      @indent += 1
      if block_given?
        yield
        dec
      end
    end

    def filler
      "  "
    end

    def nl
      puts
    end

    def append(str)
      print str
    end

    def plop(str)
      append(" #{str}")
    end

    def slab(str)
      print (filler * @indent) + str
    end

    def log(str)
      puts (filler * @indent) + str
    end

    def info(str)
      log "@@ #{str}"
    end

    def error(str)
      log "!! #{str}"
    end

    def tpass
      plop "PASSED\n"
    end

    def tfail
      plop "FAILED\n"
    end

  end

  def assert(str, obj)
    @assert_count += 1
    @logger.slab str
    @logger.inc do
      unless obj
        @assert_fail_count += 1
        @logger.tfail
        err = AssertFailure.new "#{obj}"
        @logger.error err.inspect
      end
    end
    @logger.tpass
    return true
  end

  def assert_equal(str, obj, other)
    @assert_count += 1
    @logger.slab str
    @logger.inc do
      unless obj == other
        @assert_fail_count += 1
        @logger.tfail
        err = AssertFailure.new "#{obj} was not equal to #{other}"
        @logger.error err.inspect
      end
    end
    @logger.tpass
    return true
  end

  def assert_class_is_kind_of(str, obj, klass)
    @assert_count += 1
    @logger.slab str
    @logger.inc do
      unless obj.kind_of?(klass)
        @assert_fail_count += 1
        @logger.tfail
        err = AssertFailure.new "#{obj} was not an instance of #{klass}"
        @logger.error err.inspect
      end
    end
    @logger.tpass
    return true
  end

  def assert_exception(str, exception)
    @assert_count += 1
    @logger.slab str
    success = false # workaround for return bug
    begin
      yield
    rescue => ex
      unless ex.kind_of?(exception)
        @assert_fail_count += 1
        @logger.tfail
        @logger.inc do
          err = AssertFailure.new "#{ex.class} was raised instead of #{exception}"
          @logger.error err.inspect
        end
      end
      @logger.tpass
      success = true
      #return true # BUG
    end
    return true if success
    @assert_fail_count += 1
    @logger.tfail
    @logger.inc do
      err = AssertFailure.new "#{exception} was not raised"
      @logger.error err.inspect
    end
  end

  def init
    super
    @logger = Logger.new
    @test_count = 0
    @fail_count = 0
    @assert_count = 0
    @assert_fail_count = 0
    begin
      run_tests
    rescue => ex
      @logger.nl
      @logger.error "run_tests has failed: \n  #{ex.inspect}"
    end
    @logger.info "Test have been completed"
    @logger.info "  Tests: #{@test_count - @fail_count} / #{@test_count}"
    @logger.info "  Assertions: #{@assert_count - @assert_fail_count} / #{@assert_count}"
    State.pop
  end

  def test(name)
    @logger.nl
    @logger.info "Running Test(#{name})"
    @logger.inc do
      @test_count += 1
      begin
        yield
      rescue => ex
        @fail_count += 1
        @logger.nl
        @logger.error "Test(#{name}) has failed: \n  #{ex.inspect}"
      end
    end
    @logger.nl
  end

  def run_tests
    # replace.me
  end

end