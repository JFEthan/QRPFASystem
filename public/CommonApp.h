#pragma once
#include "CommonMacros.h"
#include "ConfigReader.h"
#include <memory>

//! Base class for all app in sub-modules such as ViewerTestApp in viewer module.
class CommonApp {
public:
  CommonApp(const std::string& _config_file_path) {
    config_reader = std::make_shared<ConfigReader>(_config_file_path);
    config_reader->Initialize();
  }
  virtual ~CommonApp() = default;

  //! Main entry method for module
  virtual int Run(int, char **) {
    PFAS_LOG(error) << "Need to re-implement Run() in sub-class";
    return 0;
  };

  //! Return the corresponding configuration accessor.
  //!
  //! The underlying configuration format is *json* and the json manipulation is implemented by using
  //! *boost::property_tree*. The APIs provided by *boost::property_tree* are quite common and generic.
  //! To provide more meaning APIs, moudules need to define its own accessor which will take *ConfigReader*
  //! as its ctor's parameter.
  template<typename accessor_t> accessor_t GetConfigurationAccessor() { 
    return config_reader->GetAccessor<accessor_t>();
  };

  virtual void ExecuteQuit() = 0;

protected:
  std::shared_ptr<ConfigReader> config_reader;
};