#include <mxcomp/vsostream.h>
#include <mxcomp/threadedstringbuf.h>
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
     struct vsstreambuf : public mxcomp::threadstringbuf<CharT, TraitsT> {
          std::mutex vsmutex;
          virtual ~vsstreambuf() { sync(); }          
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
