#ifndef MOON_TEXTURE_H
#define MOON_TEXTURE_H

#include "moon.hxx"
#include <memory>
#include <unordered_map>

namespace Moon {

  struct Rect {
    GLfloat x;
    GLfloat y;
    GLfloat w;
    GLfloat h;
  };

  struct VertexPos {
    GLfloat x;
    GLfloat y;
  };

  struct uvMap {
    GLfloat u;
    GLfloat v;
  };

  struct VertexData2D {
    GLfloat x;
    GLfloat y;
    GLfloat u;
    GLfloat v;
  };

  /* Thankfully, deleting the copy constructor on superclass deletes them
    on the default subclass ones too. 

     Adrinael │ geordi: { C c; C d(c); } struct B { B(const B&) = delete; };  struct C: B { };  // example one
       geordi │ error: use of deleted function 'C::C()'
     Adrinael │ geordi: { C c; C d(c); } struct B { B(const B&) = delete; B() {} };  struct C: B { C(const C&) : B() {}  C() {} };  // and how to get past that
       geordi │ <no output>
     Adrinael │ Damn, example one was faulty
        Speed │ ah, thank you :)
     Adrinael │ geordi: { C c; C d(c); } struct B { B(const B&) = delete; };  struct C: B { C() {} };  // example one
       geordi │ error: no matching function for call to 'B::B()'                                                     
        Speed │ (wow awesome bot)
     Adrinael │ geordi: { C c; C d(c); } struct B { B(const B&) = delete; B() {} };  struct C: B { C() {} };  // example one
       geordi │ error: use of deleted function 'C::C(const C&)'
     Adrinael │ There!
     Adrinael │ The default copy ctor will call base class's copy ctor, so in effect it's deleted

    */

  /* you need to provide a load method */
  class CacheObject: public std::enable_shared_from_this<CacheObject> {
  public:
    // std::shared_ptr<CacheObject> shared_from_this(); -- inherited

    CacheObject(std::string const& key);
    virtual ~CacheObject();

    CacheObject(CacheObject const&) = delete;
    CacheObject& operator=(CacheObject const&) = delete;

    //std::shared_ptr<CacheObject> get(std::string const& key) const;
  protected:
    std::string _key;
    static std::unordered_map<std::string, std::weak_ptr<CacheObject>> _cache;
  };

  class Texture: public CacheObject {
  public:
    ~Texture();

    static std::shared_ptr<Texture> load(std::string filename);
    void render(const GLfloat &x, const GLfloat &y, Rect *clip=NULL);
    void render(const GLfloat &x, const GLfloat &y, const GLuint &vboID, const GLuint &iboID);

    GLuint width();
    GLuint height();
    GLuint id();
  private:
    Texture(std::string filename);

    GLuint mTextureID;
    GLuint mTextureWidth;
    GLuint mTextureHeight;

    GLuint mVBOID;
    GLuint mIBOID;
  };

};

#endif