#include "GameSound.h"
#include "helpers.h"



GameSound::GameSound(std::string model_dir)
{
	this->sfx_manager = AudioManager::create_AudioManager();
	this->sfx_click = this->sfx_manager->get_sound(model_dir + "Pickup_Coin.wav");

	this->bgm_manager = AudioManager::create_AudioManager();
	this->bgm_tetris = this->bgm_manager->get_sound(model_dir + "Korobeiniki-arr.ogg");
	this->bgm_tetris->set_loop(true);
	this->bgm_tetris->play();

	this->update_bgm_task = new GenericAsyncTask("updates game loop", GameSound::update_bgm, this);
	this->taskMgr->add(this->update_bgm_task);
}


AsyncTask::DoneStatus GameSound::update_bgm(GenericAsyncTask *task, void *dataPtr)
{
	GameSound* _this = static_cast<GameSound*>(dataPtr);
	_this->bgm_manager->update();
	return AsyncTask::DS_cont;
}

void GameSound::play_sfx()
{
	// because there's only one sfx we just play it
	// without further checks
	this->sfx_click->play();
}

void GameSound::add_volume(SoundManagerType type, float value)
{
	float vol;
	switch (type)
	{
	case sfx:
		vol = this->sfx_manager->get_volume();
		this->sfx_manager->set_volume(Helper::clamp(vol + value, 0.0f, 1.0f));
		break;
	case bgm:
		vol = this->bgm_manager->get_volume();
		this->bgm_manager->set_volume(Helper::clamp(vol + value, 0.0f, 1.0f));
		break;
	default:
		// do nothing
		break;
	}
}