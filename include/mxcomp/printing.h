#pragma once
#ifdef _MSC_VER
#include <Windows.h>
#define DEMANGLE_SYMBOL_NAME(name, buffer, size, status) name
#else 
#include <cxxabi.h>
#define DEMANGLE_SYMBOL_NAME(name, buffer, size, status) abi::__cxa_demangle(name, buffer, size, status)
#endif
#include <iostream>
#include <typeinfo>
namespace mxcomp {
  static inline std::ostream& operator<<(std::ostream& o, const std::type_info& ti){
    char buf[1024];
    size_t size=1024;
    int status;
	
	const char* res = DEMANGLE_SYMBOL_NAME(ti.name(),
				     buf,
				     &size,
				     &status);
    return o << res;
}
}
