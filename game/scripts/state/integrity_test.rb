class State::IntegrityTest < State

  def init
    abort "Moon::Sprite was not defined" unless defined?(Moon::Sprite)
    abort "Moon::Tone was not defined" unless defined?(Moon::Tone)
    abort "Moon::Color was not defined" unless defined?(Moon::Color)
    super
    State.pop
  end

end