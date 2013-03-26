#pragma once

#include <functional>
#include <tuple>

namespace tupleExt {
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
  template < uint N >
    struct apply {
      template < typename... ArgsF, typename... ArgsT, typename... Args >
	static inline void applyTuple(const std::function<void ( ArgsF... )>& f,
				      std::tuple<ArgsT...>& t,
				      Args&... args ) {
	apply<N-1>::applyTuple( f, t, std::get<N-1>( t ), args... );
      }


      template < typename F, typename... ArgsT, typename... Args >
	static inline auto applyTuple_ret(const F& f,
					  std::tuple<ArgsT...>& t,
					  Args&... args ) 
	RETURN(apply<N-1>::applyTuple_ret( f, t, std::get<N-1>( t ), args... ));

      template < typename... ArgsF, typename RetF, typename... ArgsT, typename... Args >
	static inline RetF applyTuple_ret(const std::function<  RetF ( ArgsF... ) >& f,
					  std::tuple<ArgsT...>& t,
					  Args&... args ) {
	return apply<N-1>::applyTuple_ret( f, t, std::get<N-1>( t ), args... );
      }

      template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
	static inline void applyTuple_obj(T* pObj, 
					  const std::function< void (T&, ArgsF... ) >& f,
					  std::tuple<ArgsT...>& t,
					  Args&... args ) {
	apply<N-1>::applyTuple_obj( pObj, f, t, std::get<N-1>( t ), args... );
      }

      template < typename T, typename retF, typename... ArgsF, typename... ArgsT, typename... Args >
	static inline retF applyTuple_obj_ret(T* pObj, 
					      const std::function< retF ( ArgsF... ) >& f,
					      std::tuple<ArgsT...>& t,
					      Args&... args ) {
	return apply<N-1>::applyTuple_obj_ret( pObj, f, t, std::get<N-1>( t ), args... );
      }

    };

  /**
     This is the specialization of apply which marks the termination of the recursive expansion.
     \sa apply
  */
  template <>
    struct apply<0> {

    template < typename F, typename... ArgsT, typename... Args >
      static inline auto applyTuple_ret(const F& f,
					std::tuple<ArgsT...>& /* t */,
					Args&... args )
      RETURN( f (args... ) );

    template < typename Rtn, typename... ArgsF, typename... ArgsT, typename... Args >
      static inline Rtn applyTuple_ret(const std::function<Rtn ( ArgsF... ) >& f,
				       std::tuple<ArgsT...>& /* t */,
				       Args&... args )
    {
      return f( args... );
    }

    template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
      static inline void applyTuple_obj( T* pObj,
					 const std::function< void (T&, ArgsF... ) >& f,
					 std::tuple<ArgsT...>&,
					 Args&... args )
    {
      return f(*pObj, args... );
    }

    template < typename T, typename retF, typename... ArgsF, typename... ArgsT, typename... Args >
      static inline retF applyTuple_obj_ret( T* pObj,
					     const std::function< retF (T&, ArgsF... ) >& f,
					     std::tuple<ArgsT...>&,
					     Args&... args )
    {
      return f(*pObj, args... );
    }
  };

  // --- Non-members, non-return
  template < typename... ArgsF, typename... ArgsT >
    static inline void applyTuple( const std::function< void (ArgsF...)>& f,
				   std::tuple<ArgsT...>& t )
  {
    apply<sizeof...(ArgsT)>::applyTuple( f, t );
  }

  template < typename... ArgsF, typename... ArgsT >
    static inline void applyTuple( void (*f)(ArgsF...),
				   std::tuple<ArgsT...>& t )
  {
    applyTuple( std::function< void (ArgsF...) >(f), t );
  }

  // --- Non-members, non-return
  template < typename F, typename... ArgsT >
    static inline auto applyTuple( const F& f,
				  std::tuple<ArgsT...>& t ) 
    RETURN(apply<sizeof...(ArgsT)>::applyTuple_ret( f, t ))  

  template < typename... ArgsF, typename Rtn, typename... ArgsT >
    static inline Rtn applyTuple( const std::function<Rtn (ArgsF...)>& f,
				  std::tuple<ArgsT...>& t )
  {
    return apply<sizeof...(ArgsT)>::applyTuple_ret( f, t );
  }

  template < typename... ArgsF, typename Rtn, typename... ArgsT >
    static inline Rtn applyTuple( Rtn (*f)(ArgsF...),
				  std::tuple<ArgsT...>& t )
  {
    return applyTuple( std::function<Rtn (ArgsF...)>(f), t );
  }

  // --- Member versions, no return
  template < typename T, typename... ArgsF, typename... ArgsT >
    static inline void applyTuple( T* pObj, const std::function< void (T&, ArgsF...) >& f, std::tuple<ArgsT...>& t )
  {
    apply<sizeof...(ArgsT)>::applyTuple_obj(pObj, f, t );
  }

  template < typename T, typename... ArgsF, typename... ArgsT >
    static inline void applyTuple( T* pObj, void (T::*f)(ArgsF...), std::tuple<ArgsT...>& t )
  {
    applyTuple(pObj, std::function< void (T&, ArgsF...) >(f), t );
  }

  // --- Member versions, return
  template < typename T, typename retF, typename... ArgsF, typename... ArgsT >
    static inline retF applyTuple( T* pObj, const std::function<retF (T&, ArgsF...)>& f,
				   std::tuple<ArgsT...>& t )
  {
    return apply<sizeof...(ArgsT)>::applyTuple_obj_ret(pObj, f, t );
  }  

  template < typename T, typename retF, typename... ArgsF, typename... ArgsT >
    static inline retF applyTuple( T* pObj, retF (T::*f)(ArgsF...),
				   std::tuple<ArgsT...>& t )
  {
    return applyTuple(pObj, std::function< retF (T&, ArgsF...) >(f), t );
  }  

} 
