#pragma once
#include "stdafx.h"
#include "helpers.h";
#include "Menu.h"
#include "Mole.h"

class GameLoop : public Menu
{
public:
	GameLoop(PandaFramework *framework, std::string model_dir, MouseCollision *mouseCollision, EventHandler *myEventHandler);
	~GameLoop();

	static void on_left_click(const Event* eventPtr, void* dataPtr);
	static void on_right_click(const Event* eventPtr, void* dataPtr);
	static AsyncTask::DoneStatus update_world(GenericAsyncTask *task, void *dataPtr);

	SaveData* get_save_data();
	void set_score(int score);
private:
	PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
	PT(ClockObject) globalClock = ClockObject::get_global_clock();
	PT(GenericAsyncTask) update_world_task;

	void set_moles_in_holes();
	std::vector<MoleModel*> moleModels;

	bool all_moles_at_rest();
	int selected_mole_id;

	PT(TextNode) score_text;
	NodePath textNodePath;

	int score;
	void set_score_txt();
};

