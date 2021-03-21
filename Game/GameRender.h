#pragma once
#include "stdafx.h"

class GameRender
{
public:
	GameRender(PandaFramework *framework);
	~GameRender();
private:
	PandaFramework *framework;
	WindowFramework *window;

	void setup_lights();
	void setup_window();
	void setup_shaders();
	void setup_prc();
};
