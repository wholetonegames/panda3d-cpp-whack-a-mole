#pragma once
#include "stdafx.h"
#include "helpers.h";
#include "Menu.h"

class GamePause : public Menu
{
public:
	GamePause(PandaFramework *framework, std::string model_dir, MouseCollision *mouseCollision, EventHandler *myEventHandler);
	~GamePause();

	static void on_left_click(const Event* eventPtr, void* dataPtr);
};

