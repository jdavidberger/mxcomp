#include <ostream>
#include <sstream>
#include <string>
#pragma warning(disable: 4265)
#include <mutex>
#pragma warning(default: 4265)

namespace mxcomp {
     template <class CharT, class TraitsT = std::char_traits<CharT> >
     struct threadstringbuf : public std::basic_stringbuf<CharT, TraitsT> {
          std::mutex vsmutex;
          virtual ~threadstringbuf() { sync(); }
          virtual int_type underflow() {
               std::lock_guard<std::mutex> g(vsmutex);
               return std::basic_stringbuf<CharT, TraitsT>::underflow();
          }
          virtual int_type overflow(int_type c = traits_type::eof()) {
               std::lock_guard<std::mutex> g(vsmutex);
               return std::basic_stringbuf<CharT, TraitsT>::overflow(c);
          }
          virtual int_type pbackfail(int_type c = traits_type::eof()) {
               std::lock_guard<std::mutex> g(vsmutex);
               return std::basic_stringbuf<CharT, TraitsT>::pbackfail(c);
          }
     };
}