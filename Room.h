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

public:
	Room(rapidxml::xml_node<>* node);
	bool exit_check();
  Room* movement(std::string direction, const std::unordered_map<std::string, Object*>&);
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

};


#endif /* ROOM_H_ */
