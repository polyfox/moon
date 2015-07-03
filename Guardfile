require 'guard/plugin'

module ::Guard
  class MoonBuild < ::Guard::Plugin
    def exec_build(*args)
      system('bundle', 'exec', 'rake', *args)
    end

    def do_tests
      Guard::Notifier.notify 'Testing', title: 'moon', image: :pending
      if exec_build('--verbose', 'test')
        Guard::Notifier.notify 'Tests Passed', title: 'moon', image: :success
        true
      else
        Guard::Notifier.notify 'Tests Failed', title: 'moon', image: :failed
        false
      end
    end

    def do_build
      Guard::Notifier.notify 'Building', title: 'moon', image: :pending
      if exec_build('--verbose', 'build')
        Guard::Notifier.notify 'Build Successful', title: 'moon', image: :success
        true
      else
        Guard::Notifier.notify 'Build Failed', title: 'moon', image: :failed
        false
      end
    end

    def run_all
      do_build and do_tests
    end

    def run_on_changes(paths)
      do_build and do_tests
    end
  end
end

guard :moon_build do
  watch(/modules\/(.+)\/(?:src|include|tools)\/(.+\.(?:cpp|cxx|cc|c|hpp|hxx|hh|h))/)
  watch(/modules\/(.+)\/(?:mrblib|docstub)\/(.+\.rb)/)
  watch(/(.+\.rake)/)
  watch(/CMakeLists.txt/)
  watch(/mrb_config.rb/)
  watch(/Rakefile/)
  watch(/test\/(.+\.rb)/)
end
