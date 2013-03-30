/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once
#include <tuple>
#include <map>
#include <mxcomp/tuple_ext.h>
namespace mxcomp {
  namespace tuples {
  /* -------------------- MappedTuple_ -------------------- */
  template <typename K, typename KC, typename... T>
    struct MappedTuple_  {
      std::map<K, int> map;
      std::tuple<T...> data;
      KC kc;
     
      template <typename Tn>
      void operator()(unsigned int idx, Tn& t){
	map[ kc(t) ] = idx; 
      }
    
    MappedTuple_(KC f, T... t) : kc(f), data(t...) {
      
      iterate_i(*this, data);
    }
 
    MappedTuple_(T... t) : data(t...) {
      iterate_i(*this, data);
    }
      
      template <typename E>
      void operator()(const K& key, E& e){
	run( map[key], e, data);
      }
     
      template <typename E>
      auto findAndRun(const K& key, const E& _e) -> void {
	E e(_e);
	mxcomp::tuples::run(e, map[key], data);
      }

      template <typename E>
      auto get(const K& key, E& e) RETURN ( mxcomp::tuples::get(e, map[key], *this) )


    };

  }
}
