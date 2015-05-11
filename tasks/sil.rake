namespace :sil do
  dir = File.expand_path('../vendor/sil', File.dirname(__FILE__))

  task :build do
    sh %(cd "#{dir}" &&
      (premake4 gmake &&
        cd build &&
        make -j 4 SIL))
  end

  task :clean do
    sh %(make -C "#{dir}/build" clean)
  end

  task :hard_clean do
    FileUtils.rm_rf(File.join(dir, 'build'))
  end
end
