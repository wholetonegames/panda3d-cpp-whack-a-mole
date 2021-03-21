#pragma once
#include "stdafx.h"

enum SoundManagerType { sfx, bgm };

class GameSound
{
public:
	GameSound(std::string model_dir);

	static AsyncTask::DoneStatus update_bgm(GenericAsyncTask *task, void *dataPtr);
	void play_sfx();
	void add_volume(SoundManagerType type, float value);
private:
	PT(AudioManager) sfx_manager;
	PT(AudioManager) bgm_manager;

	std::string model_dir;
	PT(AudioSound) sfx_click;
	PT(AudioSound) bgm_tetris;

	PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
	PT(GenericAsyncTask) update_bgm_task;
};

