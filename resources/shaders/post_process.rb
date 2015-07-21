#!/usr/bin/env ruby
ARGV.each do |filename|
  pruned_new_lines = false
  prev_line = nil
  File.open(filename, "r") do |f|
    f.each_line do |line|
      next if line == "\n" unless pruned_new_lines
      pruned_new_lines = true
      # removes junk, lines with semicolons, spaces before #macros and so on
      this_line = line.gsub(/^\s\b/, '').gsub(/^\s*;/, '').gsub(/^\s+#/, '#')
      # skip this line if its a newline and the previous line was also
      # a newline
      next if prev_line == "\n" && this_line == "\n"
      print this_line
      prev_line = this_line
    end
  end
end
