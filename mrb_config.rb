require_relative 'lib/platform'

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

  conf.cxx do |c|
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
    c.flags << "-std=#{std}"
  end

  d = File.dirname(__FILE__)
  vd = File.expand_path('vendor', d)
  [conf.cc, conf.cxx].each do |c|
    c.flags << '-g3'

    if Platform.darwin?
      c.flags << '-DMOON_GL_GLFW'
    else
      c.flags << '-DMOON_GL_GLEW'
    end

    # If you want Moon to guess the GLSL shader versions to load, enable this
    # line, otherwise, you must set is_legacy
    #c.flags << '-DMOON_GUESS_SHADER_VERSION'

    # system
    c.include_paths << File.join(vd, 'glm')
    # graphics
    c.include_paths << File.join(vd, 'glfw/include')
    c.include_paths << File.join(vd, 'soil/include')
    c.include_paths << File.join(vd, 'sil/include')
    c.include_paths << File.join(vd, 'freetype-gl')
    c.include_paths << File.join(vd, 'nanovg/src')
    # audio
    c.include_paths << File.join(vd, 'gorilla-audio/include')
  end

  conf.linker do |l|
    l.library_paths << File.join(vd, 'glfw/build/src')
    l.library_paths << File.join(vd, 'freetype-gl/build')
    l.library_paths << File.join(vd, 'gorilla-audio/build/build')
    l.library_paths << File.join(vd, 'nanovg/build')
    l.library_paths << File.join(vd, 'sil/build')
    l.library_paths << File.join(vd, 'soil/build')
    l.libraries << 'glfw'
    l.libraries << 'freetype-gl'
    l.libraries << 'gorilla'
    l.libraries << 'nanovg'
    l.libraries << 'freetype'
    l.libraries << 'SOIL'
    l.libraries << 'SIL'
    if Platform.darwin?
      l.flags_after_libraries << '-framework OpenGL'
      l.flags_after_libraries << '-framework OpenAL'
      l.flags_after_libraries << '-framework CoreFoundation'
    else
      l.libraries << 'GL'
      l.libraries << 'openal'
    end
    l.libraries << 'pthread'
  end
end
