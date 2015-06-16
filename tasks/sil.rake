namespace :sil do
  dir = File.expand_path('../vendor/sil', File.dirname(__FILE__))

  task :clean do
    sh %(make -C "#{dir}/build" clean)
  end

  task :hard_clean do
    FileUtils.rm_rf(File.join(dir, 'build'))
  end
end
