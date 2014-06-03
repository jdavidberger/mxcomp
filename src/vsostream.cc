#include <mxcomp/vsostream.h>
#define NTDDI_WIN7SP1 0x06010100
#include <Windows.h>
#include <ostream>
#include <sstream>
#include <string>
#pragma warning(disable: 4265)
#include <mutex>
#pragma warning(default: 4265)

namespace mxcomp {
     
     template <class CharT, class TraitsT = std::char_traits<CharT> >
     struct vsstreambuf : public std::basic_stringbuf<CharT, TraitsT> {
          std::mutex vsmutex;
          virtual ~vsstreambuf() { sync(); }
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
     protected:
          int sync()
          {
               
               std::lock_guard<std::mutex> g(vsmutex);
               output_debug_string(str().c_str());
               str(std::basic_string<CharT>());
               return 0;
          }
          void output_debug_string(const CharT *text) {}
     };

     template<> void vsstreambuf<char>::output_debug_string(const char *text)
     {
          ::OutputDebugStringA(text);
     }

     template<> void vsstreambuf<wchar_t>::output_debug_string(const wchar_t *text)
     {
          ::OutputDebugStringW(text);
     }

     std::ostream vsdb(new vsstreambuf<char>());
}