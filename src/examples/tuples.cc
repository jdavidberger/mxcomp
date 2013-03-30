/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#include <mxcomp/tuples.h>
using namespace std;
using namespace mxcomp;
using namespace mxcomp::tuples;

struct F {
  int cnt = 0;

  template <typename T>
  int operator()(T& t){
    cnt++;
    std::cerr << t << std::endl;
    return cnt;
  }
  template <typename T>
  void operator()(T& t) const {
    std::cerr << t << std::endl;
  }

  template <typename T>
  size_t operator()(size_t idx, T& t) const {
    std::cerr << idx << ": " << t << std::endl;
    return idx + 1;
  }

  template <size_t idx, typename T>
  void operator()(T& t) const {
    std::cerr << idx << ": " << t << std::endl;
  }

};

int main() {
  auto tuple = make_tuple('a', 12, 3.14, "Test");
  F f;
  F const f2;
  printTuple(tuple);
  iterate(F(), tuple);
  iterate(f, tuple);
  iterate(F(), make_tuple('a', 12));
  iterate(f, make_tuple('a', 12));
  iterate(f2, make_tuple('a', 12));

  iterate_ti(F(), tuple);
  iterate_ti(f, tuple);
  iterate_ti(F(), make_tuple('a', 12));
  iterate_ti(f2, make_tuple('a', 12));

  iterate_i(F(), tuple);
  iterate_i(f, tuple);
  iterate_i(F(), make_tuple('a', 12));
  iterate_i(f2, make_tuple('a', 12));

  fold(F(), tuple, 0);
  fold(f, tuple, 1);
  fold(F(), make_tuple('a', 12), 2);
  fold(f2, make_tuple('a', 12), 3);

  get(F(), 0, tuple);
  get(f, 1, tuple);
  get(F(), 1, make_tuple('a', 12));
  get(f2, 0, make_tuple('a', 12));


  return 0;
}
