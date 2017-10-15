/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <functional>
#include <map>
#include <mutex>

namespace atlas {
namespace util {

template <typename Key, typename Value>
class Cache {
public:
  using key_type = Key;
  using value_type = Value;
  using creator_type = std::function<value_type* ()>;
  
  value_type* get(const key_type& key, const creator_type& creator) {
    std::lock_guard<std::mutex> guard(lock_);
    auto it = map_.find(key);
    if (it != map_.end()) {
      auto& value = it->second;
      if( value ) {
        Log::debug() << key << " was found in cache" << std::endl;
        return value;
      }
    }
    Log::debug() << key << " not found in cache, creating new" << std::endl;
    value_type* value = creator();
    map_[key] = value;
    return value;
  }
private:
  std::mutex lock_;
  std::map<key_type,value_type*> map_;
};

} // namespace util
} // namespace atlas
