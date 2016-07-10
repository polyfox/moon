require 'yard'
require 'yard/mruby'
require 'yard/rake/yardoc_task'
require_relative 'lib/platform'

load 'tasks/mruby.rake'
load 'tasks/shaders.rake'

module YARD::MRuby::Handlers::C::Source
  class InitHandler < Base
    MATCH3 = /mmrb_\w+_init\s*\(/mx
    handles MATCH3
  end
end

YARD::Parser::SourceParser.register_parser_type :header, YARD::MRuby::Parser::C::HeaderParser, 'hxx'
YARD::Parser::SourceParser.register_parser_type :source, YARD::MRuby::Parser::C::SourceParser, 'cxx'

YARD::Rake::YardocTask.new do |t|
  t.files = %w{modules/system/mrblib/typed_attributes.rb
  modules/**/src/**/mrb_*.cxx modules/**/mrblib/**/*.rb modules/**/include/**/mrb/**/*.hxx}
end

task :cmake_clean do
  if Dir.exist?('build')
    Dir.chdir 'build' do
      sh 'make clean'
    end
  end
end

task clean: ['shaders:clean', :cmake_clean]

task :remove_build_dir do
  FileUtils::Verbose.rm_rf 'build'
end

task hard_clean: ['shaders:clean', :remove_build_dir]

task :cmake_build do
  FileUtils.mkdir_p 'build'
  Dir.chdir 'build' do
    cmd = 'cmake ..'
    cmd << " -DRAKE_VERBOSE=ON" if Rake.verbose
    sh cmd
    sh 'make'
  end
end

task build: ['shaders:build', :cmake_build]
task clean_build: ['mruby:hard_clean', :hard_clean]
task rebuild: [:clean_build, :build]

task test: 'mruby:test'
task docs: :yard
task default: [:build, :docs]
