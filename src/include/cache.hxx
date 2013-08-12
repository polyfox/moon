#ifndef MOON_CACHE_H
#define MOON_CACHE_H

#include <memory>
#include <unordered_map>
#include <algorithm> // std::move

/* 
    Cache
    
    A standard cache class template. To make a class use it, publicly
    inherit from Cache<myclass>. A load function is provided, which
    either loads an object from cache, or constructs a new instance.
    All of the parameters given to the load method will be forwarded
    to the constructor.

    It is recommended, that the constructor of the class is private,
    that way we can only get instances via the load method, meaning
    all of the objects will be cached. The cache class will need to
    be added as a friend, to be allowed to construct it though.

    The template also takes a second argument, specifying the key type
    (std::string by default). The parameters passed to load are used
    to construct the key.

    Note that each individual class gets it's own cache store, since
    each individual class derives from a specialized Cache class (we
    derive from Cache<Texture>, not Cache). This decreases the risk
    of potential key collisions if we used two different key schemes.

*/

namespace Moon {

  template <class Object, class Key = std::string>
  class Cache {
  public:
    Cache(Key const& key) {
      //printf("added %s to cache\n", key.c_str());
      this->_key = key;
      //Cache::_cache[key] = this->shared_from_this(); // Note: override previous resource of same key, if any
      // this doesn't work because the object wasn't constructed yet and at least one shared_ptr must exist
    };

    virtual ~Cache() {
      //printf("removed %s from cache\n", _key.c_str());
      Cache::_cache.erase(_key);
    };

    Cache(Cache<Object> const&) = delete;
    Cache& operator=(Cache<Object> const&) = delete;

    static std::shared_ptr<Object> get(Key const& key) {
      auto const it = _cache.find(key);
      if (it == _cache.end()) { return std::shared_ptr<Object>(); }

      return std::static_pointer_cast<Object>(it->second.lock());
    };

    template <typename... Args>
    static std::shared_ptr<Object> load(Args&&... args) {
      Key key(std::forward<Args>(args)...);
      auto ptr = std::move(get(key)); // move it into the new pointer, so we don't copy construct

      if (ptr) return ptr; // cache hit

      ptr = std::move(std::shared_ptr<Object>(new Object(std::forward<Args>(args)...))); // make_shared is better than this

      Cache::_cache[key] = ptr;
      return ptr;
    };
  protected:
    Key _key;
    static std::unordered_map<Key, std::weak_ptr<Cache<Object, Key>>> _cache;
  };
  template <class Object, class Key>
  std::unordered_map<Key, std::weak_ptr<Cache<Object, Key>>> Cache<Object, Key>::_cache;

}


/* boost::hash_combine */
template <class T>
inline void hash_combine(std::size_t & seed, const T & v) {
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

/* hash specialization for std::pair (needed for (unordered_)map with std::pair keys) */
namespace std {
  template<typename S, typename T>
  struct hash<pair<S, T>> {
    inline size_t operator()(const pair<S, T> & v) const {
      size_t seed = 0;
      ::hash_combine(seed, v.first);
      ::hash_combine(seed, v.second);
      return seed;
    }
  };
}
#endif
