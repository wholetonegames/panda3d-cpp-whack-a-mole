#pragma once
#include "stdafx.h"
#include "helpers.h";
#include "Menu.h"

class OptionsMenu : public Menu
{
public:
	OptionsMenu(PandaFramework *framework, std::string model_dir, MouseCollision *mouseCollision, EventHandler *myEventHandler, std::string previousState);
	~OptionsMenu();

	static void on_left_click(const Event* eventPtr, void* dataPtr);
private:
	std::string previousState;
};

