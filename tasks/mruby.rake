require 'fileutils'

namespace :mruby do
  rootdir = File.expand_path('../', File.dirname(__FILE__))
  dir = File.expand_path('vendor/mruby', rootdir)

  mrb_exec = lambda do |command = nil|
    Dir.chdir dir do
      cmd = [%(MRUBY_CONFIG="#{rootdir}/mrb_config.rb")]
      cmd.push command if command
      cmd.unshift("--verbose") if Rake.verbose
      cmd.unshift("rake")
      sh cmd.join(" ")
    end
  end

  task :build do
    mrb_exec.call
  end

  task :clean do
    mrb_exec.call 'clean'
  end

  task :clean_gems do
    FileUtils::Verbose.rm_rf(File.join(dir, 'build/mrbgems'))
  end

  task :deep_clean do
    FileUtils::Verbose.rm_rf(File.join(dir, 'build'))
  end

  task hard_clean: :deep_clean

  task :test do
    mrb_exec.call 'test'
  end
end
