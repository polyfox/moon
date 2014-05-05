class State::IntegrityTest < State

  def init
    super
    [:Vector2, :Vector3, :Vector4, :Font, :Music, :Rect, :Screen,
     :Sound, :Sprite, :Spritesheet, :Texture, :Tone].each do |sym|
      if Moon.const_defined?(sym)
        puts "[INTEGRITY] @@ Moon::#{sym}"
      else
        puts "[INTEGRITY] !! Moon::#{sym}"
      end
    end
    State.pop
  end

end