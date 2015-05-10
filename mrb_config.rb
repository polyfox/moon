MRuby::Build.new do |conf|
  toolchain_name = (ENV['MOON_MRUBY_TOOLCHAIN'] || :gcc).to_sym
  toolchain toolchain_name

  puts "\t\\\\ Using #{toolchain_name} Toolchain \\\\"

  # Full backtraces
  enable_debug

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
  conf.gem github: 'IceDragon200/mruby-glew'     # GLEW
  conf.gem github: 'IceDragon200/mruby-glfw3'    # GLFW
  conf.gem github: 'IceDragon200/mruby-nanovg'   # nanovg

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
  vd = File.expand_path('vendor', d)
  [conf.cc, conf.cxx].each do |cc|
    # system
    cc.include_paths << File.join(vd, 'glm')
    # graphics
    cc.include_paths << File.join(vd, 'glfw/include')
    cc.include_paths << File.join(vd, 'soil/include')
    cc.include_paths << File.join(vd, 'sil/include')
    cc.include_paths << File.join(vd, 'freetype-gl')
    cc.include_paths << File.join(vd, 'nanovg/src')
    # audio
    cc.include_paths << File.join(vd, 'gorilla-audio/include')
  end

  conf.linker do |l|
    linker.library_paths << File.join(vd, 'glfw/build/src')
    linker.library_paths << File.join(vd, 'freetype-gl/build')
    linker.library_paths << File.join(vd, 'gorilla-audio/build/build')
    linker.library_paths << File.join(vd, 'nanovg/build')
    linker.library_paths << File.join(vd, 'sil/build')
    linker.library_paths << File.join(vd, 'soil/build')
    linker.libraries << 'glfw'
    linker.libraries << 'freetype-gl'
    linker.libraries << 'gorilla'
    linker.libraries << 'nanovg'
    linker.libraries << 'freetype'
    linker.libraries << 'SOIL'
    linker.libraries << 'SIL'
    if RUBY_PLATFORM.include?('darwin')
      linker.flags_after_libraries << '-framework OpenGL'
      linker.flags_after_libraries << '-framework OpenAL'
      linker.flags_after_libraries << '-framework CoreFoundation'
    else
      linker.libraries << 'GL'
      linker.libraries << 'openal'
    end
    linker.libraries << 'pthread'
  end
end
