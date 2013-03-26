#pragma once
#include <tuple>
#include <map>
#include <mxcomp/tuple_ext.h>
namespace tupleExt {
  /* -------------------- MappedTuple_ -------------------- */
  template <typename K, typename KC, typename... T>
    struct MappedTuple_ : public std::tuple<T...> {
    std::map<K, int> map;
    KC kc;
     
    template <typename Tn>
      void operator()(unsigned int idx, Tn& t){
      map[ kc(t) ] = idx; 
    }
     
  MappedTuple_(KC f, T... t) : kc(f), std::tuple<T...>(t...) {
      iterate_i(*this, *this);
    }
 
  MappedTuple_(T... t) : std::tuple<T...>(t...) {
      iterate_i(*this, *this);
    }

    template <typename E>
      void operator()(const K& key, E& e){
      run( map[key], e, *this);
    }
     
    template <typename E>
      auto findAndRun(const K& key, const E& _e) -> void {
      E e(_e);
      run(e, map[key], *this);
    }

    template <typename E>
      auto get(const K& key, E& e) RETURN ( tupleExt::get(e, map[key], *this) )


  };
  /* -------------------- /MappedTuple_ -------------------- */
}
