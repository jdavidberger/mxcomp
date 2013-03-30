/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once
#include <tuple>
#include <map>
#include <stdexcept>
#include <vector>
#include "metaprogramming.h"
#include <functional>
#include <mxcomp/tuple/apply.h>
#include <mxcomp/range.h>

namespace mxcomp {
  namespace tuples {
    static inline void noop(const std::initializer_list<int>&){}

    /* -------------------- iterate -------------------- */
    template <typename F, typename Tuple, size_t... N>
      inline auto iterate(F&& f, const Tuple& t, const indices<N...>&) -> void {
      // This merits an explanation:
      // The pack expansion semantics disallow use on the comma operator alone. 
      // Ie, we can't just do: f(std::get<N>(t))...;
      // even though intuitvely we'd expect the expansion to give:
      // f(std::get<0>(t)), f(std::get<1>(t)), ... f(std::get<N>(t))
      // which is valid. 
      //
      // However if we define a function to pass that expansion into it works. 
      // The new problem is that most of the applications of iterate are for
      // void functions, and you can't pass void as an argument. So what we do is
      // use the comma operator to evaluate our function call, discard it, and use 0. 
      noop({ (f(std::get<N>(t)),0)... });
    }

    template <typename F, typename Tuple>
      inline auto iterate(F&& f, const Tuple& t) -> void {
      iterate(std::forward<F>(f), t, typename range<0, std::tuple_size<Tuple>::value>::type());
    }

  /* -------------------- /iterate -------------------- */

  /* -------------------- iterate_ti -------------------- */
    template <typename F, typename Tuple, size_t... N>
      inline auto iterate_ti(F&& f, const Tuple& t, const indices<N...>&) -> void {
      noop( {(f.template operator()<N>(std::get<N>(t)), 0)...});
    }
    
    template <typename F, typename Tuple>
      inline auto iterate_ti(F&& f, const Tuple& t) -> void {
      iterate_ti(std::forward<F>(f), t, typename range<0, std::tuple_size<Tuple>::value>::type());
    }

  /* -------------------- /iterate_ti -------------------- */

  /* -------------------- iterate_i -------------------- */
    template <typename F, typename Tuple, size_t... N>
      inline auto iterate_i(F&& f, const Tuple& t, const indices<N...>&) -> void {
      noop( {(f(N, std::get<N>(t)), 0)...});
    }
    
    template <typename F, typename Tuple>
      inline auto iterate_i(F&& f, const Tuple& t) -> void {
      iterate_i(std::forward<F>(f), t, typename range<0, std::tuple_size<Tuple>::value>::type());
    }
    /* -------------------- /iterate_i -------------------- */

  /* -------------------- make_jumptable -------------------- */
  template <typename F, typename FT, std::size_t N, typename... Args>
    auto inline make_jumptable1(FT* jump[sizeof...(Args)]) 
    -> typename std::enable_if< N == sizeof...(Args), void>::type { }
 
  template <typename F, typename FT, std::size_t N, typename... Args>
    auto inline make_jumptable1(FT* jump[sizeof...(Args)]) 
    -> typename std::enable_if< N < sizeof...(Args), void>::type {
    jump[N] = [](F& f, const std::tuple<Args...>& t) { return f( std::get<N>(t)); };  
    make_jumptable1<F, FT, N+1, Args...>(jump);
  }

  template <typename F, typename Tuple, size_t N>
    auto inline jump_(F& f, const Tuple& t) 
    RETURN(f( std::get<N>(t)));

  template <typename F, typename FT, typename Tuple, size_t... N>
    auto inline make_jumptable(FT* jump[sizeof...(N)], const indices<N...>&) -> void {
    noop( {(jump[N] = jump_<F, Tuple, N>,0)...});
  }

  template <typename F, typename FT, typename... Args>
    auto inline make_jumptable(FT* jump[sizeof...(Args)]) -> void {
    make_jumptable<F, FT, std::tuple<Args...>>(jump, typename range<0, sizeof...(Args)>::type());
  }

  /* -------------------- /make_jumptable -------------------- */

  /* -------------------- run -------------------- */
  template <typename F, typename... Args>
    auto inline run( F&& f, const size_t i, const std::tuple<Args...>& t) -> void
  {
    using FT = void ( F&&, const std::tuple<Args...>& );
    static FT* jump[sizeof...(Args)];
    static bool init = false;
    if(!init){    
      init = true;
      make_jumptable<F, FT, Args...>(jump);
    }
    jump[i](f, t);
  }
  /* -------------------- /run -------------------- */

  /* -------------------- get -------------------- */
  template <typename F, typename... Args>
    auto inline get( F&& f, const size_t i, const std::tuple<Args...>& t) -> decltype(f(std::get<0>(t) ))
  {
    using FT = decltype(f( std::get<0>(t))) ( F&, const std::tuple<Args...>& );
    static FT* jump[sizeof...(Args)];
    static bool init = false;
    if(!init){    
      init = true;
      make_jumptable<F, FT, Args...>(jump);
    }
    return jump[i](f, t);
  }
  /* -------------------- /get -------------------- */

  /* -------------------- to_vector -------------------- */
  template <std::size_t N = 0, typename F, typename Rtn, typename... Args>
    static inline auto to_vector(const F& , const std::tuple<Args...>&, std::vector<Rtn>& ) 
    -> typename std::enable_if<N == sizeof...(Args), void>::type { }

  template <std::size_t N = 0, typename F, typename Rtn, typename... Args>
    static inline auto to_vector(F&& f, const std::tuple<Args...>& t, std::vector<Rtn>& vector) 
    -> typename std::enable_if<N < sizeof...(Args), void>::type
    {
      vector[N] = f(std::get<N>(t) );
      to_vector<N+1, F, Rtn, Args...>(f, t, vector);
    }
  
  template <typename F, typename... Args>
    static inline auto to_vector(F&& f, const std::tuple<Args...>& t) 
    -> std::vector< decltype(f(std::get<0>(t) )) > 
  {
    using Rtn = decltype(f(std::get<0>(t) ));
    std::vector<Rtn> vector;
    vector.resize(sizeof...(Args));
    to_vector<0, F, Rtn, Args...>(f, t, vector);
    return vector;
  }
  /* -------------------- /to_vector -------------------- */

  /* -------------------- fold -------------------- */
  template <std::size_t N = 0, typename F, typename Fold, typename... Args>
    inline auto fold(F&& , const std::tuple<Args...>&, const Fold& fold) -> typename std::enable_if<N == sizeof...(Args), Fold>::type { 
    return fold;
  }

  template <std::size_t N = 0, typename F, typename Fold, typename... Args>
    inline auto fold(F&& f, const std::tuple<Args...>& t, Fold&& fold_v) -> typename std::enable_if<N < sizeof...(Args), Fold>::type{
    fold_v = f(fold_v,  std::get<N>(t) );
    return fold<N+1, F, Fold, Args...>(std::forward<F>(f), t, std::forward<Fold>(fold_v));
  }
  /* -------------------- /fold -------------------- */
}
}
#include <mxcomp/tuple/list.h>
#include <mxcomp/tuple/map.h>
