//
//  Item.cpp
//  Zork
//
//  Created by Austin Hoffmann on 10/15/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#include "Item.hpp"


Item::Item(rapidxml::xml_node<>* itemNode) {
  rapidxml::xml_node<>* itemProperty = itemNode->first_node("name");
  set_name(itemProperty->value());
  
}
