require 'fileutils'

namespace :mruby do
  rootdir = File.expand_path('../', File.dirname(__FILE__))
  dir = File.expand_path('vendor/mruby', rootdir)

  task :clean do
    sh %(make -C "#{dir}" clean)
  end

  task :clean_gems do
    FileUtils.rm_rf(File.join(dir, 'build/mrbgems'))
  end

  task :hard_clean do
    FileUtils.rm_rf(File.join(dir, 'build'))

  task :test do
    Dir.chdir dir do
      sh %(./minirake MRUBY_CONFIG="#{rootdir}/mrb_config.rb" test)
    end
  end
end
