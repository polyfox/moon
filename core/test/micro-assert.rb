module Moon
  module Test
    class AssertError < StandardError
      #
    end

    def init_assertions
      @assertions = 0
      @assertions_raised = 0
    end

    def raise_assert_error(msg=nil, original_exception=nil)
      @assertions_raised += 1
      if original_exception
        raise AssertError, (msg || "assertion failed\n") + original_exception.inspect + "\n" + original_exception.backtrace("\n").indent(2)
      else
        raise AssertError, (msg || "assertion failed")
      end
    end

    def assert(obj, msg=nil)
      @assertions += 1
      obj ? true : raise_assert_error(msg)
    end

    def assert_true(obj, msg=nil)
      assert(!!obj, msg)
    end

    def assert_false(obj, msg=nil)
      assert_true(!obj, msg)
    end

    def assert_equal(a, b, msg=nil)
      assert_true(a == b, msg || "expected #{a} == #{b}")
    end

    def assert_not_equal(a, b, msg=nil)
      assert_true(a != b, msg || "expected #{a} != #{b}")
    end

    def assert_nil(a, msg=nil)
      assert_equal(a, nil, msg)
    end

    def assert_raise(klass, msg=nil)
      begin
        yield
      rescue Exception => e
        assert_equal(e.class, klass, msg)
      end
    end

    def assert_not_raise(msg=nil)
      @assertions += 1
      begin
        yield
      rescue Exception => e
        raise_assert_error(msg || "expected no raise", e)
      end
    end

    def assert_kind_of(klass, obj, msg=nil)
      assert_true(obj.kind_of?(klass), msg || "expected #{obj} to be kind_of? #{klass}")
    end

    def assert_not_kind_of(klass, obj, msg=nil)
      assert_false(obj.kind_of?(klass), msg || "expected #{obj} not to be kind_of? #{klass}")
    end

    def assert_float(exp, act, msg=nil)
      assert_true(check_float(exp, act), msg || "expected #{exp} to equal #{act}")
    end

    ## Swiped from mruby/test/assert
    # Performs fuzzy check for equality on methods returning floats
    def check_float(a, b)
      #tolerance = 1e-12
      tolerance = 1e-2
      a = a.to_f
      b = b.to_f
      if a.finite? and b.finite?
        (a-b).abs < tolerance
      else
        true
      end
    end
  end
end
