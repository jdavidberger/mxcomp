#pragma once
#include "utf.h"

namespace mxcomp {
  namespace utf {    

#ifndef _MSCVER
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
#endif

  }
}

