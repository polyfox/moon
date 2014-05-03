class State::MouseEvents < State

  include Moon

  def init
    #@observer = Observer.new
    @window = Window.new(32,32,128,128)

    button = Button.new(@window, 0, 0, "Test") { puts "button click'd!" }

    @window.on :click do |event|
      puts "window clicked with ALT!" if event.altKey
    end

    @window.on :dblclick do |event|
      puts "double clicked!"
    end

    #@observer.on :any do |event|
    #  if Moon::Input::Mouse.in_rect? @window
    #    @window.trigger event
    #  end
    #end
    super
  end

  def update(delta)
    @observer.update delta
    @window.update delta
    super delta
  end

  def render
    @window.render
    super
  end

end
