#pragma once
#include "stdafx.h"
#include "helpers.h";

class Menu
{
public:
	Menu(PandaFramework *framework, std::string model_dir, MouseCollision *mouseCollision, EventHandler *myEventHandler);
	~Menu();
protected:
	PandaFramework *framework;
	EventHandler *myEventHandler;
	WindowFramework *window;
	std::string model_dir;

	NodePath stage;
	void setup_stage(std::string model_name, std::string target_name);

	MouseWatcher *mouseWatcher;
	CollisionRay *pickerRay;
	CollisionHandlerQueue *myHandler;
	CollisionTraverser *myTraverser;
};

