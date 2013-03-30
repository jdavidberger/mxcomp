/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#include <mxcomp/tuples.h>
using namespace std;
using namespace mxcomp::tuples;

struct A { };
struct B : public A {};
struct C {};
struct D : public A, public C {};

struct PrintType {
    template <typename T>
    void operator()(const size_t i, const T& t) {
      if(i > 0) std::cout << ", ";
      std::cout << &typeid(T).name()[1]; // Cheap demangle for simple struct (gcc)
    }
};
