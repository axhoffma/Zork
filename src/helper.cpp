//
//  helper.cpp
//  Zork
//
//  Created by Austin Hoffmann on 10/17/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#include "helper.hpp"
#include "Room.h"
#include "Container.hpp"
#include "Creature.hpp"
#include "Item.hpp"

GameInformation::~GameInformation() {
  //Deletion of currentRoom is handled by the objectMap
  delete [] inventory;
};

//Add objects to a room
void add(Object* object, Room* room) {
  
  //Check if the object is a container
  if(dynamic_cast<Container*>(object) != NULL) {
    room->add_container(object->get_name());
  }
  
  //Check if the object is an Item
  else if(dynamic_cast<Item*>(object) != NULL) {
    room->add_item(object->get_name());
  }
  
  else if(dynamic_cast<Creature*>(object) != NULL) {
    room->add_creature(object->get_name());
  }
}

//Add an item to a container
void add(Item* item, Container* container) {
  container->add_item(item->get_name());
}

//Find an object on the map
Object* find_object(std::string objectName, std::unordered_map<std::string, Object*>& objectMap) {
  auto search = objectMap.find(objectName);
  if(search != objectMap.end()) {
    return search->second;
  }
  else {
    return nullptr;
  }
}

//Delete an object from a Room or Container
void delete_object(std::string objectToDelete, Object* objectToSearch) {
  
  //Check if the searched object is a container
  if(dynamic_cast<Container*>(objectToSearch) != NULL) {
    Container* container = dynamic_cast<Container*>(objectToSearch);
    bool found = container->find_item(objectToDelete);
    if(found) {
      container->remove_item(objectToDelete);
    }
  }
  
  //Check if the searched object is a Room
  if(dynamic_cast<Room*>(objectToSearch) != nullptr) {
    Room* room = dynamic_cast<Room*>(objectToSearch);
    bool found = room->find_container(objectToDelete);
    if(found) {
      room->remove_container(objectToDelete);
    }
    found = room->find_item(objectToDelete);
    if(found) {
      room->remove_item(objectToDelete);
    }
    found = room->find_creature(objectToDelete);
    if(found) {
      room->remove_creature(objectToDelete);
    }
  }
  
  //If the object is an Item or a Creature, do nothing
  
}

//Update the status of an object to a new string
void update(Object* object, std::string newStatus) {
  object->set_status(newStatus);
}

void parse_user_commands(std::string input, GameInformation& gameInfo, bool user) {
  bool triggerFired = false;
  bool& triggerPass = triggerFired;
  //only check triggers if user starts the command
  if(user) {
    gameInfo.currentRoom->find_triggers(input, gameInfo, triggerPass);
    gameInfo.inventory->find_triggers(input, gameInfo, triggerPass);
  }
  std::istringstream iss{input};
  std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
  if(!triggerFired) {
    //n, s, e, w
    if(tokens[0] == "n" || tokens[0] == "s" || tokens[0] == "e" || tokens[0] == "w") {
      gameInfo.currentRoom = gameInfo.currentRoom->movement(input, gameInfo);
    }
    
    //open exit
    else if(input == "open exit") {
      if(gameInfo.currentRoom->exit_check()) {
        exit(0);
      }
    }
    
    //i
    else if(tokens[0] == "i") {
      gameInfo.inventory->open_container();
    }
    
    //open (container)
    else if(tokens[0] == "open" && tokens.size() == 2) {
      std::string containerName = tokens[1];
      bool found = gameInfo.currentRoom->find_container(containerName);
      if(found) {
        auto containerSearch = gameInfo.objectMap.find(containerName);
        auto container = dynamic_cast<Container*>(containerSearch->second);
        container->open_container();
      }
      else {
        std::cout << "Error: that container is not in this room" << std::endl;
      }
    }
    
    //put (item) in (container)
    else if(tokens[0] == "put" && tokens.size() == 4) {
      bool found = gameInfo.inventory->find_item(tokens[1]);
      if(found) {
        bool containerFound = gameInfo.currentRoom->find_container(tokens[3]);
        if(containerFound) {
          auto containerSearch = gameInfo.objectMap.find(tokens[3]);
          auto container = dynamic_cast<Container*>(containerSearch->second);
          bool allowedInsert = container->check_accept(tokens[1]);
          if(allowedInsert) {
            container->add_item(tokens[1]);
            gameInfo.inventory->remove_item(tokens[1]);
            std::cout << "Item " << tokens[1] << " added to " << tokens[3] << std::endl;
          }
          else {
            std::cout << "You cannot put that item there" << std::endl;
          }
          
        }
        else {
          std::cout << "Error: that container is not in this room" << std::endl;
        }
        
      }
      else {
        std::cout << "Error: you dont have " << tokens[1] << " in your inventory" << std::endl;
      }
    }
    
    //take (item)
    else if(tokens[0] == "take" && tokens.size() == 2) {
      std::string itemName = tokens[1];
      bool found = gameInfo.currentRoom->find_item(itemName, gameInfo.objectMap);
      if(found) {
        gameInfo.inventory->add_item(itemName);
        gameInfo.currentRoom->remove_item(itemName, gameInfo.objectMap);
        
        std::cout << "Item " << itemName << " added to inventory" << std::endl;
      }
      else {
        std::cout << "Error: that item is not in the current room" << std::endl;
      }
    }
    
    //drop (item)
    else if(tokens[0]== "drop" && tokens.size() == 2) {
      std::string itemName = tokens[1];
      bool found = gameInfo.inventory->find_item(itemName);
      if(found) {
        gameInfo.inventory->remove_item(itemName);
        gameInfo.currentRoom->add_item(itemName);
        std::cout << itemName << " dropped" << std::endl;
      }
      else {
        std::cout << "Error: you do not have " << itemName << " in your inventory" << std::endl;
      }
    }
    
    //read (item)
    else if(tokens[0] == "read" && tokens.size() == 2) {
      std::string itemName = tokens[1];
      bool found = gameInfo.inventory->find_item(itemName);
      if(found) {
        auto search = gameInfo.objectMap.find(itemName);
        auto item = dynamic_cast<Item*>(search->second);
        item->read_writing();
      }
      else {
        std::cout << "Error: you do not have " << itemName << " in your inventory" << std::endl;
      }
    }
    
    //turn on (item)
    else if(tokens[0] == "turn" && tokens[1] == "on" && tokens.size() == 3) {
      std::string itemName = tokens[2];
      bool found = gameInfo.inventory->find_item(itemName);
      if(found) {
        Item* item = dynamic_cast<Item*>(gameInfo.objectMap[itemName]);
        //Get the vector of actions to perform
        auto actions = item->turn_on();
        for(auto action : actions) {
          parse_commands(action, gameInfo);
        }
      }
      else {
        std::cout << "Error: " << tokens[2] << " is not in your inventory" << std::endl;
      }
    }
    
    //attack (creature) with (item)
    else if(tokens[0] == "attack" && tokens[2] == "with" && tokens.size() == 4) {
      std::string itemName = tokens[3];
      std::string creatureName = tokens[1];
      bool found = gameInfo.currentRoom->find_creature(creatureName);
      if(found) {
        found = gameInfo.inventory->find_item(itemName);
        if(found) {
          Creature* creature = dynamic_cast<Creature*>(gameInfo.objectMap[creatureName]);
          bool effective = creature->check_attack(itemName);
          if(effective) {
            //Check the attack conditions
            std::cout << "You attack " << creatureName << " with " << itemName << "!" << std::endl;
            creature->execute_attack(gameInfo);
          }
          else {
            std::cout << "The item has no effect!" << std::endl;
          }
        }
        else {
          std::cout << "That item is not in your inventory" << std::endl;
        }
      }
      else {
        std::cout << "That creature is not in this room" << std::endl;
      }
    }
    
    //Only print Invalid if the user entered the command, otherwise
    //just check behind the scenes funtions
    else {
      if(user) {
        std::cout << "Invalid Command" << std::endl;
      }
    }
    //only recheck triggers if it is a user command
    if(user) {
      gameInfo.currentRoom->find_triggers("", gameInfo, triggerPass);
      gameInfo.inventory->find_triggers("", gameInfo, triggerPass);
      while(triggerPass) {
        triggerPass = false;
        gameInfo.currentRoom->find_triggers("", gameInfo, triggerPass);
        gameInfo.inventory->find_triggers("", gameInfo, triggerPass);
      }
    }
  }
}


void parse_commands(std::string command, GameInformation& gameInfo) {
  //Separate the input words for easy analysis
  std::istringstream iss{command};
  std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
  
  //Check if the command matches a possible user command
  parse_user_commands(command, gameInfo, false);
  
  //Option 1: Update (object) to (status)
  if(tokens[0] == "Update") {
    auto search = gameInfo.objectMap.find(tokens[1]);
    if(search != gameInfo.objectMap.end()) {
      Object* object = search->second;
      update(object, tokens[3]);
    }
  }
  
  //Option 2: Add (object) to (room/container)
  if(tokens[0] == "Add") {
    auto search = gameInfo.objectMap.find(tokens[1]);
    if(search != gameInfo.objectMap.end()) {
      Object* object = search->second;
      search = gameInfo.objectMap.find(tokens[3]);
      if(search != gameInfo.objectMap.end()) {
        Object* addTo = search->second;
        if(dynamic_cast<Room*>(addTo) != NULL) {
          Room* room = dynamic_cast<Room*>(addTo);
          add(object, room);
        }
        if(dynamic_cast<Container*>(addTo) != NULL) {
          Container* container = dynamic_cast<Container*>(addTo);
          add(dynamic_cast<Item*>(object), container);
        }
      }
    }
  }
  
  //Option 3: Delete (object)
  if(tokens[0] == "Delete") {
    auto search = gameInfo.objectMap.find(tokens[1]);
    if(search != gameInfo.objectMap.end()) {
      Object* object = search->second;
      
      //If it is a room, just remove the room from the map
      if(dynamic_cast<Room*>(object) != NULL) {
        gameInfo.objectMap.erase(object->get_name());
      }
      
      //Search every Object and remove references
      for(auto mapElement : gameInfo.objectMap) {
        Object* mapObject = mapElement.second;
        delete_object(tokens[1], mapObject);
      }
    }
  }
  if(tokens[0] == "Game" && tokens[1] == "Over") {
    std::cout << "Victory!" << std::endl;
    exit(0);
  }
  
}
