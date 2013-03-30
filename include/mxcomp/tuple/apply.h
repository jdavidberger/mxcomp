/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once

#include <functional>
#include <tuple>
#include <mxcomp/range.h>
namespace mxcomp {
  namespace tuples {

  template < typename F, typename Tuple,  size_t... N>
    static inline auto applyTuple( F&& f, Tuple& t, const indices<N...>& ) 
    RETURN( f( std::get<N>(t)... ));
  
  template < typename F, typename Tuple>
    static inline auto applyTuple( F&& f, Tuple& t ) 
    RETURN( applyTuple(f, t, typename range<0, std::tuple_size<Tuple>::value>::type() ));

  template < typename T, typename F, typename Tuple,  size_t... N>
    static inline auto applyTuple(T* obj, F&& f, Tuple& t, const indices<N...>& ) 
    RETURN( (obj->*f)( (std::get<N>(t))... ));
  
  template < typename T, typename F, typename Tuple>
    static inline auto applyTuple(T* obj, F&& f, Tuple& t ) 
    RETURN( applyTuple(obj, f, t, typename range<0, std::tuple_size<Tuple>::value>::type() ));

  

  /** 
      The original concept for the tuple functionality below was largely found at http://stackoverflow.com/a/1547118. 
      
      It recursively unrolls a tuple, and applies it to function. 
      So basically if you have:
      
      tuple<Arg1, Arg2, Arg3, Arg4> tuple;

      applyTuple(t, &T::f, tuple)
      is equivalent to
      t->f( get<0>(tuple), get<1>(tuple), get<2>(tuple), get<3>(tuple));

      There are a few variations below, namely for return types and static functions. This should all probably 
      be part of the standard. 

      The modifications from the original posting were to change all the types to reference types, and add some overloads. 
  */
  template < size_t N >
    struct apply {
      template < typename F, typename... ArgsT, typename... Args >
	static inline auto applyTuple(const F& f,
					  std::tuple<ArgsT...>& t,
					  Args&... args ) 
	RETURN(apply<N-1>::applyTuple( f, t, std::get<N-1>( t ), args... ));

      template < typename T, typename F, typename... ArgsT, typename... Args >
	static inline auto applyTuple_obj(T* pObj, 
					      const F& f,
					      std::tuple<ArgsT...>& t,
					      Args&... args )
	RETURN(apply<N-1>::applyTuple_obj( pObj, f, t, std::get<N-1>( t ), args... ));

    };

  /**
     This is the specialization of apply which marks the termination of the recursive expansion.
     \sa apply
  */
  template <>
    struct apply<0> {
    template < typename F, typename... ArgsT, typename... Args >
      static inline auto applyTuple(const F& f,
					std::tuple<ArgsT...>& /* t */,
					Args&... args )
      RETURN( f (args... ) );

    template < typename T, typename F, typename... ArgsT, typename... Args >
      static inline auto applyTuple_obj( T* pObj,
					     const F& f,
					     std::tuple<ArgsT...>&,
					     Args&... args )
      RETURN( (pObj->*f)( args... ) );
  };

  /*
  template < typename F, typename... ArgsT >
    static inline auto applyTuple( const F& f,
				   std::tuple<ArgsT...>& t ) 
    RETURN(apply<sizeof...(ArgsT)>::applyTuple( f, t ));
  
  template < typename T, typename F, typename... ArgsT >
    static inline auto applyTuple( T* pObj, 
				   const F& f,
				   std::tuple<ArgsT...>& t ) 
    RETURN(apply<sizeof...(ArgsT)>::applyTuple_obj(pObj, f, t ));
  */
} 
}
