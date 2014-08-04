module Moon
  class SkinSlice3 < RenderContainer
    attr_accessor :horz       # Boolean
    attr_accessor :windowskin # Spritesheet

    def initialize
      super
      @horz = true
      @windowskin = nil
    end

    def render(ix=0, iy=0, iz=0, options={})
      if @windowskin
        x, y, z = *(@position + [ix, iy, iz])
        cw, ch = @windowskin.cell_width, @windowskin.cell_height

        if @horz
          @windowskin.render x, y, z, 0
          ((width/cw).to_i-2).times do |w|
            @windowskin.render x+(w+1)*cw, y, z, 1
          end
          @windowskin.render x+width-cw, y, z, 2
        else
          @windowskin.render x, y, z, 0
          ((height/ch).to_i-2).times do |h|
            @windowskin.render x, y+(h+1)*ch, z, 1
          end
          @windowskin.render x, y+height-ch, z, 2
        end
      end
      super
    end
  end
end
