libs = Dir.glob("build/**/*.{so,a,so.*}").sort
puts "Libraries present are:"
libs.each do |lib|
  puts "\t#{lib}"
end
