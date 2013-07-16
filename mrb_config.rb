MRuby::Build.new do |conf|
  toolchain :gcc

  # include the default GEMs
  conf.gembox 'default'

  # conf.gem :github => 'mattn/mruby-require'
end