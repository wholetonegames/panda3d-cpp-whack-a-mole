#include "GameFSM.h"

GameFSM::GameFSM(PandaFramework *framework) : FSM("GameFSM")
{
	this->framework = framework;
	this->myEventHandler = EventHandler::get_global_event_handler();
	this->get_assets_dir();
	this->setup_mouse();
	this->gameSound = new GameSound(this->model_dir);

	ADD_STATE(GameFSM, StartState);
	ADD_STATE(GameFSM, OptionsState);
	ADD_STATE(GameFSM, PauseState);
	ADD_STATE(GameFSM, GameLoopState);

	ADD_TRANSITION(StartState, OptionsState);
	ADD_TRANSITION(StartState, GameLoopState);
	ADD_TRANSITION(OptionsState, StartState);
	ADD_TRANSITION(OptionsState, PauseState);
	ADD_TRANSITION(PauseState, OptionsState);
	ADD_TRANSITION(PauseState, GameLoopState);
	ADD_TRANSITION(PauseState, StartState);
	ADD_TRANSITION(GameLoopState, PauseState);
	ADD_TRANSITION(GameLoopState, StartState);
}


DEF_ENTER(GameFSM, StartState)
{
	this->myEventHandler->add_hook("newGame", GameFSM::play_game, this);
	this->myEventHandler->add_hook("options", GameFSM::play_options, this);
	this->startMenu = new StartMenu(this->framework, this->model_dir, this->mouseCollision, this->myEventHandler);
}

DEF_EXIT(GameFSM, StartState)
{
	this->myEventHandler->remove_all_hooks();
	delete this->startMenu;
}

DEF_ENTER(GameFSM, OptionsState)
{
	this->myEventHandler->add_hook("start", GameFSM::play_start, this);
	this->myEventHandler->add_hook("pause", GameFSM::play_pause, this);
	this->myEventHandler->add_hook("newGame", GameFSM::play_game, this);
	this->myEventHandler->add_hook("bgm_down", GameFSM::bgm_down, this);
	this->myEventHandler->add_hook("bgm_up", GameFSM::bgm_up, this);
	this->myEventHandler->add_hook("sfx_down", GameFSM::sfx_down, this);
	this->myEventHandler->add_hook("sfx_up", GameFSM::sfx_up, this);
	auto previousState = this->get_prev_state()->get_name();
	this->optionsMenu = new OptionsMenu(this->framework, this->model_dir, this->mouseCollision, this->myEventHandler, previousState);
}

DEF_EXIT(GameFSM, OptionsState)
{
	this->myEventHandler->remove_all_hooks();
	delete this->optionsMenu;
}

DEF_ENTER(GameFSM, PauseState)
{
	this->myEventHandler->add_hook("newGame", GameFSM::play_game, this);
	this->myEventHandler->add_hook("start", GameFSM::play_start, this);
	this->myEventHandler->add_hook("options", GameFSM::play_options, this);
	this->gamePause = new GamePause(this->framework, this->model_dir, this->mouseCollision, this->myEventHandler);
}

DEF_EXIT(GameFSM, PauseState)
{
	this->myEventHandler->remove_all_hooks();
	delete this->gamePause;
}

DEF_ENTER(GameFSM, GameLoopState)
{
	this->myEventHandler->add_hook("pause", GameFSM::play_pause, this);
	this->myEventHandler->add_hook("play_sfx", GameFSM::play_sfx, this);
	this->gameLoop = new GameLoop(this->framework, this->model_dir, this->mouseCollision, this->myEventHandler);

	if (this->get_prev_state()->get_name() == "PauseState") {
		this->gameLoop->set_score(this->saveData->score);
	}
}

DEF_EXIT(GameFSM, GameLoopState)
{
	// storing data in case we need to resume game
	this->saveData = this->gameLoop->get_save_data();

	this->myEventHandler->remove_all_hooks();
	delete this->gameLoop;
}

void GameFSM::get_assets_dir()
{
	Filename mydir = ExecutionEnvironment::get_binary_name();
	mydir = mydir.get_dirname();
	this->model_dir = mydir + "/assets/";
}

void GameFSM::setup_mouse()
{
	PT(CollisionNode) pickerNode;
	NodePath pickerNP;

	auto window = this->framework->get_window(0);
	NodePath mouseNode = window->get_mouse();
	this->mouseWatcher = DCAST(MouseWatcher, mouseNode.node());

	pickerNode = new CollisionNode("mouseRay");
	auto camera = window->get_camera_group();
	pickerNP = camera.attach_new_node(pickerNode);
	pickerNode->set_from_collide_mask(GeomNode::get_default_collide_mask());
	this->pickerRay = new CollisionRay();
	pickerNode->add_solid(this->pickerRay);
	this->myHandler = new CollisionHandlerQueue();
	this->myTraverser = new CollisionTraverser("ctraverser");
	this->myTraverser->add_collider(pickerNP, this->myHandler);

	this->mouseCollision = new MouseCollision(this->mouseWatcher, this->pickerRay,
		this->myHandler, this->myTraverser);
}

void GameFSM::play_game(const Event * eventPtr, void* dataPtr)
{
	GameFSM* _this = static_cast<GameFSM*>(dataPtr);
	_this->request("GameLoopState");
}

void GameFSM::play_options(const Event * eventPtr, void* dataPtr)
{
	GameFSM* _this = static_cast<GameFSM*>(dataPtr);
	_this->request("OptionsState");
}

void GameFSM::play_pause(const Event * eventPtr, void* dataPtr)
{
	GameFSM* _this = static_cast<GameFSM*>(dataPtr);
	_this->request("PauseState");
}

void GameFSM::play_start(const Event * eventPtr, void* dataPtr)
{
	GameFSM* _this = static_cast<GameFSM*>(dataPtr);
	_this->request("StartState");
}

void GameFSM::play_sfx(const Event * eventPtr, void* dataPtr)
{
	GameFSM* _this = static_cast<GameFSM*>(dataPtr);
	_this->gameSound->play_sfx();
}

void GameFSM::bgm_down(const Event * eventPtr, void* dataPtr)
{
	GameFSM* _this = static_cast<GameFSM*>(dataPtr);
	_this->gameSound->add_volume(bgm, -0.1f);
}

void GameFSM::bgm_up(const Event * eventPtr, void* dataPtr)
{
	GameFSM* _this = static_cast<GameFSM*>(dataPtr);
	_this->gameSound->add_volume(bgm, 0.1f);
}

void GameFSM::sfx_down(const Event * eventPtr, void* dataPtr)
{
	GameFSM* _this = static_cast<GameFSM*>(dataPtr);
	_this->gameSound->add_volume(sfx, -0.1f);
}

void GameFSM::sfx_up(const Event * eventPtr, void* dataPtr)
{
	GameFSM* _this = static_cast<GameFSM*>(dataPtr);
	_this->gameSound->add_volume(sfx, 0.1f);
}
