namespace :'gorilla-audio' do
  dir = File.expand_path('../vendor/gorilla-audio', File.dirname(__FILE__))

  task :build do
    sh %(cd "#{dir}" &&
      mkdir -p build/build &&
      cd build/build &&
      (cmake .. && make -j 4))
  end

  task :clean do
    sh %(make -C "#{dir}/build/build" clean)
  end

  task :hard_clean do
    FileUtils.rm_rf(File.join(dir, 'build/build'))
  end
end
