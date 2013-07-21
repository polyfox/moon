class State_ModifierTest < State
  
  class Table

    def initialize(xsize, ysize)
      @xsize = xsize
      @ysize = ysize
      @data = Array.new(@ysize) { Array.new(@xsize, 0) }
    end

    def [](x, y)
      @data[y][x] || 0
    end

    def []=(x, y, n)
      @data[y][x] = n
    end

    def fill(n)
      for y in 0...@ysize
        for x in 0...@xsize
          @data[y][x] = n
        end
      end
    end

    def clear
      fill(0)
    end

    def row(y)
      @data[y]
    end

    def row_count
      @data.size
    end

    def to_s
      result = ""
      for y in 0...@ysize
        result.concat(@data[y].join(", "))
        result.concat("\n")
      end
      return result
    end

  end

  CNULL  = 0
  CFALSE = 0
  CTRUE  = 1

  BUTTONS = [
    Input::Mouse::Buttons::BUTTON_1,
    Input::Mouse::Buttons::BUTTON_2,
    Input::Mouse::Buttons::BUTTON_3,
    Input::Mouse::Buttons::BUTTON_4,
    Input::Mouse::Buttons::BUTTON_5,
    Input::Mouse::Buttons::BUTTON_6,
    Input::Mouse::Buttons::BUTTON_7,
    Input::Mouse::Buttons::BUTTON_8
  ]

  MODS = [CNULL, Input::Keys::MOD_SHIFT, Input::Keys::MOD_CONTROL, 
                 Input::Keys::MOD_ALT, Input::Keys::MOD_SUPER]

  def init
    init_data_table
    init_spriteset
    super
  end

  def init_data_table
    @data_table = Table.new(BUTTONS.size, MODS.size)
  end

  def init_spriteset
    @spritesheet = Spritesheet.new("resources/media_buttons_32x32.png", 32, 32)
  end

  def render
    super
    for y in 0...@data_table.row_count
      @data_table.row(y).each_with_index do |state, i|
        @spritesheet.render(i * 32, y * 32, y % 4) if state == CTRUE
      end
    end
  end

  def update
    MODS.each_with_index do |mod, y|
      BUTTONS.each_with_index do |button, x|
        if mod != CNULL
          #puts format(">> Setting button=%d with mod=%d", button, mod)
          @data_table[x, y] = Input::Mouse.pressed?(button, mod) ? CTRUE : CFALSE
        else
          #puts format(">> Setting button=%d", button)
          @data_table[x, y] = Input::Mouse.pressed?(button) ? CTRUE : CFALSE
        end
      end
    end

    # console debug
    #puts "      | L, R, M"
    #puts "N/A   | " + @data_table.row(0).join(", ")
    #puts "SHIFT | " + @data_table.row(1).join(", ")
    #puts "CTRL  | " + @data_table.row(2).join(", ")
    #puts "ALT   | " + @data_table.row(3).join(", ")
    #puts "SUPER | " + @data_table.row(4).join(", ")
    render # needs to be added to the C++ side
    super
  end

end
