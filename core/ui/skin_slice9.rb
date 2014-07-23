module Moon
  class SkinSlice9 < RenderContainer
    attr_accessor :windowskin # Spritesheet

    def initialize
      super
      @windowskin = nil
    end

    def render(ix=0, iy=0, iz=0, options={})
      if @windowskin
        x, y, z = *(@position + [ix, iy, iz])
        cw, ch = @windowskin.cell_width, @windowskin.cell_height

        # render the windowskin (background)
        (width/cw).to_i.times do |w|
          (height/ch).to_i.times do |h|
            @windowskin.render x+w*cw, y+h*ch, z, 4
          end
        end
        # edges (top/bottom)
        (width/cw).to_i.times do |w|
          @windowskin.render x+w*cw, y, z, 1
          @windowskin.render x+w*cw, y+height-ch, z, 7
        end
        # edges (left/right)
        (height/ch).to_i.times do |h|
          @windowskin.render x, y+h*ch, z, 3
          @windowskin.render x+width-cw, y+h*ch, z, 5
        end
        # corners
        @windowskin.render x, y, z, 0
        @windowskin.render x+width-cw, y, z, 2
        @windowskin.render x, y+height-ch, z, 6
        @windowskin.render x+width-cw, y+height-ch, z, 8
      end
      super
    end
  end
end
