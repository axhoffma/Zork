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
#include "./rapidXML/rapidxml.hpp"
#include <iostream>

class Container : public Object {
  std::vector<std::string> items;
  
public:
  Container(rapidxml::xml_node<>* node);
  Container();
  void open_container();
};

#endif /* Container_hpp */
