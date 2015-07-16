require 'yard'
require 'yard/rake/yardoc_task'
require_relative 'lib/platform'

load 'tasks/mruby.rake'
load 'tasks/shaders.rake'

YARD::Rake::YardocTask.new do |t|
  t.files = ['docstub/**/*.rb',
             'modules/**/*.rb']
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
