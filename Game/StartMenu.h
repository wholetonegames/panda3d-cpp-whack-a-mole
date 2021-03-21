#pragma once
#include "stdafx.h"
#include "helpers.h";
#include "Menu.h"

class StartMenu : public Menu
{
public:
	StartMenu(PandaFramework *framework, std::string model_dir, MouseCollision *mouseCollision, EventHandler *myEventHandler);
	~StartMenu();

	static void on_left_click(const Event* eventPtr, void* dataPtr);
};

