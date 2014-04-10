#
# moon/core/tone.rb
#
class Moon::Tone

  alias :h :hue
  alias :h= :hue=
  alias :s :saturation
  alias :s= :saturation=
  alias :l :lightness
  alias :l= :lightness=

  def dup
    Tone.new h, s, l
  end

end
