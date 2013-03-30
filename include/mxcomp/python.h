/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#include "reflection.h"
#include <boost/python.hpp>
#include <cxxabi.h>
#include "tuple_ext.h"

namespace mxcomp {
  using namespace boost::python;

  template<typename T>
  struct python_process {
    boost::python::class_<T>& clazz;

    template <typename S>
    void operator()(const Field_<T, S>& f) {
      clazz.def_readwrite(f.name.c_str(), f.field);
    };

    template <typename S>
    void operator()(const Function_<T, S>& f) {
      clazz.def(f.name.c_str(), f.field);
    };
  };

  inline static std::string prettyName(const std::type_info& ti){
    char buf[1024];
    size_t size=1024;
    int status;
    char* res = abi::__cxa_demangle (ti.name(),
				     buf,
				     &size,
				     &status);
    return std::string(res);
  }

  template <typename... T>
    struct Register { 
      static void Do(){}
    };

  template <typename H, typename... T> 
    struct Register<H, T...> {
    static void Do() {
      auto clazz = boost::python::class_<H>( prettyName(typeid(H)).c_str(), init<>());
      auto functor = python_process<H>{clazz};
      mxcomp::tuples::iterate(functor, MetaClass_<H>::members());    
      Register<T...>::Do();
    }
  };
}

#define MODULE(name, ...)			\
  BOOST_PYTHON_MODULE(name)			\
  {						\
    mxcomp::Register<__VA_ARGS__>::Do();	\
  }						
