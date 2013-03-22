#pragma once
#include <sstream>

namespace Options {
  enum t {
    DEFAULT = 3,
    Trim = 1,
    IgnoreWhitespace = 2
  };
}

// ----------- Parse format string -----------------
static inline bool skip_to_format(Options::t options, std::istream& b, const char*& format){
  while( !(format[0] == '$' && format[1] == '{' )){
    while(b.peek() != *format && (options & Options::IgnoreWhitespace) && isspace(b.peek()))
      b.get();
    if(b.peek() != *format) return false;    
    if(b.peek() == '\0') return false;
    b.get();
    format++;
  }     
  while(options & Options::IgnoreWhitespace && isspace(b.peek()))
    b.get();
  return true;
}

static inline bool skip_to_unformat(const char*& format){
  while( *format != '}' ){
    if(*format == '\0') return false;
    format++;
  } 
  format++;
  return true;
}

template <typename T>
inline static bool extract(std::istream& b, T& t){
  b >> t;  
  return !b.fail();
}

struct finish {
  std::string& s;  
};
static inline std::istream& operator >>(std::istream& b, finish& f) {
  b >> f.s;
  return b; 
}

template <>
inline bool extract(std::istream& b, std::string& t){
  while(b.good()){
    int v = b.get();
    bool accept = 
      (v >= '0' && v <= ';') ||
      (v >= '@' && v <= 'Z') ||
      (v >= '_' && v <= 'z') ||
      (v == '%') ||
      (v == '-');
    if(accept)
      t += v;
    else {
      b.unget();
      break;
    }
  }
  return t.size() > 0;
}

static inline int _msscanf(Options::t options, std::istream& b, const char*& format){
  while( *format != '\0' && !b.eof() ){
    if(b.peek() != *format) return 0;
    b.get(); 
    if(!b.good()) return 0;
    format++;
  }       

  if(options & Options::Trim)
    while( isspace(b.peek()) ) b.get();
  return *format == '\0' && EOF == b.peek() ? 1 : 0;
} 

template <typename H, typename... T>
static int _msscanf(Options::t options, std::istream& b, const char*& format, H& h, T&... t){  
  if(!skip_to_format(options, b, format)) return 0;
  skip_to_unformat(format);
  return extract(b, h) ? 
    1 + _msscanf(options, b, format, t...) :
    0;
}

template <typename... T>
  static int msscanf(Options::t options, std::istream& s, const char* format, T&... t){
  if(options & Options::Trim)
    while( isspace(s.peek()) ) s.get();
  return _msscanf(options, s, format, t...);
}

template <typename... T>
  static int msscanf(std::istream& s, const char* format, T&... t){
  return msscanf(Options::DEFAULT, s, format, t...);
}

template <typename B, typename... T>
static int msscanf(Options::t options, const B& b, const char* format, T&... t){
  std::stringstream ss(b);
  return msscanf(options, *(std::istream*)&ss, format, t...);
}

template <typename... T>
  static auto msscanf(const char* b, const char* format, T&... t) -> decltype(new std::stringstream(b), 0) {
  return msscanf(Options::DEFAULT, b, format, t...);
}

template <typename... T>
  static auto msscanf(const std::string& b, const char* format, T&... t) -> decltype(new std::stringstream(b), 0) {
  return msscanf(Options::DEFAULT, b, format, t...);
}
