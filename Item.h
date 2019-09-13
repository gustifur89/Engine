#pragma once
#include "Headers.h"
#include "Entity.h"

class Item : public Holdable
{
public:
	Item(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI);
	~Item();
};

class Gun : public Item
{
public:
	Gun(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI);
	~Gun();
};

