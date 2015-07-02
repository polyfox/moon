require 'yard'
require 'yard/rake/yardoc_task'
require_relative 'lib/platform'

load 'tasks/mruby.rake'

YARD::Rake::YardocTask.new do |t|
  t.files = ['docstub/**/*.rb',
             'modules/**/*.rb']
end

task :clean do
  if Dir.exist?('build')
    Dir.chdir 'build' do
      sh 'make clean'
    end
  end
end

task :remove_build_dir do
  FileUtils::Verbose.rm_rf 'build'
end

task hard_clean: :remove_build_dir

task :build do
  FileUtils.mkdir_p 'build'
  Dir.chdir 'build' do
    cmd = 'cmake ..'
    cmd << " -DRAKE_VERBOSE=ON" if Rake.verbose
    sh cmd
    sh 'make'
  end
end

task clean_build: ['mruby:hard_clean', 'remove_build_dir']
task rebuild: [:clean_build, :build]

task test: 'mruby:test'
task docs: :yard
task default: [:build, :docs]
