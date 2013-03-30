/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#include <assert.h>
#include "tuple_ext.h"
#include <functional>
namespace mxcomp {

  template <typename T>
    struct use_ {
      const T* t = 0;  
      bool manage = false;
      use_(const T* _t) {
	t = _t;
	if(t && !t->managed) {
	  manage = /* t->managed = */ true;
	}
      }
      T* operator->(){	
	return t;
      }
      ~use_(){
	if(t && manage) {
	  delete t;
	  t = 0;
	}
      }
      operator bool() const {
	return t != 0;
      }
    };

  template<typename T> static inline bool operator==(const use_<T>& lhs, const use_<T>& rhs){
    if(rhs.t && lhs.t) return *lhs.t == *rhs.t;
    return lhs.t == rhs.t;    
  } 

  template<typename T> static inline bool operator< (const use_<T>& lhs, const use_<T>& rhs){ 
    if(rhs.t && lhs.t) return *lhs.t < *rhs.t;
    return lhs.t < rhs.t;
  } 

  template<typename T> static inline bool operator!=(const use_<T>& lhs, const use_<T>& rhs){return !operator==(lhs,rhs);} 
  template<typename T> static inline bool operator> (const use_<T>& lhs, const use_<T>& rhs){return  operator< (rhs,lhs);} 
  template<typename T> static inline bool operator<=(const use_<T>& lhs, const use_<T>& rhs){return !operator> (lhs,rhs);} 
  template<typename T> static inline bool operator>=(const use_<T>& lhs, const use_<T>& rhs){return !operator< (lhs,rhs);}
  
  template <typename T>
    static use_<T> use(T* t) {
    return use_<T>(t);
  }
  
}
