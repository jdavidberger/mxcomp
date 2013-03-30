/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#include "tuples2.h"
using namespace mxcomp::tuples;
template <typename... T> static auto make_2(T&... t) RETURN( std::make_tuple(t..., t...))
template <typename... T> static auto make_4(T&... t) RETURN( make_2(t..., t...))
template <typename... T> static auto make_8(T&... t) RETURN( make_4(t..., t...))
template <typename... T> static auto make_16(T&... t) RETURN( make_8(t..., t...))
template <typename... T> static auto make_32(T&... t) RETURN( make_16(t..., t...))
template <typename... T> static auto make_64(T&... t) RETURN( make_32(t..., t...))
template <typename... T> static auto make_128(T&... t) RETURN( make_64(t..., t...))
template <typename... T> static auto make_256(T&... t) RETURN( make_128(t..., t...))
template <typename... T> static auto make_512(T&... t) RETURN( make_256(t..., t...))

struct F {
  template <typename... Args>
  void operator()(Args... args) const {
    std::cout << sizeof...(args) << std::endl;
  }
};

int main() {
  auto tuple = make_64("A");
  
  printTuple(tuple); 
  
  printTuple(tail(tuple)); 
  printTuple(append(tuple, std::string("String"))); 
  
  F f;
  applyTuple(f, tuple);
  return 0;
}
