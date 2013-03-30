#include "../tuple_ext.h"
#include <iostream>

namespace mxcomp {
  namespace tuples {
  struct Print {
    template <typename T>
    void operator()(const size_t i, const T& t) {
      if(i > 0) std::cout << ", ";
      std::cout << t;
    }
  };

  template <typename P = Print, typename... Args>
    static inline void printTuple(const std::tuple<Args...>& tuple){
    P p;
    std::cout << "(";
    mxcomp::tuples::iterate_i(p, tuple);
    std::cout << ")" << std::endl;
  }
}
}
