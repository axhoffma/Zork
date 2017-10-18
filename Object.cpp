/*
 * Object.cpp
 *
 *  Created on: Oct 13, 2017
 *      Author: austinhoffmann
 */

#include "Object.h"

Object::~Object() {}

void Object::set_name(std::string name) {
	this->name = name;
}
std::string Object::get_name() {
	return this->name;
}

void Object::set_description(std::string description) {
  this->description = description;
}
std::string Object::get_description() {
  return this->description;
}

void Object::set_status(std::string status) {
  this->status = status;
}

std::string Object::get_status() {
  return this->status;
}




