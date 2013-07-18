MRuby::Build.new do |conf|
  toolchain :gcc

  # include the default GEMs
  conf.gembox 'default'

  # Temporary custom solution. If we end up
  # creating a custom gem, make a gem folder
  # in moon root and put it there.
  conf.gem :github => 'archSeer/mruby-require'
end