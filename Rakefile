require 'yard'
require 'yard/rake/yardoc_task'

YARD::Rake::YardocTask.new do |t|
  t.files = ['docstub/**/*.rb',
             'modules/**/*.rb']
end

task :build do
  sh 'bash build.sh'
end

task doc: :yard
task default: [:build, :doc]
