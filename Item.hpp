//
//  Item.hpp
//  Zork
//
//  Created by Austin Hoffmann on 10/15/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#ifndef Item_hpp
#define Item_hpp

#include "Object.h"

class Item : public Object {
  std::string writing;
public:
  Item(rapidxml::xml_node<>*);
  void read_writing();
};

#endif /* Item_hpp */
