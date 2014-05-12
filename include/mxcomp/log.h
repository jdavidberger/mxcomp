/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once
#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>
#ifndef _MSC_VER
#include <pthread.h>
#endif
#include <typeinfo>
#include <atomic>
#include <iterator>

namespace mxcomp {
  namespace log {
    enum LogLevels {
      INVALID = -100,
      Debug = -2,
      Verbose = -1,
      Info,
      Warning,
      Error
    };

    std::wostream & operator<< (std::wostream & ostr,
         std::string const & str);

    void SetLogStream(const std::string& category, std::wostream& stream);
    std::wostream& LogStream(const std::string& category);

    void SetLogLevel(const std::string& category, int level);
    void SetLogLevel(const std::string& category, const std::string& level);

    std::map<std::string, int>& logLevels();
    static inline bool ShouldLog(const std::string& category, int level){

      auto it = logLevels().find(category);
      int ll = it == logLevels().end() ? 0 : it->second;
      return ll <= level;
    }

    static inline std::wostream& currThreadName(std::wostream& stream){
      std::wstring buffer;
      buffer.resize(32);
#ifndef _MSC_VER
      pthread_getname_np(pthread_self(), &buffer[0], 32);    
#endif
      return stream << buffer.c_str();
    }
  }
}

std::ostream& operator<<(std::ostream&, const std::type_info&);
#define LOG(CAT, LEVEL) if(mxcomp::log::ShouldLog(#CAT, mxcomp::log::LEVEL) ) mxcomp::log::LogStream(#CAT) << L"[" << L#CAT << L", " << L#LEVEL << L" (" << mxcomp::log::currThreadName << L")] "
