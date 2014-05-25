#pragma once

#include <iostream>
#include <typeinfo>
namespace mxcomp {
  static inline std::ostream& operator<<(std::ostream& o, const std::type_info& ti){
#ifdef _MSC_VER
	  const char* res = ti.name();
#else
	char buf[1024];
    size_t size=1024;
    int status;
	
	const char* res = abi::__cxa_demangle(ti.name(),
				     buf,
				     &size,
				     &status);
#endif

    return o << res;
}
}
