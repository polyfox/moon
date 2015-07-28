# This is the mingw32 mruby toolchains, DO NOT REQUIRE THIS IN THE MOON Rakefile.

MRuby::Toolchain.new(:i686_w64_mingw32) do |conf|
  toolchain :gcc

  [conf.cc, conf.objc, conf.asm].each do |cc|
    cc.command = 'i686-w64-mingw32-gcc'
  end
  conf.cxx.command = 'i686-w64-mingw32-g++'
  conf.linker.command = 'i686-w64-mingw32-gcc'

  conf.archiver do |archiver|
    archiver.command = 'i686-w64-mingw32-ar'
  end

  conf.exts do |exts|
    exts.object = '.o'
    exts.executable = '.exe'
    exts.library = '.a'
  end
end

MRuby::Toolchain.new(:x86_64_w64_mingw32) do |conf|
  toolchain :i686_w64_mingw32

  [conf.cc, conf.objc, conf.asm].each do |cc|
    cc.command = 'x86_64-w64-mingw32-gcc'
  end
  conf.cxx.command = 'x86_64-w64-mingw32-g++'
  conf.linker.command = 'x86_64-w64-mingw32-gcc'
  conf.archiver do |archiver|
    archiver.command = 'x86_64-w64-mingw32-ar'
  end
end
