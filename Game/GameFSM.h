#pragma once
#include "stdafx.h"
#include "fsm.h"
#include "helpers.h"
#include "StartMenu.h"
#include "GameLoop.h"
#include "GameSound.h"
#include "OptionsMenu.h"
#include "GamePause.h"

class GameFSM : public FSM
{
public:
	GameFSM(PandaFramework *framework);

private:
	DECL_STATE(GameFSM, StartState);
	DECL_STATE(GameFSM, OptionsState);
	DECL_STATE(GameFSM, PauseState);
	DECL_STATE(GameFSM, GameLoopState);

	PandaFramework *framework;
	EventHandler *myEventHandler;
	StartMenu *startMenu;
	GameLoop * gameLoop;

	OptionsMenu *optionsMenu;
	GamePause *gamePause;

	void get_assets_dir();
	std::string model_dir;

	void setup_mouse();
	MouseWatcher *mouseWatcher;
	CollisionRay *pickerRay;
	CollisionHandlerQueue *myHandler;
	CollisionTraverser *myTraverser;
	MouseCollision *mouseCollision;

	static void play_game(const Event * eventPtr, void* dataPtr);
	static void play_options(const Event * eventPtr, void* dataPtr);
	static void play_pause(const Event * eventPtr, void* dataPtr);
	static void play_start(const Event * eventPtr, void* dataPtr);
	static void play_sfx(const Event * eventPtr, void* dataPtr);

	static void bgm_down(const Event * eventPtr, void* dataPtr);
	static void bgm_up(const Event * eventPtr, void* dataPtr);
	static void sfx_down(const Event * eventPtr, void* dataPtr);
	static void sfx_up(const Event * eventPtr, void* dataPtr);
	GameSound *gameSound;

	SaveData* saveData;
};

