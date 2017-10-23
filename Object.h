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

struct GameInformation;

class Object {
private:
	std::string name;
	std::string status;
	std::string description;

public:
	virtual ~Object();
  
  //Construct the name, status, and description that is common to every Object
  Object(rapidxml::xml_node<>*);
  //Only use for base constructor is to make the inventory
  Object() : name{"inventory"} {};

  //get_name() returns the name of the object
	std::string get_name() const;

  //get_status() returns the status of the object, or an empty string if no status
  //was specified
	std::string get_status() const;
  
  //Set_status updates the status to the string passed into the function
  void set_status(std::string status);
  
  //Get_description() returns the description of the object, or an empty string
  //if no status was specified
	std::string get_description() const;
  //Updates the description to the string passed into the function
	void set_description(std::string);
  
  //find_object() should return a boolean value indicating if the object
  //passed in by name is owned by the current object
  virtual bool find_object(std::string object) = 0;
  
  //find_trigger() should search all objects owned by the current object
  //for valid triggers, and execute them if their conditions are met.
  //It should additionally set the boolean value to true if a trigger was run,
  //so the calling function can adjust accordingly
  virtual void find_triggers(std::string, GameInformation& gameInfo, bool&) = 0;

};



#endif /* OBJECT_H_ */
