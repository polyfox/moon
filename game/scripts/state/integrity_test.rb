class State::IntegrityTest < State

  def init
    abort "Moon::Sprite was not defined" unless Moon.const_defined?(:Sprite)
    abort "Moon::Tone was not defined" unless Moon.const_defined?(:Tone)
    abort "Moon::Color was not defined" unless Moon.const_defined?(:Color)
    super
    State.pop
  end

end