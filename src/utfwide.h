#pragma once
#include "utf.h"

namespace mxcomp {
  namespace utf {    

       //TODO: find a better way to differentiate utf-16 vs utf-32 systems
#ifndef _MSC_VER
    static_assert( sizeof(wchar_t) == 4, "Invalid wchar_t size for environment.");

    template <> inline utf32_char_t extract_code_point(const wchar_t*& buffer, const wchar_t* end) {
      return extract_code_point<utf32_char_t>( *((const utf32_char_t**)&buffer), (const utf32_char_t*)end);
    }

    template <> inline uint8_t push_code_points(utf32_char_t point, std::wstring& str) {
      str.push_back(point);      
      return 1;
    }

    template <> inline uint8_t code_length<wchar_t>(wchar_t point) {
      return code_length<utf32_char_t>(point);
    }
#else 
       static_assert(sizeof(wchar_t) == 2, "Invalid wchar_t size for environment.");

       template <> inline utf32_char_t extract_code_point(const wchar_t*& buffer, const wchar_t* end) {
            return extract_code_point<utf16_char_t>(*((const utf16_char_t**)&buffer), (const utf16_char_t*)end);
       }

       template <> inline uint8_t push_code_points(utf32_char_t point, std::wstring& str) {
            return push_code_points<utf16_char_t>(point, *(utf16_t*)&str);
       }

       template <> inline uint8_t code_length<wchar_t>(wchar_t point) {
            return code_length<utf16_char_t>(point);
       }

#endif

  }
}

