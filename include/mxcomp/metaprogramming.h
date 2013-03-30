/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once
#define RETURN(x) -> decltype(x) { return x; }
#define STATIC_RETURN(x) -> decltype(x) { static const auto _instance = x; return _instance; }

template <typename T> struct instance_of {
  T& value;
};
