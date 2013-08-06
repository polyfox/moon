#include <memory>
#include <unordered_map>
#include <algorithm> // std::move

/* 
    Cache
    
    A standard cache class template. To make a class use it, publicly
    inherit from Cache<myclass>. You need to provide a `load` type of
    static public member function, which will either construct a new
    instance and add it to the cache or call a (private) constructor.

    For an example, see Texture::load.

    Note that each individual class gets it's own cache store, since
    each individual class derives from a specialized Cache class (we
    derive from Cache<Texture>, not Cache). This decreases the risk
    of potential key collisions if we used two different key schemes.

*/

namespace Moon {

  template <class Object>
  class Cache {
  public:
    Cache(std::string const& key) {
      printf("added %s to cache\n", key.c_str());
      this->_key = key;
      //Cache::_cache[key] = this->shared_from_this(); // Note: override previous resource of same key, if any
      // this doesn't work because the object wasn't constructed yet and at least one shared_ptr must exist
    };

    virtual ~Cache() {
      printf("removed %s from cache\n", _key.c_str());
      Cache::_cache.erase(_key);
    };

    Cache(Cache<Object> const&) = delete;
    Cache& operator=(Cache<Object> const&) = delete;

    static std::shared_ptr<Object> get(std::string const& key) {
      auto const it = _cache.find(key);
      if (it == _cache.end()) { return std::shared_ptr<Object>(); }

      return std::static_pointer_cast<Object>(it->second.lock());
    };
  protected:
    std::string _key;
    static std::unordered_map<std::string, std::weak_ptr<Cache<Object>>> _cache;
  };
  template <class Object>
  std::unordered_map<std::string, std::weak_ptr<Cache<Object>>> Cache<Object>::_cache;

}
