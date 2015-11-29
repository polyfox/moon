#encoding:UTF-8
MRuby::Gem::Specification.new('mruby-moon-engine') do |spec|
  spec.license = 'MIT'
  spec.authors = ['Blaž Hrastnik', 'Corey Powell']
  spec.version = '0.0.0'
  spec.summary = 'Moon engine class'
  spec.description = 'Moon Engine\'s engine class'
  spec.homepage = 'https://github.com/polyfox/moon'
  spec.add_dependency 'mruby-moon-system'
  spec.add_dependency 'mruby-moon-graphics'
  #spec.add_dependency 'mruby-moon-audio'
end
