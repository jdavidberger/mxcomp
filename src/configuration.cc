/* Copyright (C) 2012-2013 Justin Berger 
   The full license is available in the LICENSE file at the root of this project and is also available at http://opensource.org/licenses/MIT. */
#include <mxcomp/configuration.h>
#include <mxcomp/log.h>

#include <dlfcn.h>
#include <assert.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <atomic>
using namespace rikitiki;
static void getAssemblyName(std::string& name){
    Dl_info info;
    memset(&info, 0, sizeof(Dl_info));
    dladdr( (void*)&getAssemblyName, &info);

    std::string exePath(info.dli_fname);
    auto start = exePath.find_last_of('/');
    auto end = exePath.find_last_of('.');

    start = start == std::string::npos ? 0 : start + 1;
    end = end == std::string::npos ? exePath.size() : 
      end <= start ? exePath.size() : end;    

    name = std::string(&exePath[start], &exePath[end]);
}

static void getAssemblyPath(std::string& path){
  Dl_info info;
  dladdr( (void*)&getAssemblyName, &info);
  std::string exePath(info.dli_fname);
  
  auto end = exePath.find_last_of('/');
  end = end == std::string::npos ? 1 : end;    
  path = std::string(&exePath[0], &exePath[end]);
}

Configuration::Configuration() : Config() {     
}

static size_t find_close(const std::string& path, int start){
  for(size_t i = start;i < path.size();i++){
    if(path[i] == ')')
      return i;
  }
  return std::string::npos;
}

static size_t find_open_decl(const std::string& path, int start){
  bool dollar = false;
  for(size_t i = start;i < path.size();i++){
    switch( path[i] ){
    case '$': dollar = true; break;
    case '(': if(dollar) return i; 
    default:
      dollar = false;            
    }
  }
  return std::string::npos;
}

static void expand_variables(std::map<std::string, std::string> variables, const std::string& value, std::string& exp_value){
  int last = 0;
  size_t open_decl, close_decl;

  do {
    open_decl = find_open_decl(value, last);
    close_decl =  find_close(value, open_decl);
    if(open_decl != std::string::npos){
      std::string key(&value[open_decl+1], &value[close_decl]);
      exp_value.insert(exp_value.back(), &value[last], open_decl - last - 1);
      exp_value += variables[key];    
      last = close_decl + 1;
    }
  } while (open_decl != std::string::npos);
  exp_value.insert(exp_value.size(), &value[last]);
}


static void performSubstitutions(std::map<std::string, std::string>& variables, libconfig::Setting& setting){
  if( setting.getType() == libconfig::Setting::TypeString ){
    std::string exp;
    std::string existing = setting;
    expand_variables(variables, existing, exp);
    setting = exp;
    variables[setting.getPath()] = exp;
    LOG(Config, Verbose) << "Setting: " << setting.getPath() << " = " << exp << std::endl;
  } else if( setting.isAggregate() ) {
    for(int i = 0;i < setting.getLength();i++){
      performSubstitutions(variables, setting[i]);
    }
  }
}

void Configuration::ReadGlobal() {
    std::string exeName, exePath;
    getAssemblyName(exeName);
    getAssemblyPath(exePath);
std::map<std::string, std::string> variables;  
    std::vector<std::string> configLocations;
    configLocations.push_back( exePath + "/" + exeName + ".cfg");
    configLocations.push_back( exePath + "/../conf/" + exeName + ".cfg");
    configLocations.push_back( "/etc/rikitiki/conf/" + exeName + ".cfg");
    configLocations.push_back( exePath + "/default.cfg");
    configLocations.push_back( exePath + "/../conf/default.cfg");
    configLocations.push_back( "/etc/rikitiki/conf/default.cfg");
    bool success = false;
    std::string filePath;
    for(auto it = configLocations.begin();success == false && it != configLocations.end();it++){
      FILE* file = fopen(it->c_str(), "r");
      if(file != NULL){
	filePath = *it;
	try {
	  read(file);
	  success = true;
	} catch(libconfig::ParseException& ex){
	  LOG(Config, Error) << "Can not parse: " << filePath << ": " << ex.getError() << " on " << ex.getLine() << std::endl;
	}
      }
    }
    if(!success){
      std::cerr << "Could not find a config file to use. Tried: " << std::endl;
      for(auto it = configLocations.begin();it != configLocations.end();it++){
	std::cerr << "\t" << *it << std::endl;
      }
    } else {
      variables["CONFIG_PATH"] = std::string(&filePath[0], &filePath[ filePath.find_last_of('/') ]);
    }
  
    variables["BIN_NAME"] = exeName;
    variables["BIN_PATH"] = exePath;
    LOG(Config, Info) << "Exe path " << exePath << std::endl;
    LOG(Config, Info) << "Loaded config file " << filePath << std::endl;
  
    performSubstitutions(variables, getRoot());
}

Configuration& Configuration::Global(){
  static Configuration config;
  static std::atomic_bool loaded(false);
  if(!loaded.exchange(true)) config.ReadGlobal();  
  return config;
}

