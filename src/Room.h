/*
 * Room.h
 *
 *  Created on: Oct 13, 2017
 *      Author: austinhoffmann
 */

#ifndef ROOM_H_
#define ROOM_H_

#include "Object.h"
#include "Container.hpp"
#include <iostream>
#include <unordered_map>
#include "./rapidXML/rapidxml.hpp"


class Room : public Object {
  std::string type = "regular";
  std::array<std::string, 4> border;
  std::vector<std::string> containers;
  std::vector<std::string> items;
  std::vector<std::string> creatures;
  std::vector<Trigger> triggers;
  
public:
  Room(rapidxml::xml_node<>* node);
  bool exit_check();
  Room* movement(std::string direction, GameInformation&);
  bool find_container(std::string);
  inline void add_container(std::string container) {
    containers.push_back(container);
  }
  void remove_container(std::string container);
  bool find_item(std::string, const std::unordered_map<std::string, Object*>&);
  bool find_item(std::string);
  void remove_item(std::string, std::unordered_map<std::string, Object*>&);
  void remove_item(std::string);
  inline void add_item(std::string item) {
    items.push_back(item);
  }
  
  bool find_creature(std::string);
  inline void add_creature(std::string creature) {
    creatures.push_back(creature);
  }
  void remove_creature(std::string);
  
  bool find_object(std::string);
  void find_triggers(std::string input, GameInformation&, bool&);
};


#endif /* ROOM_H_ */
