class String

  def indent(n)
    result = ""
    self.each_line do |line|
      result << (" " * n) +  line
    end
    result
  end

end

@tests = []
@test_stack = [@tests]

def describe(obj)
  @test_stack << []
  yield
  stack = @test_stack.pop
  stack.map! do |a|
    str, b = *a
    [obj.to_s + " " + str, b]
  end
  @test_stack[-1].concat(stack)
end

def it(str, &block)
  @test_stack[-1] << [str, block]
end

def run_specs
  passed = 0
  failures = []
  @tests.each do |a|
    str, block = *a
    #print str
    begin
      block.call
      #print " .\n"
      print "."
      passed += 1
    rescue => ex
      #print " F\n"
      print "F"
      failures << [str, ex.inspect.dup, ex.backtrace.dup]
    end
  end
  puts
  failures.each_with_index do |a, i|
    str, inspect, backtrace = *a
    puts "##{i} #{str}"
    puts inspect
    puts backtrace.join("\n").indent(2)
  end
  puts
  puts "Assertions " << (@assertions-@assertions_raised).to_s << " / " << @assertions.to_s
  puts "Tests " << passed.to_s << " / " << @tests.size.to_s
end
