namespace :soil do
  dir = File.expand_path('../vendor/soil', File.dirname(__FILE__))

  task :build_soil do
    sh %(cd "#{dir}" &&
      (premake4 gmake &&
        cd build &&
        make -j 4 SOIL))
  end

  task build: ['sil:build', :build_soil]

  task :clean do
    sh %(make -C "#{dir}/build" clean)
  end
end
