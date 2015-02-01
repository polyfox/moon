MRuby::Build.new do |conf|
  d = File.dirname(__FILE__)
  toolchain :gcc

  # include the default GEMs
  conf.gembox 'default'

  conf.gem core: 'mruby-string-utf8'
  conf.gem core: 'mruby-eval'

  conf.gem github: 'iij/mruby-io'
  conf.gem github: 'iij/mruby-dir'
  conf.gem github: 'iij/mruby-regexp-pcre'

  conf.gem github: 'IceDragon200/mruby-require'

  conf.gem File.join(d, 'modules/system')

  # so we can have nice things
  conf.gem github: 'AndrewBelt/mruby-yaml'

  conf.gem github: 'IceDragon200/mruby-nanovg'
  conf.gem github: 'IceDragon200/mruby-oui'
  conf.gem github: 'IceDragon200/mruby-blendish'

  [conf.cc, conf.cxx].each do |cc|
    cc.include_paths << File.join(d, 'vendor/glm')
    cc.include_paths << File.join(d, 'vendor/nanovg/src')
    cc.include_paths << File.join(d, 'vendor/oui-blendish')
  end

  conf.linker do |l|
    linker.library_paths << File.join(d, 'vendor/nanovg/build')
    linker.libraries << 'GL'
    linker.libraries << 'nanovg'
  end
end
