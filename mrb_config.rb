MRuby::Build.new do |conf|
  toolchain :gcc

  # include the default GEMs
  conf.gembox 'default'

  # Temporary custom solution. If we end up
  # creating a custom gem, make a gem folder
  # in moon root and put it there.
  conf.gem :github => 'archSeer/mruby-require'
end

# Define cross build settings
MRuby::CrossBuild.new('androideabi') do |conf|
  ENV['ANDROID_NDK_HOME'] = '/opt/android-ndk'
  ENV['ANDROID_TARGET_ARCH_ABI'] = 'armeabi-v7a'

  toolchain :androideabi
end
