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

task hard_clean: [
  'freetype-gl:hard_clean',
  'glfw:hard_clean',
  'gorilla-audio:hard_clean',
  'mruby:hard_clean',
  'sil:hard_clean',
  'soil:hard_clean'
]

task build: [
  'freetype-gl:build',
  'glfw:build',
  'gorilla-audio:build',
  'mruby:build',
  'sil:build',
  'soil:build'
]

task test: 'mruby:test'
task docs: :yard
task default: [:build, :docs]
