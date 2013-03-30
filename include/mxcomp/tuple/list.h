#pragma once
#include <mxcomp/tuple/apply.h>

namespace mxcomp {
  namespace tuples {
  /** Append an item to the end of a tuple. */
  template <typename... Args, typename H>
    std::tuple<Args..., H> append(std::tuple<Args...>& lst, const H& el){    
    return applyTuple([el](Args&... args) { 
	return std::make_tuple(args..., el); 
      }, lst);
  }

  /** Get the head of a tuple */
  template <typename H, typename... Args>
    H& head(std::tuple<H, Args...>&& lst){
    return std::get<0>(lst);
  }

  /** Get the tail of a tuple */
  template <typename H, typename... Args>
    std::tuple<Args...> tail(std::tuple<H, Args...>& lst) {
    return applyTuple( [](H& h, Args&... args){ 
	return std::make_tuple(args...); 
      } , lst);
  }

  /** This structure helps us pull out all tuples of a given type.

      The int/longs are tacked on to the ends of the get functions to
      give the functions a precedence, so there are no ambiguities.
   */
  template <typename T>
    struct of_type {      
      
      /** Terminal condition -- when the 'in' tuple is blank our out tuple is
	  our resultant type */
      template <typename... Out> 
      static auto get(std::tuple<Out...> out, const std::tuple<>&, int) -> std::tuple<Out...> {
	return out;
      }

      /** This function is only visible when H is a desendent of T. 
	  You can change out 'is_base_of' for different filters. */
      template <typename... Out, typename H, typename... In> 
	static auto get(std::tuple<Out...> out, std::tuple<H, In...> in, int) 
	-> typename std::enable_if<std::is_base_of<T, H>::value, decltype(of_type<T>::get(append(out, head(in)), tail(in), 0))>::type 
      {
	return of_type<T>::get(append(out, head(in)), tail(in), 0);
      }

      /** This is the function that is inserted when the filter fails.
	  Notice that the 'head' of the in tuple is discarded. */
      template <typename... Out, typename H, typename... In> 
	static auto get(std::tuple<Out...> out, std::tuple<H, In...> in, long) 
	RETURN(of_type<T>::get(out, tail(in), 0) );
   
      /** Starter call to initiate the filter */
      template <typename H,typename... Args> static auto get(std::tuple<H, Args...>& tuple) 
	RETURN(of_type<T>::get( std::make_tuple(), tuple, 0));

    };

  /** Syntaxtic sugar to make the call a bit nicer. Calls the entry point to the above 
      structure.
   */
  template <typename T, typename... Args>
    static inline auto ofType(std::tuple<Args...>& lst) RETURN( of_type<T>::get(lst) )

}
}
