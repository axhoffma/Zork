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

class Object {
private:
	std::string name;
	std::string status;
	std::string description;

public:
	virtual ~Object();

	//virtual void remove();
	//void update(std::string);

	std::string get_name();
	void set_name(std::string);

	std::string get_status();
	std::string get_description();
	void set_description(std::string);

};



#endif /* OBJECT_H_ */
