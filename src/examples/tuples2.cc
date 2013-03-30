#define METHOD1 true
#include "tuples2.h"
using namespace mxcomp::tuples;
void f(char& a, int& b, double& c, const char* d){
  std::cout << "F called! " << a << " " << b << " " << c << " " << d << std::endl;
}

int main() {
  auto tuple = make_tuple('a', 12, 3.14, "Test");
  
  // from "mxcomp/tuple/printing.h"
  printTuple(tuple); 
  
  // from "mxcomp/tuple/list.h"  
  printTuple(tail(tuple)); 
  printTuple(append(tuple, std::string("String"))); 

  applyTuple(f, tuple);
  applyTuple( [](char&,int&,double&,char const*&) { 
      std::cout << "Lambda called. " << std::endl;
    }, tuple);

  auto sTuple = make_tuple(A(), B(), C(), D());
  printTuple<PrintType>( ofType<A>(sTuple) );
  printTuple<PrintType>( ofType<C>(sTuple) );

  return 0;
}
