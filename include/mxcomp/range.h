/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once 
namespace mxcomp {
  template <size_t... N>
  struct indices {}; 

  template < size_t begin, size_t end, typename I = void> 
    struct range {
      using type = typename range<begin, end, indices<> >::type;
    };

  template < size_t begin, size_t end, size_t... N > 
    struct range<begin, end, indices<N...> > {
    using type = typename range<begin+1, end, indices<N..., begin> >::type;
  };

  template < size_t end, size_t... N > 
    struct range<end, end, indices<N...> > {
    using type = indices<N...>;
  };
}
