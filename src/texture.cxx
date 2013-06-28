#include "texture.hxx"
/* DevIL */
#include <IL/il.h>
#include <IL/ilu.h>

namespace Moon {
  Texture::Texture(std::string filename)
  : CacheObject(filename)
  {

    //Texture loading success
    bool textureLoaded = false;

    //Generate and set current image ID
    ILuint imgID = 0;
    ilGenImages(1, &imgID);
    ilBindImage(imgID);

    //Load image
    ILboolean success = ilLoadImage(filename.c_str());

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
        glBindTexture(GL_TEXTURE_2D, 0);

        //Check for error
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
          printf("Error loading texture from %p pixels! glGetError: %s\n", pixels, error);
        } else {
          textureLoaded = true;

          //Vertex data
          VertexData2D vData[4];
          GLuint iData[4];

          //Set rendering indices
          iData[0] = 0;
          iData[1] = 1;
          iData[2] = 2;
          iData[3] = 3;

          //Create VBO
          glGenBuffers(1, &mVBOID);
          glBindBuffer(GL_ARRAY_BUFFER, mVBOID);
          glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData2D), vData, GL_DYNAMIC_DRAW);

          //Create IBO
          glGenBuffers(1, &mIBOID);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);

          //Unbind buffers
          glBindBuffer(GL_ARRAY_BUFFER, 0);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        };
      };

      //Delete file from memory
      ilDeleteImages( 1, &imgID );
    }

    //Report error
    if(!textureLoaded) {
      printf("Unable to load %s\n", filename.c_str());
      throw;
    }
  };

  Texture::~Texture() {
    //Delete texture
    if(mTextureID != 0) {
      glDeleteTextures(1, &mTextureID);
      mTextureID = 0;
    }
    //Free VBO and IBO
    if(mVBOID != 0) {
      glDeleteBuffers(1, &mVBOID);
      glDeleteBuffers(1, &mIBOID);
    }

    mTextureWidth = 0;
    mTextureHeight = 0;
  };

  void Texture::render(GLfloat x, GLfloat y, Rect *clip /*=NULL*/) {
    // If the texture exists
    if(mTextureID != 0) {
      //Remove any previous transformations
      glLoadIdentity();

      //Texture coordinates
      GLfloat texTop = 0.f;
      GLfloat texBottom = 1.f;
      GLfloat texLeft = 0.f;
      GLfloat texRight = 1.f;

      //Vertex coordinates
      GLfloat quadWidth = mTextureWidth;
      GLfloat quadHeight = mTextureHeight;

      //Handle clipping
      if(clip != NULL) {
        //Texture coordinates
        texLeft = clip->x / mTextureWidth;
        texRight = (clip->x + clip->w) / mTextureWidth;
        texTop = clip->y / mTextureHeight;
        texBottom = (clip->y + clip->h) / mTextureHeight;

        //Vertex coordinates
        quadWidth = clip->w;
        quadHeight = clip->h;
      }

      //Move to rendering point
      glTranslatef(x, y, 0.f);


      //Set vertex data
      VertexData2D vData[4];

      //Texture coordinates
      vData[0].u =  texLeft; vData[0].v =    texTop;
      vData[1].u = texRight; vData[1].v =    texTop;
      vData[2].u = texRight; vData[2].v = texBottom;
      vData[3].u =  texLeft; vData[3].v = texBottom;

      //Vertex positions
      vData[0].x =       0.f; vData[0].y =        0.f;
      vData[1].x = quadWidth; vData[1].y =        0.f;
      vData[2].x = quadWidth; vData[2].y = quadHeight;
      vData[3].x =       0.f; vData[3].y = quadHeight;

      //Set texture ID
      glBindTexture(GL_TEXTURE_2D, mTextureID);

      //Enable vertex and texture coordinate arrays
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        //Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, mVBOID);

        //Update vertex buffer data
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(VertexData2D), vData);

        //Set texture coordinate data
        glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData2D), (GLvoid*)offsetof(VertexData2D, u));

        //Set vertex data
        glVertexPointer(2, GL_FLOAT, sizeof(VertexData2D), (GLvoid*)offsetof(VertexData2D, x));

        //Draw quad using vertex data and index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);
        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);

      //Disable vertex and texture coordinate arrays
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glDisableClientState(GL_VERTEX_ARRAY);
    };
  };

  std::shared_ptr<Texture> Texture::load(std::string filename) {
    auto const it = CacheObject::_cache.find(filename);
    if (it == CacheObject::_cache.end()) {
      std::cout << "cache miss!" << std::endl;
      std::shared_ptr<Texture> ptr = std::shared_ptr<Texture>(new Texture(filename));
      CacheObject::_cache[filename] = ptr;
      return ptr;
    }
    std::cout << "cache hit!" << std::endl;
    return std::static_pointer_cast<Texture>(it->second.lock());
  }

  std::unordered_map<std::string, std::weak_ptr<CacheObject>> CacheObject::_cache;

  CacheObject::CacheObject(std::string const& key) {
    std::cout << "added " << key << " to cache" << std::endl;
    this->_key = key;
    //CacheObject::_cache[key] = this->shared_from_this(); // Note: override previous resource of same key, if any
    // this doesn't work because the object wasn't constructed yet and at least one shared_ptr must exist
  };

  CacheObject::~CacheObject() {
    std::cout << "removed " << _key << " from cache" << std::endl;
    CacheObject::_cache.erase(_key);
  };

  /*std::shared_ptr<CacheObject> CacheObject::get(std::string const& key) const {
    auto const it = _cache.find(key);
    if (it == _cache.end()) { return std::shared_ptr<CacheObject>(); }

    return it->second.lock();
  };*/

}