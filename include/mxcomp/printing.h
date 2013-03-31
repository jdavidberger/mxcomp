#pragma once

#include <cxxabi.h>
#include <iostream>
#include <typeinfo>
namespace mxcomp {
  static inline std::ostream& operator<<(std::ostream& o, const std::type_info& ti){
    char buf[1024];
    size_t size=1024;
    int status;
    char* res = abi::__cxa_demangle (ti.name(),
				     buf,
				     &size,
				     &status);
    return o << res;
}
}
