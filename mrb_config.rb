MRuby::Build.new do |conf|
  toolchain :gcc

  # include the default GEMs
  conf.gembox 'default'

  conf.gem core: 'mruby-string-utf8'
  conf.gem core: 'mruby-eval'

  conf.gem github: 'iij/mruby-io'
  conf.gem github: 'iij/mruby-dir'
  conf.gem github: 'iij/mruby-regexp-pcre'
  conf.gem github: 'iij/mruby-tempfile'
  conf.gem github: 'iij/mruby-require'

  # moon gems
  d = File.dirname(__FILE__)
  # moon-system
  conf.gem File.join(d, 'modules/system')
  # moon-graphics
  conf.gem File.join(d, 'modules/graphics')
  # moon-audio
  conf.gem File.join(d, 'modules/audio')
  # moon-engine
  conf.gem File.join(d, 'modules/engine')

  # so we can have nice things
  conf.gem github: 'AndrewBelt/mruby-yaml'
  conf.gem github: 'IceDragon200/mruby-glfw3'
  conf.gem github: 'IceDragon200/mruby-glew'
  conf.gem github: 'IceDragon200/mruby-nanovg'
  conf.gem github: 'IceDragon200/mruby-oui'
  conf.gem github: 'IceDragon200/mruby-blendish'

  [conf.cc, conf.cxx].each do |cc|
    # system
    cc.include_paths << File.join(d, 'vendor/glm')
    # graphics
    cc.include_paths << File.join(d, 'vendor/glfw/include')
    cc.include_paths << File.join(d, 'vendor/nanovg/src')
    cc.include_paths << File.join(d, 'vendor/oui-blendish')
    cc.include_paths << File.join(d, 'vendor/soil/include')
    cc.include_paths << File.join(d, 'vendor/sil/include')
    cc.include_paths << File.join(d, 'vendor/freetype-gl')
    # audio
    cc.include_paths << File.join(d, 'vendor/gorilla-audio/include')
    # moon include
    cc.include_paths << File.join(d, 'modules/system/include')
    cc.include_paths << File.join(d, 'modules/graphics/include')
    cc.include_paths << File.join(d, 'modules/audio/include')
  end

  conf.linker do |l|
    linker.library_paths << File.join(d, 'vendor/nanovg/build')
    linker.library_paths << File.join(d, 'vendor/soil/build')
    linker.library_paths << File.join(d, 'vendor/sil/build')
    linker.library_paths << File.join(d, 'vendor/freetype-gl/build')
    linker.library_paths << File.join(d, 'vendor/gorilla-audio/build/build')
    linker.library_paths << File.join(d, 'vendor/glfw/build')
    linker.libraries << 'pthread'
    linker.libraries << 'GL'
    linker.libraries << 'openal'
    linker.libraries << 'glfw'
    linker.libraries << 'nanovg'
    linker.libraries << 'SOIL'
    linker.libraries << 'SIL'
    linker.libraries << 'freetype'
    linker.libraries << 'freetype-gl'
    linker.libraries << 'gorilla'
  end
end
