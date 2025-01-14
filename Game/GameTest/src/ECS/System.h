#pragma once

#include "stdafx.h"
#include "Entity.h"
#include <set>

//The difference between a set and a list is that a set is an ordered collection of unique elements, while a list is an ordered collection of elements that may contain duplicates
//We can also insert and erase entities in a set without using std::find to first see if they are in the set (unlike a list). If we try to insert or erase an entity that is or is not already in the set, nothing will happen

class System //A system is any functionality that iterates on a list of entities with a certain signature of components. All systems will inherit from this class.
{
public:
	std::set<Entity> entities; //Set of entities
};
