/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#pragma once
#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>
#include <pthread.h>
#include <typeinfo>
#include <atomic>

namespace rikitiki {
  namespace log {
    enum LogLevels {
      INVALID = -100,
      Debug = -2,
      Verbose = -1,
      Info,
      Warning,
      Error
    };

    void SetLogStream(const std::string& category, std::ostream& stream);
    std::ostream& LogStream(const std::string& category);

    void SetLogLevel(const std::string& category, int level);
    void SetLogLevel(const std::string& category, const std::string& level);

    std::map<std::string, int>& logLevels();
    static inline bool ShouldLog(const std::string& category, int level){

      auto it = logLevels().find(category);
      int ll = it == logLevels().end() ? 0 : it->second;
      return ll <= level;
    }

    static inline std::ostream& currThreadName(std::ostream& stream){
      std::string buffer;
      buffer.resize(32);
      pthread_getname_np(pthread_self(), &buffer[0], 32);    
      return stream << buffer.c_str();
    }
  }
}

std::ostream& operator<<(std::ostream&, const std::type_info&);
#define LOG(CAT, LEVEL) if(rikitiki::log::ShouldLog(#CAT, rikitiki::log::LEVEL) ) rikitiki::log::LogStream(#CAT) << "[" << #CAT << ", " << #LEVEL << " (" << rikitiki::log::currThreadName << ")] "
