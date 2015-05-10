namespace :'freetype-gl' do
  dir = File.expand_path('../vendor/freetype-gl', File.dirname(__FILE__))

  task :build do
    sh %(cd "#{dir}" && mkdir -p build && cd build &&
      cmake -Dfreetype-gl_BUILD_DEMOS=false -Dfreetype-gl_BUILD_APIDOC=false .. &&
      make -j 4)
  end

  task :clean do
    sh %(make -C "#{dir}/build" clean)
  end
end
