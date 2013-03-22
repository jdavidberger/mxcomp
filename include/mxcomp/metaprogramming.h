#pragma once
#define RETURN(x) -> decltype(x) { return x; }
#define STATIC_RETURN(x) -> decltype(x) { static const auto _instance = x; return _instance; }

template <typename T> struct instance_of {
  T& value;
};
