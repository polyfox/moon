MRuby::Build.new do |conf|
  toolchain_name = (ENV['MOON_MRUBY_TOOLCHAIN'] || :gcc).to_sym
  toolchain toolchain_name

  puts "\t\\\\ Using #{toolchain_name} Toolchain \\\\"

  # Can I haz sane stack traces please :(
  #enable_debug # debugging causes oui and blendish to spaz out

  # include the default GEMs
  conf.gembox 'default'
  conf.gembox File.join(File.dirname(__FILE__), 'modules/moon')

  conf.gem core: 'mruby-eval'
  conf.gem github: 'iij/mruby-io'                # General IO stuff, File system blah de dah.
  conf.gem github: 'iij/mruby-dir'               # Directory access
  conf.gem github: 'iij/mruby-regexp-pcre'       # Regular expression engine :3
  conf.gem github: 'iij/mruby-tempfile'          # required by iij/mruby-require
  conf.gem github: 'iij/mruby-require'           # require

  # nice things
  conf.gem github: 'AndrewBelt/mruby-yaml'       # YAML :3
  conf.gem github: 'IceDragon200/mruby-nanovg'   # nanovg
  conf.gem github: 'IceDragon200/mruby-oui'      # OUI
  conf.gem github: 'IceDragon200/mruby-blendish' # blendish

  conf.cxx do |cxx|
    std = 'c++11'
    comp = toolchain_name == :clang ? 'clang' : 'gcc'
    result = `#{comp} -dumpversion`.chomp
    case toolchain_name
    when :clang
      # nothing yet
      #if ver < '2.9'
      #  end
    when :gcc
      if result < '4.7'
        std = 'c++0x'
      end
    end
    puts "\t\\\\ Using #{comp}(#{result}) c++ std: #{std} \\\\"
    cxx.flags << "-std=#{std}"
  end

  d = File.dirname(__FILE__)
  [conf.cc, conf.cxx].each do |cc|
    # system
    cc.include_paths << File.join(d, 'vendor/glm')
    # graphics
    cc.include_paths << File.join(d, 'vendor/glfw/include')
    cc.include_paths << File.join(d, 'vendor/soil/include')
    cc.include_paths << File.join(d, 'vendor/sil/include')
    cc.include_paths << File.join(d, 'vendor/freetype-gl')
    cc.include_paths << File.join(d, 'vendor/nanovg/src')
    cc.include_paths << File.join(d, 'vendor/oui-blendish')
    # audio
    cc.include_paths << File.join(d, 'vendor/gorilla-audio/include')
  end

  conf.linker do |l|
    linker.library_paths << File.join(d, 'vendor/freetype-gl/build')
    linker.library_paths << File.join(d, 'vendor/glfw/build/src')
    linker.library_paths << File.join(d, 'vendor/gorilla-audio/build/build')
    linker.library_paths << File.join(d, 'vendor/nanovg/build')
    linker.library_paths << File.join(d, 'vendor/sil/build')
    linker.library_paths << File.join(d, 'vendor/soil/build')
    linker.libraries << 'glfw'
    linker.libraries << 'freetype-gl'
    linker.libraries << 'gorilla'
    linker.libraries << 'nanovg'
    linker.libraries << 'freetype'
    linker.libraries << 'SOIL'
    linker.libraries << 'SIL'
    linker.libraries << 'GL'
    linker.libraries << 'openal'
    linker.libraries << 'pthread'
  end
end
