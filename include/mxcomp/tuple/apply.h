/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once

#include <functional>
#include <tuple>
#include <mxcomp/range.h>
namespace mxcomp {
  namespace tuples {

    // This code is mostly adapted from http://preney.ca/paul/archives/934 
    template < typename F, typename Tuple,  size_t... N>
      static inline auto applyTuple( F&& f, Tuple&& t, const indices<N...>& ) 
      RETURN( f( std::get<N>(std::forward<Tuple>(t) )... ));
  

    template < typename T, typename F, typename Tuple,  size_t... N>
      static inline auto applyTuple(T* obj, F&& f, Tuple&& t, const indices<N...>& ) 
      RETURN( (obj->*f)( (std::get<N>(std::forward<Tuple>(t)))... ));

    template < typename F, typename Tuple>
      static inline auto applyTuple( F&& f, Tuple&& t ) 
      RETURN( applyTuple(std::forward<F>(f), std::forward<Tuple>(t), 
			 typename range<0, std::tuple_size<typename std::remove_reference<Tuple>::type>::value>::type() ));  

    template < typename T, typename F, typename Tuple>
      static inline auto applyTuple(T* obj, F&& f, Tuple&& t ) 
      RETURN( applyTuple(obj, std::forward<F>(f), std::forward<Tuple>(t), 
			 typename range<0, std::tuple_size< typename std::remove_reference<Tuple>::type >::value>::type() ));

  
  } 
}
