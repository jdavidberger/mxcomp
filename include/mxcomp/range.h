/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once 

// This code is mostly due to http://preney.ca/paul/archives/934 
namespace mxcomp {
  template <size_t... N>
  struct indices {}; 

  template <class A, class B>
    struct merge;

  template <size_t... A, size_t... B>
    struct merge< indices<A...>, indices<B...> > {
    using type = indices<A..., (sizeof...(A)+B)...>;
  };

  template < size_t size > 
    struct seq {
      using type = 
	typename merge< typename seq<size / 2>::type, 
	                typename seq<size - size / 2>::type>::type; // size / 2 != (size - size / 2) for integers when size is odd. 
    };

  template <> struct seq<0> { using type = indices<>;  };
  template <> struct seq<1> { using type = indices<0>; };
  
  template < size_t begin, size_t end, typename I = void> 
    struct range {
      using type = typename range<begin, end, typename seq<end - begin >::type >::type;
    };
  
  template < size_t begin, size_t end, size_t... N > 
    struct range<begin, end, indices<N...> > {
    using type = indices< (begin + N)... >;
  };

  /*

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
  */
}
