require 'yard'
require 'yard/rake/yardoc_task'
require_relative 'lib/platform'

load 'tasks/freetype-gl.rake'
load 'tasks/glfw.rake'
load 'tasks/gorilla-audio.rake'
load 'tasks/mruby.rake'
load 'tasks/sil.rake'
load 'tasks/soil.rake'

YARD::Rake::YardocTask.new do |t|
  t.files = ['docstub/**/*.rb',
             'modules/**/*.rb']
end

task clean: [
  'freetype-gl:clean',
  'glfw:clean',
  'gorilla-audio:clean',
  'mruby:clean',
  'sil:clean',
  'soil:clean'
]

task :remove_build_dir do
  FileUtils.rm_rf 'build'
end

task hard_clean: [
  'freetype-gl:hard_clean',
  'glfw:hard_clean',
  'gorilla-audio:hard_clean',
  'mruby:hard_clean',
  'sil:hard_clean',
  'soil:hard_clean',
  'remove_build_dir'
]

task :build do
  FileUtils.mkdir_p 'build'
  Dir.chdir 'build' do
    sh 'cmake .. && make'
  end
end

task clean_build: ['mruby:hard_clean', 'remove_build_dir']
task rebuild: [:clean_build, :build]

task test: 'mruby:test'
task docs: :yard
task default: [:build, :docs]
