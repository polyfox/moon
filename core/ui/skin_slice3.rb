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
          (width/cw).to_i.times do |w|
            @windowskin.render x+w*cw, y, z, 1
            @windowskin.render x+w*cw, y+height-ch, z, 7
          end
        else
          (height/ch).to_i.times do |h|
            @windowskin.render x, y+h*ch, z, 3
            @windowskin.render x+width-cw, y+h*ch, z, 5
          end
        end
      end
      super
    end
  end
end
