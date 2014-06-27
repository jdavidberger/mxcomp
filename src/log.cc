/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */

#include <mxcomp/log.h>
#include <fstream>
#include <iostream>
//#include <mxcomp/future_contwith.h>

// http://connect.microsoft.com/VisualStudio/feedback/details/809540/c-warnings-in-stl-thread
#pragma warning(disable: 4265)
#include <mutex>
#pragma warning(default: 4265)

#ifdef _MSC_VER
#include <mxcomp/vsostream.h>
static std::ostream* defaultStream = &mxcomp::vsdb;
#else 
static std::ostream* defaultStream = &std::cerr;
#endif


namespace mxcomp {
  namespace log {


    static std::map<std::string, std::ostream*> logStreams;
	std::mutex logMutex; 

    std::map<std::string, int>& logLevels(){
      static std::map<std::string, int>* logLevels = 0;
	  if (logLevels == 0) { // Makes logLevels look up cheap when there is an object (no mutex lock)
		  std::lock_guard<std::mutex> lock(logMutex);
		  if (logLevels == 0) { // If we blocked for another thread, make sure we dont create two logLevels. 
			  logLevels = new std::map<std::string, int>();
		  }
	  }
      return *logLevels;
    }
    void SetLogStream(std::ostream* stream) {
         defaultStream = stream;
    }
    void SetLogStream(std::ostream& stream) {
         defaultStream = &stream;
    }

    void SetLogStream(const std::string& category, std::ostream& stream){
      logStreams[category] = &stream;
    }

    std::ostream& LogStream(const std::string& category){
      auto it = logStreams.find(category);
      return it == logStreams.end() ? *defaultStream : *(it->second);
    }

    void SetLogLevel(const std::string& category, int level){
      logLevels()[category] = level;
    }
    static int getLevel(const std::string& level){
      if(level == "Verbose") {
	return Verbose;
      } else if (level == "Info") {
	return Info;
      } else if (level == "Debug") {
	return Debug;
      } else if (level == "Error") {
	return Error;
      }
      return INVALID;
    }

    void SetLogLevel(const std::string& category, const std::string& level){ 
      int _level = getLevel(level);
      SetLogLevel(category, _level);
      LOG(Logging, Verbose) << "Setting '" << category << "' to '" << level << "'(" << _level << ")" << std::endl;
      if(_level == INVALID)
	LOG(Logging, Error) << "Invalid logging level('" << level << "') set for category '" << category << "'. Level set to highest verbosity." << std::endl; 
    }
  }
}

// We _must_ define this block in this file; as its the only way to insure that it fires off before any of the functions for logging
#ifdef RT_USE_CONFIGURATION
#include <rikitiki/configuration/configuration>
using namespace rikitiki;
using namespace rikitiki::log;
using namespace libconfig;    

static bool LoadFromConfig(Configuration& config) {
  try {
    const Setting& logsettings = config.getRoot()["log"]["levels"];
    std::string name, value;
    for(int i = 0;i < logsettings.getLength();i++){
      const Setting& entry = logsettings[i];
      bool cFind = entry.lookupValue("category", name);
      bool vFind = entry.lookupValue("level", value);      
      if(cFind && vFind)
	SetLogLevel(name, value);
    }
    return true;
  } catch(const SettingNotFoundException& nfex){
    return false;
  }
}

static bool config_loaded = LoadFromConfig(Configuration::Global());
#endif

