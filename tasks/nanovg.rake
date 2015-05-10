namespace :nanovg do
  dir = File.expand_path('../vendor/nanovg', File.dirname(__FILE__))

  task :build_nanovg do
    sh %(cd "#{dir}" &&
      (premake4 gmake &&
        cd build &&
        make -j 4 nanovg))
  end

  task build: ['sil:build', :build_nanovg]

  task :clean do
    sh %(make -C "#{dir}/build" clean)
  end
end
