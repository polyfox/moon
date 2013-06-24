#include "texture.hxx"

namespace Moon {
  Texture::Texture(std::string path) {
    //Texture loading success
    bool textureLoaded = false;

    //Generate and set current image ID
    ILuint imgID = 0;
    ilGenImages(1, &imgID);
    ilBindImage(imgID);

    //Load image
    ILboolean success = ilLoadImage(path.c_str());

    //Image loaded successfully
    if(success == IL_TRUE) {
      //Convert image to RGBA
      success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
      if(success == IL_TRUE) {
        //Create texture from file pixels

        GLuint* pixels = (GLuint*)ilGetData();
        mTextureWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
        mTextureHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

        //Generate texture ID
        glGenTextures(1, &mTextureID);
        glBindTexture(GL_TEXTURE_2D, mTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, NULL);

        //Check for error
        GLenum error = glGetError();
        if( error != GL_NO_ERROR ) {
          printf("Error loading texture from %p pixels! glGetError: %s\n", pixels, error);
        } else {
          textureLoaded = true;
        };
      }

      //Delete file from memory
      ilDeleteImages( 1, &imgID );
    }

    //Report error
    if(!textureLoaded) {
      printf("Unable to load %s\n", path.c_str());
      throw;
    }

  };

  Texture::~Texture() {
    //Delete texture
    if(mTextureID != 0) {
      glDeleteTextures(1, &mTextureID);
      mTextureID = 0;
    }

    mTextureWidth = 0;
    mTextureHeight = 0;
  };

  void Texture::render(GLfloat x, GLfloat y){
    // If the texture exists
    if( mTextureID != 0 ) {
        // Remove any previous transformations
        glLoadIdentity();
        //Move to rendering point
        glTranslatef(x, y, 0.f);

        glBindTexture( GL_TEXTURE_2D, mTextureID);
        //Render textured quad
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f,            0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( mTextureWidth,            0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( mTextureWidth, mTextureHeight );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f, mTextureHeight );
        glEnd();
    };
  };

}