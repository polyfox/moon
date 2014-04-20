TODO
====

## Urgent

* mousepos events, and clicking events need to be sorted out

## The regular
* Tone rgbs instead of hsl

  Yup, r,g,b,s components (red, green, blue, saturation)
  instead of h,s,l (hue, saturation, lumin-whatever)

* Font#draw_text outline_color

  Outline Color is currently fixed to 0, 0, 0, 0.1 I think

* Texture compositing, and drawing functions

  Big Topic, somehow, some manner of texture compositing.

* make Camera class, wrapping the view_matrix

* State#update(delta)

* Screen#resize or Screen#width, Screen#height (using glfwSetWindowSize)

  Register a callback with glfwSetWindowSizeCallback that then resets the viewport matrix.

* A Engine#debug flag, that turns debugging features on/off (for instance FPS in titlebar)

## IceDragon's wishlist! :D
* Sprite.scale_x/y
* Spritesheet.opacity, or should I just change to using Sprite ;-; ?
  Spritesheet is pretty nice for rendering tilemaps since we don't have to
  regenerate the vbo, it would be nice to have some opacity though.
* Kill those camel cases in the core/ !
* More UI goodness?
* ~~Sprite.shader~~ Postponed until further notice


## Platform related
- Android (after it doesn't stab you in the foot)