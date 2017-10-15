//
//  helper.hpp
//  Zork
//
//  Created by Austin Hoffmann on 10/15/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#ifndef helper_hpp
#define helper_hpp

#include "Object.h"
#include <unordered_map>

template<class T>
void update_map(T currentObject, std::unordered_map<std::string, T>& map) {
  map.erase(currentObject.get_name());
  map.insert(std::make_pair(currentObject.get_name(), currentObject));
}

#endif /* helper_hpp */
