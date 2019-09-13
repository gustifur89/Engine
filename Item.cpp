#include "Item.h"

// =============================== Item ===============================


Item::Item(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI) : Holdable(radius, mesh, UI, true)
{

}

Item::~Item()
{

}


// =============================== Gun ===============================


Gun::Gun(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI) : Item(radius, mesh, UI)
{

}

Gun::~Gun()
{

}
