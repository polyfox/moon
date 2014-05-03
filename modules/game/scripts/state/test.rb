#
# core/state_test.rb
#   Countless random little tests, this file changes often
class State::Test < State

  def init
    super
    @sprites = []
    @font = Moon::Font.new("resources/fonts/ipaexg00201/ipaexg.ttf", 16);
    puts "Bounds: #{@font.calc_bounds("Sphinx of black quartz, judge my vow.")}"
    #@sound = Sound.new("resources/startup.wav", "wav")
    #@sound.play
    #Music.play("resources/CamelsNommingHay.ogg", "ogg")
    #@s = Spritesheet.new("resources/hyptosis_tile-art-batch-1.png", 32, 32)
    # 6 FPS at 15000 items
    # 55 FPS at 1500 items
    #1500.times do |i|
      sprite = Moon::Sprite.new("resources/obama_sprite.png")
      #sprite.x = rand(640)
      #sprite.y = rand(480)
      #sprite.z = -(i-1500) / 1500
      #sprite.tone.hue = (rand(9000)/1000)
      #sprite.opacity = (rand(1000)/1000)
      @sprites << sprite
    #end
#=end
  end

  def update(delta)
    #p "SPACE is PRESSED!" if Input.pressed?(Input::Keys::SPACE)
    #p "Mouse is in area!" if Input::Mouse.in_area?(0, 0, 32, 32)
    #p Input::Mouse.pos
    super delta
  end

  def render
    #for i in 0..1800 # runs efficiently at 60FPS, 18000 runs at 20FPS
    #  @s.render(i, i, i)
    #end
    #@s.render(10, 10, 0, 5)
    @sprites.each {|sprite| sprite.render(0, 0, 1) }
    @font.render(0, 0, 1, "Sphinx of black quartz, judge my vow.")
    @font.render(64,128, 1, "日本国 音読みと中国語")
    super
  end

end