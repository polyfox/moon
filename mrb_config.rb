require 'set'
require_relative 'lib/platform'

toolchain_name = (ENV['MOON_MRUBY_TOOLCHAIN'] || :gcc).to_sym
platform = Platform.new

rootdir = File.dirname(__FILE__)
MRuby::Build.new 'host', File.expand_path("build", rootdir) do |conf|
  toolchain toolchain_name

  puts "\t\\\\ Using #{toolchain_name} Toolchain \\\\"

  # Full backtraces
  conf.enable_debug

  # include the default GEMs
  conf.gembox 'default'
  conf.gembox File.expand_path('modules/moon', rootdir)

  conf.gem core: 'mruby-eval'
  conf.gem core: 'mruby-bin-debugger'
  conf.gem github: 'IceDragon200/mruby-patch-core'
  conf.gem github: 'iij/mruby-io'                # General IO stuff, File system blah de dah.
  conf.gem github: 'iij/mruby-dir'               # Directory access
  conf.gem github: 'iij/mruby-regexp-pcre'       # Regular expression engine :3
  conf.gem github: 'iij/mruby-tempfile'          # required by iij/mruby-require
  conf.gem github: 'iij/mruby-require'           # require

  # nice things
  #conf.gem github: 'AndrewBelt/mruby-yaml'                          # YAML :3
  conf.gem github: 'IceDragon200/mruby-yaml', branch: 'args_req-fix' # YAML :3

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

  vd = File.expand_path('vendor', rootdir)
  bvd = File.expand_path('build/vendor', rootdir)
  puts "VendorDir: #{vd}"
  puts "Build.VendorDir: #{bvd}"
  [conf.cc, conf.cxx].each do |c|
    flags = Set.new(c.flags)
    # remove other optimization flags
    flags.delete("-O0")
    flags.delete("-O1")
    flags.delete("-O2")

    # enable debugging
    flags << '-g3'
    # optimize level 3
    flags << '-O3'

    # Its a good idea to get all the warnings
    flags << '-Wall'
    flags << '-Wextra'
    # shuts up those unusued-parameter warnings, trust me, you'll be swimming
    # in them from a mruby extension.
    flags << '-Wno-unused-parameter'

    c.defines << 'ENABLE_DEBUG'

    if platform.darwin?
      # GLFW
      c.defines << 'MOON_GL_GLFW'
    else
      # GLEW
      c.defines << 'MOON_GL_GLEW'
    end

    # enable mruby-yaml: null, Null and NULL
    flags << "-DMRUBY_YAML_NULL=1"
    # disable all the extra mruby-yaml aliases, this makes it more like ruby
    flags << "-DMRUBY_YAML_BOOLEAN_ON=0"
    flags << "-DMRUBY_YAML_BOOLEAN_YES=0"
    flags << "-DMRUBY_YAML_BOOLEAN_SHORTHAND_YES=0"
    flags << "-DMRUBY_YAML_BOOLEAN_OFF=0"
    flags << "-DMRUBY_YAML_BOOLEAN_NO=0"
    flags << "-DMRUBY_YAML_BOOLEAN_SHORTHAND_NO=0"

    # required system includes
    c.include_paths << File.expand_path('glm', vd)
    # required graphics includes
    c.include_paths << File.expand_path('glfw/include', vd)
    c.include_paths << File.expand_path('glfw/src', bvd) # has the glfw_config.h
    c.include_paths << File.expand_path('glfw/include', vd)
    c.include_paths << File.expand_path('soil/include', vd)
    c.include_paths << File.expand_path('sil/include', vd)
    c.include_paths << File.expand_path('freetype-gl', vd)
    # required audio includes
    c.include_paths << File.expand_path('gorilla-audio/include', vd)
    c.include_paths.uniq!

    c.flags = flags.to_a
  end

  conf.linker do |l|
    l.library_paths << File.expand_path('glfw/src', bvd)
    l.library_paths << File.expand_path('freetype-gl', bvd)
    l.library_paths << File.expand_path('gorilla-audio/build', bvd)
    l.library_paths << File.expand_path('sil', bvd)
    l.library_paths << File.expand_path('soil', bvd)
    l.library_paths.uniq!

    l.libraries << 'glfw'
    l.libraries << 'freetype-gl'
    l.libraries << 'gorilla'
    l.libraries << 'freetype'
    l.libraries << 'SOIL'
    l.libraries << 'SIL'

    if platform.linux?
      l.libraries << 'GLEW'
      l.libraries << 'GL'
      l.libraries << 'openal'
    elsif platform.windows?
      l.libraries << 'glew32'
      l.libraries << 'opengl32'
      l.libraries << 'OpenAL32'
    elsif platform.darwin?
      l.libraries << 'GLEW'
      l.flags_after_libraries << '-framework OpenGL'
      l.flags_after_libraries << '-framework OpenAL'
      l.flags_after_libraries << '-framework CoreFoundation'
    end

    if platform.unix?
      l.libraries << 'pthread'
    end

    puts "Linking these libraries:"
    l.libraries.each do |lib|
      puts "\t#{lib}"
    end
  end
end
