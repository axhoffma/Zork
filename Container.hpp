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
#include <algorithm>

class Container : public Object {
  std::vector<std::string> items;
  std::vector<std::string> accepts;
  
public:
  Container(rapidxml::xml_node<>*);
  Container();
  
  bool find_item(std::string);
  inline void add_item(std::string item) {
    items.push_back(item);
  }
  inline void remove_item(std::string item) {
    auto index = std::find(std::begin(items), std::end(items), item);
    items.erase(index);
  }
  void open_container();
};

#endif /* Container_hpp */
