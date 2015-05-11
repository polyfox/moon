namespace :glfw do
  dir = File.expand_path('../vendor/glfw', File.dirname(__FILE__))

  task :build do
    sh %(cd #{dir}
      mkdir -p build &&
      cd build &&
      (cmake -DBUILD_SHARED_LIBS=true -DGLFW_BUILD_EXAMPLES=false .. &&
      make -j 4))
  end

  task :clean do
    sh %(make -C "#{dir}/build" clean)
  end

  task :hard_clean do
    FileUtils.rm_rf(File.join(dir, 'build'))
  end
end
