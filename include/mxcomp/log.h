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
#include <iterator>

// http://connect.microsoft.com/VisualStudio/feedback/details/809540/c-warnings-in-stl-thread
#pragma warning(disable: 4265)
#include <mutex>
#pragma warning(default: 4265)

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
    
    extern int defaultLogLevel;
    extern std::mutex logMutex;

    void SetLogStream(std::ostream& stream);
    void SetLogStream(std::ostream* stream);
    void SetLogStream(const std::string& category, std::ostream& stream);
    std::ostream& LogStream(const std::string& category);
    void SetLogLevel(int level);
    void SetLogLevel(const std::string& category, int level);
    void SetLogLevel(const std::string& category, const std::string& level);

    std::map<std::string, int>& logLevels();
    static inline bool ShouldLog(const std::string& category, int level){

      auto it = logLevels().find(category);
      int ll = it == logLevels().end() ? defaultLogLevel : it->second;
      return ll <= level;
    }
    std::ostream& currThreadName(std::ostream& stream);    
  }
}

std::ostream& operator<<(std::ostream&, const std::type_info&);

#define LOCK_LOG (std::lock_guard<std::mutex>(mxcomp::log::logMutex)),

#define LOG(CAT, LEVEL)                                                                                                           \
     if(mxcomp::log::ShouldLog(#CAT, mxcomp::log::LEVEL) )                                                                        \
          LOCK_LOG mxcomp::log::LogStream(#CAT) << "[" << #CAT << ", " << #LEVEL << " (" << mxcomp::log::currThreadName << ")] "
