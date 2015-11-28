#encoding:UTF-8
MRuby::Gem::Specification.new('mruby-moon-audio-gorilla') do |spec|
  spec.license = 'MIT'
  spec.authors = ['Blaž Hrastnik', 'Corey Powell']
  spec.version = '0.0.0'
  spec.summary = 'Moon audio modules'
  spec.description = 'Moon Engine\'s audio module (using Gorilla Audio)'
  spec.homepage = 'https://github.com/polyfox/moon'
  # compiler config
  spec.cc.include_paths << ["#{build.root}/src", "#{build.root}/include"]
  # I think we inherited the parent flags, might as well flatten the array to
  # be sure we're working with Array<String> and not Array<String, Array<String>>
  spec.cc.flags = spec.cc.flags.flatten
  # Treat all warnings as errors, forces you to write compliant code
  [spec.cc, spec.cxx].each do |cc|
    cc.flags << '-Wall'
    cc.flags << '-Wextra'
  end

  #spec.cxx do |cxx|
  #  cxx.flags << '-std=c++11'
  #end

  spec.add_dependency 'mruby-moon-system'
end
