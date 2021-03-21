#include "stdafx.h"
#include "GameRender.h"
#include "GameFSM.h"

int main(int argc, char *argv[]) {
	// Open a new window framework
	PandaFramework framework;
	framework.open_framework(argc, argv);

	// Here is room for your own code
	GameRender gameRender = GameRender(&framework);
	GameFSM gameFSM = GameFSM(&framework);
	gameFSM.request("StartState");

	// Do the main loop, equal to run() in python
	framework.main_loop();
	framework.close_framework();
	return (0);
}