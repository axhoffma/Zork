//
//  Container.hpp
//  Zork
//
//  Created by Austin Hoffmann on 10/15/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#ifndef Container_hpp
#define Container_hpp

#include "Object.h"
#include "Item.hpp"

class Container : public Object {
  std::vector<Item> items;
};

#endif /* Container_hpp */
