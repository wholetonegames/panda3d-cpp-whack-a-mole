#pragma once
#include "stdafx.h"

struct MouseCollision
{
	MouseWatcher *mouseWatcher;
	CollisionRay *pickerRay;
	CollisionHandlerQueue *myHandler;
	CollisionTraverser *myTraverser;

	MouseCollision(
		MouseWatcher *mouseWatcher, CollisionRay *pickerRay,
		CollisionHandlerQueue *myHandler, CollisionTraverser *myTraverser
	)
	{
		this->mouseWatcher = mouseWatcher;
		this->pickerRay = pickerRay;
		this->myHandler = myHandler;
		this->myTraverser = myTraverser;
	}
};

struct SaveData {
	int score;
};

class Helper {
public:
	static float clamp(float value, float min, float max);
};

