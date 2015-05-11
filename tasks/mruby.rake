require 'fileutils'

namespace :mruby do
  rootdir = File.expand_path('../', File.dirname(__FILE__))
  dir = File.expand_path('vendor/mruby', rootdir)
  configfile = File.expand_path(ENV['MOON_MRUBY_CONFIG'] || 'mrb_config.rb', rootdir)

  task :build_mruby do
    sh %(cd "#{dir}" && (MRUBY_CONFIG="#{configfile}" make -j 4))
  end

  task build: [
    'glfw:build',
    'soil:build',
    'freetype-gl:build',
    'nanovg:build',
    'gorilla-audio:build',
    :build_mruby
  ]

  task :test do
    sh %(cd "#{dir}" && (MRUBY_CONFIG="#{configfile}" make -j 4 test))
  end

  task :clean do
    sh %(make -C "#{dir}" clean)
  end

  task :clean_gems do
    FileUtils.rm_rf(File.join(dir, 'build/mrbgems'))
  end

  task :hard_clean do
    FileUtils.rm_rf(File.join(dir, 'build'))
  end
end
