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

namespace tupleExt {
  /* -------------------- iterate -------------------- */
  template <std::size_t N = 0, typename F, typename... Args>
    inline auto iterate(const F& , const std::tuple<Args...>&) -> typename std::enable_if<N == sizeof...(Args), void>::type { }

  template <std::size_t N = 0, typename F, typename... Args>
    inline auto iterate(F& f, const std::tuple<Args...>& t) -> typename std::enable_if<N < sizeof...(Args), void>::type{
    f( std::get<N>(t) );
    iterate<N+1, F, Args...>(f, t);
  }

  /** Exists so you can make the functor in place. */
  template <std::size_t N = 0, typename F, typename... Args>
    inline auto iterate(const F& _f, const std::tuple<Args...>& t) -> typename std::enable_if<N == 0, void>::type {
    F f(_f);
    iterate<N, F, Args...>(f, t);
  }
  /* -------------------- /iterate -------------------- */

  /* -------------------- iterate_ti -------------------- */
  template <std::size_t N = 0, typename F, typename... Args>
    inline auto iterate_ti(const F& , const std::tuple<Args...>&) -> typename std::enable_if<N == sizeof...(Args), void>::type { }

  template <std::size_t N = 0, typename F, typename... Args>
    inline auto iterate_ti(F& f, const std::tuple<Args...>& t) -> typename std::enable_if<N < sizeof...(Args), void>::type{
    f.template operator()<N>(std::get<N>(t) );
    iterate_ti<N+1, F, Args...>(f, t);
  }
  
  /** Exists so you can make the functor in place. */
  template <std::size_t N = 0, typename F, typename... Args>
    inline auto iterate_ti(const F& _f, const std::tuple<Args...>& t) -> typename std::enable_if<N == 0, void>::type {
    F f(_f);
    iterate_ti<N, F, Args...>(f, t);
  }
  /* -------------------- /iterate_ti -------------------- */


  /* -------------------- iterate_i -------------------- */
  template <std::size_t N = 0, typename F, typename... Args>
    inline auto iterate_i(const F& , const std::tuple<Args...>&) -> typename std::enable_if<N == sizeof...(Args), void>::type { }

  template <std::size_t N = 0, typename F, typename... Args>
    inline auto iterate_i(F& f, const std::tuple<Args...>& t) -> typename std::enable_if<N < sizeof...(Args), void>::type{
    f(N,  std::get<N>(t) );
    iterate_i<N+1, F, Args...>(f, t);
  }
  
  /** Exists so you can make the functor in place. */
  template <std::size_t N = 0, typename F, typename... Args>
    inline auto iterate_i(const F& _f, const std::tuple<Args...>& t) -> typename std::enable_if<N == 0, void>::type {
    F f(_f);
    iterate_i<N, F, Args...>(f, t);
  }
  /* -------------------- /iterate_i -------------------- */

  /* -------------------- make_jumptable -------------------- */
  template <typename F, typename FT, std::size_t N, typename... Args>
    auto inline make_jumptable(FT* jump[sizeof...(Args)]) -> typename std::enable_if< N == sizeof...(Args), void>::type {
  }
 
  template <typename F, typename FT, std::size_t N, typename... Args>
    auto inline make_jumptable(FT* jump[sizeof...(Args)]) 
    -> typename std::enable_if< N < sizeof...(Args), void>::type {
    jump[N] = [](F& f, const std::tuple<Args...>& t) { f( std::get<N>(t)); };  
    make_jumptable<F, FT, N+1, Args...>(jump);
  }

  template <typename F, typename FT, std::size_t N, typename... Args>
    auto inline make_jumptable_rtn(FT* jump[sizeof...(Args)]) 
    -> typename std::enable_if< N == sizeof...(Args), void>::type {
  }

  template <typename F, typename FT, std::size_t N, typename... Args>
    auto inline make_jumptable_rtn(FT* jump[sizeof...(Args)]) 
    -> typename std::enable_if< N < sizeof...(Args), void>::type {
    jump[N] = [](F& f, const std::tuple<Args...>& t) { return f( std::get<N>(t)); };  
    make_jumptable_rtn<F, FT, N+1, Args...>(jump);
  }

  /* -------------------- /make_jumptable -------------------- */

  /* -------------------- run -------------------- */
  template <typename F, typename... Args>
    auto inline run( F& f, const size_t i, const std::tuple<Args...>& t) -> void
  {
    using FT = void ( F&, const std::tuple<Args...>& );
    static FT* jump[sizeof...(Args)];
    static bool init = false;
    if(!init){    
      init = true;
      make_jumptable<F, FT, 0, Args...>(jump);
    }
    jump[i](f, t);
  }
  /* -------------------- /run -------------------- */

  /* -------------------- get -------------------- */
  template <typename F, typename... Args>
    auto inline get( F& f, const size_t i, const std::tuple<Args...>& t) -> decltype(f(std::get<0>(t) ))
  {
    using FT = decltype(f( std::get<0>(t))) ( F&, const std::tuple<Args...>& );
    static FT* jump[sizeof...(Args)];
    static bool init = false;
    if(!init){    
      init = true;
      make_jumptable_rtn<F, FT, 0, Args...>(jump);
    }
    return jump[i](f, t);
  }
  /* -------------------- /get -------------------- */

  /* -------------------- to_vector -------------------- */
  template <std::size_t N = 0, typename F, typename Rtn, typename... Args>
    static inline auto to_vector(const F& , const std::tuple<Args...>&, std::vector<Rtn>& ) 
    -> typename std::enable_if<N == sizeof...(Args), void>::type { }

  template <std::size_t N = 0, typename F, typename Rtn, typename... Args>
    static inline auto to_vector(F& f, const std::tuple<Args...>& t, std::vector<Rtn>& vector) 
    -> typename std::enable_if<N < sizeof...(Args), void>::type
    {
      vector[N] = f(std::get<N>(t) );
      to_vector<N+1, F, Rtn, Args...>(f, t, vector);
    }
  
  template <typename F, typename... Args>
    static inline auto to_vector(F& f, const std::tuple<Args...>& t) 
    -> std::vector< decltype(f(std::get<0>(t) )) > 
  {
    using Rtn = decltype(f(std::get<0>(t) ));
    std::vector<Rtn> vector;
    vector.resize(sizeof...(Args));
    to_vector<0, F, Rtn, Args...>(f, t, vector);
    return vector;
  }

  /** Exists so you can make the functor in place. */
  template <typename F, typename Rtn, typename... Args>
    static inline auto to_vector(const F& _f, const std::tuple<Args...>& t) 
    -> std::vector< decltype(f(std::get<0>(t) )) > 
  {
    F f(_f);
    return to_vector<F, Rtn, Args...>(f, t);
  }
  /* -------------------- /to_vector -------------------- */

  /* -------------------- fold -------------------- */
  template <std::size_t N = 0, typename F, typename Fold, typename... Args>
    inline auto fold(const F& , const std::tuple<Args...>&, const Fold& fold) -> typename std::enable_if<N == sizeof...(Args), Fold>::type { 
    return fold;
  }

  template <std::size_t N = 0, typename F, typename Fold, typename... Args>
    inline auto fold(F& f, const std::tuple<Args...>& t, Fold& fold_v) -> typename std::enable_if<N < sizeof...(Args), Fold>::type{
    fold_v = f(fold_v,  std::get<N>(t) );
    return fold<N+1, F, Fold, Args...>(f, t, fold_v);
  }

  /** Exists so you can make the functor in place. */
  template <std::size_t N = 0, typename F, typename Fold, typename... Args>
    inline auto fold(const F& _f, const std::tuple<Args...>& t, const Fold& _fold) -> typename std::enable_if<N < sizeof...(Args), Fold>::type{
    F f(_f);
    Fold fold_v(_fold);
    return fold<N, F, Fold, Args...>(f, t, fold_v);
  }
  /* -------------------- /fold -------------------- */
}

#include <mxcomp/tuple/list.h>
#include <mxcomp/tuple/map.h>
