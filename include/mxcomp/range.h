/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once 

// <s>This code is mostly due to http://preney.ca/paul/archives/934 </s>
// Now it's mostly due to http://stackoverflow.com/questions/13072359/c11-compile-time-array-with-logarithmic-evaluation-depth/13073076#13073076
namespace mxcomp {

  template <size_t... N>
  struct indices {}; 

  /** Merge, given two indices classes, produces an extension of the first with all the elements of the second, offset by the last element in the first. Ie, if you merge <0, 1, 2>, <0, 1, 2> you get <0, 1, 2, 3, 4, 5> */
  template <class A, class B>
    struct merge;

  template <size_t... A, size_t... B>
    struct merge< indices<A...>, indices<B...> > {
    using type = indices<A..., (sizeof...(A)+B)...>;
  };

  /** Sequence defines a sequence starting at 0 going up to and including size */ 
  template < size_t size > 
    struct seq {
      using type = 
	typename merge< typename seq<size / 2>::type, 
	                typename seq<size - size / 2>::type>::type; // size / 2 != (size - size / 2) for integers when size is odd. 
    };

  template <> struct seq<0> { using type = indices<>;  };
  template <> struct seq<1> { using type = indices<0>; };
  
  /** Range defines an indices range from 'begin' up to and excluding 'end'. */
  template < size_t begin, size_t end, typename I = void> 
    struct range {
      using type = typename range<begin, end, typename seq<end - begin >::type >::type;
    };
  
  template < size_t begin, size_t end, size_t... N > 
    struct range<begin, end, indices<N...> > {
    using type = indices< (begin + N)... >;
  };

}
