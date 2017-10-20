/*
 * Object.h
 *
 *  Created on: Oct 13, 2017
 *      Author: austinhoffmann
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "Trigger.hpp"
#include "./rapidXML/rapidxml.hpp"

class Object {
private:
	std::string name;
	std::string status;
	std::string description;

public:
	virtual ~Object();
  Object(rapidxml::xml_node<>*);
  //Only use for base constructor is to make the inventory
  Object() : name{"inventory"} {};


	std::string get_name();
	void set_name(std::string);

	std::string get_status();
  void set_status(std::string status);
	std::string get_description();
	void set_description(std::string);
  virtual bool find_object(std::string object) = 0;
  virtual void find_triggers(std::string, std::unordered_map<std::string, Object*>&, bool&) = 0;

};



#endif /* OBJECT_H_ */
