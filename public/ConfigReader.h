#pragma once

#include <windows.h>
#include "CommonMacros.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <functional>
#include <map>
#include <string>
#include <fstream>
#include "CommonApp.h"
#include "CommonMacros.h"

//! Wrapper of *boost::property_tree* to prvoide access to the underlying json file.
class ConfigReader final {
public:
  //! Default Construct
  ConfigReader() : config_file_path("") {}

  //! Construct the ConfigReader by the json file path.
  ConfigReader(const std::string &_config_file_path)
    : config_file_path(_config_file_path) {}

  //! Set the config path
  void SetConfigPath(const std::string &_config_file_path) { config_file_path = _config_file_path; }

  //
  std::string GetConfigPath() { return config_file_path; }

  //! Initialize the ConfigReader
  /** This function will read all data into memory. This function must be the first
   *  function to call,otherwise all the following calls will be failed. This function
   *  will set the *valid* flag. If the json file specified by ctor does not exist, the
   *  *valid* flag will be false; Otherwise, it will be true.
   */
  void Initialize() {
    std::ifstream file;
    file.open(config_file_path, std::ios_base::in);
    if (file.is_open()) {
      try {
        std::stringstream i;
        i << file.rdbuf();
        boost::property_tree::read_json(i, config_tree);
      }
      catch (...) {

      }
    }
    file.close();
  }

  //! Set the root path
  void SetPath(const std::string &_path_to_root) { path_to_root = _path_to_root; }

  //! Return the child tree relative to root path
  boost::property_tree::ptree GetChild(const std::string &_path) {
    return config_tree.get_child(GetPath(_path));
  }

  void Clear() { config_tree.clear(); }

  void Empty() { config_tree.empty(); }

  void Erase(const std::string &_path) {
    std::string _tempKey = GetPath(_path);
    size_t pos = _tempKey.rfind(".");
    if (pos >= 0) {
      std::string _key_root = _tempKey.substr(0, pos);
      std::string _del_key = _tempKey.substr(pos + 1);
      boost::property_tree::ptree &pchild = config_tree.get_child(_key_root);
      pchild.erase(_del_key);

    } else {
      config_tree.erase(_tempKey);
    }
  }

  void PutChild(const std::string _path, boost::property_tree::ptree child) {
    config_tree.put_child(GetPath(_path), child);
  }

  //! Return the value specified by path relative to root.
  template<typename return_t>
  return_t GetValue(const std::string _path) {
    return config_tree.get<return_t>(GetPath(_path));
  }

  template<typename return_t>
  return_t GetValue(const std::string _path, return_t _def_val) {
    return config_tree.get<return_t>(GetPath(_path), _def_val);
  }

  //! Store the value specified by path relatve to root.
  template<typename val_t>
  void PutValue(const std::string &_key, const val_t &_value) {
    config_tree.put(GetPath(_key), _value);
  }

  //! Write all values back to the file.
  void Save() {
    std::ofstream file;
    file.open(config_file_path, std::ios_base::out | std::ios_base::trunc);
    if(file.is_open()){
      std::stringstream text;
      boost::property_tree::write_json(text, config_tree);
      file << text.str();
      file.flush();
      file.close();
    }
  }

  //! Return a accessor instance with reference to this configuration reader
  /** To provide accessing methods with more semantic meaning, users could provide
   *  it's own accessor APIs. The only requirements of APIs is to have a constructor
   *  accepting a pointer to *ConfigReader*.
   */
  template<typename accessor_t>
  accessor_t GetAccessor() { return accessor_t(this); }

  //! Return all values in a form of std::map relative to the root path
  std::map<std::string, std::string> GetAllValues() {
    std::map<std::string, std::string> _result;

    auto _tree = std::ref(config_tree);
    if (!path_to_root.empty()) {
      auto _child_opt = config_tree.get_child_optional(path_to_root);
      if (!_child_opt) {
        PFAS_LOG(error) << "Invalid path to root: " << path_to_root;
        return _result;
      } else {
        _tree = std::ref(*_child_opt);
      }
    }

    IterateAll(
      _tree.get(), path_to_root,
      [&_result](const std::string &_key, const std::string &_val) { _result[_key] = _val; }
    );

    return _result;
  }

  //! Return all values in a form of std::map relative to the root path
  std::vector<std::string> GetLastValues(const std::string _path) {
    std::vector<std::string> _result;
    auto _tree = std::ref(config_tree);
    if (!path_to_root.empty()) {
      auto _child_opt = config_tree.get_child_optional(GetPath(_path));
      if (!_child_opt) {
        PFAS_LOG(error) << "Invalid path to root: " << path_to_root;
        return _result;
      } else {
        _tree = std::ref(*_child_opt);
        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, _tree.get()) {
                _result.push_back(v.first);
              }
      }
    }
    return _result;
  }

private:
  std::string GetPath(const std::string &_path) {
    if (path_to_root.empty()) {
      return _path;
    } else if (_path.empty()) {
      return path_to_root;
    } else {
      return path_to_root + "." + _path;
    }
  }

  void IterateAll(boost::property_tree::ptree &_tree, const std::string _path,
    std::function<void(const std::string &_key, const std::string &_v)> _cb) {
    if (_tree.empty()) {
      _cb(_path, _tree.data());
    } else {
      for (auto &v: _tree) {
        auto _next_path = _path.empty() ? v.first : (_path + "." + v.first);
        IterateAll(v.second, _next_path, _cb);
      }
    }
  }

private:
  std::string config_file_path;
  std::string path_to_root;
  boost::property_tree::ptree config_tree;
};