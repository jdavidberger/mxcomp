#include <mxcomp/tuples.h>
using namespace std;
using namespace tupleExt;

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
