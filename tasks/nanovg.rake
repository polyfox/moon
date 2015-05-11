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

  task :hard_clean do
    FileUtils.rm_rf(File.join(dir, 'build'))
  end
end
