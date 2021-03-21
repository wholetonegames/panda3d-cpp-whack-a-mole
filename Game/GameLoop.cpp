#include "GameLoop.h"


GameLoop::GameLoop(
	PandaFramework *framework,
	std::string model_dir,
	MouseCollision *mouseCollision,
	EventHandler *myEventHandler) : Menu(framework, model_dir, mouseCollision, myEventHandler)
{
	this->setup_stage("game_stage.egg", "**/hole5");

	this->framework->define_key("mouse1", "Mouse 1 Action", GameLoop::on_left_click, this);
	this->framework->define_key("mouse3", "Mouse 3 Action", GameLoop::on_right_click, this);

	this->set_moles_in_holes();

	this->update_world_task = new GenericAsyncTask("updates game loop", GameLoop::update_world, this);
	this->taskMgr->add(this->update_world_task);

	this->score = 0;
	this->score_text = new TextNode("scoreText");
	this->set_score_txt();
}


GameLoop::~GameLoop()
{
	this->stage.remove_node();
	this->update_world_task->remove();
	this->textNodePath.remove_node();
}



void GameLoop::on_left_click(const Event* eventPtr, void* dataPtr)
{
	GameLoop* _this = static_cast<GameLoop*>(dataPtr);
	if (!_this->mouseWatcher->has_mouse()) {
		return;
	}

	LPoint2 mpos = _this->mouseWatcher->get_mouse();

	_this->pickerRay->set_from_lens(_this->window->get_camera(0), mpos.get_x(), mpos.get_y());
	_this->myTraverser->traverse(_this->window->get_render());
	if (_this->myHandler->get_num_entries() == 0) {
		return;
	}
	// sorting entries so the first one is closest to the camera
	_this->myHandler->sort_entries();

	auto pickedObj = _this->myHandler->get_entry(0)->get_into_node_path();

	// whack moles
	if (pickedObj.get_name().find("mole") == 0) {
		auto index = pickedObj.get_net_tag("mole");
		auto mole = _this->moleModels.at(std::stoi(index));
		if (mole->get_motion_type() == rising) {
			_this->score += 100;
			_this->set_score_txt();
			mole->set_motion_type(sinking);
			_this->myEventHandler->dispatch_event(new Event("play_sfx"));
		}
	}

}

void GameLoop::on_right_click(const Event* eventPtr, void* dataPtr)
{
	GameLoop* _this = static_cast<GameLoop*>(dataPtr);
	_this->myEventHandler->dispatch_event(new Event("pause"));
}

void GameLoop::set_moles_in_holes()
{
	// Nine holes in total
	for (int i = 0; i < 9; i++) {
		const std::string & path = "**/hole" + std::to_string(i + 1);
		auto hole_pos = this->stage.find(path).get_pos();
		// hiding the mole completely inside the hole
		// to prevent accidental clicks
		hole_pos.add_z(-2.0f);
		auto moleData = new MoleData(i, hole_pos);
		auto mole = new MoleModel(moleData, this->model_dir, &this->stage, this->framework);
		this->moleModels.push_back(mole);
	}
}

AsyncTask::DoneStatus GameLoop::update_world(GenericAsyncTask *task, void *dataPtr)
{
	GameLoop* _this = static_cast<GameLoop*>(dataPtr);
	double delta_time = _this->globalClock->get_dt();

	if (_this->all_moles_at_rest()) {
		// "randomly" picking a mole and making it leave the hole
		_this->selected_mole_id = _this->globalClock->get_frame_count() % 9;
		_this->moleModels.at(_this->selected_mole_id)->set_motion_type(rising);
	}

	auto mole = _this->moleModels.at(_this->selected_mole_id);
	mole->update_pos(delta_time);
	return AsyncTask::DS_cont;
}

bool GameLoop::all_moles_at_rest()
{
	for (auto mole : this->moleModels) {
		if (mole->get_motion_type() != rest) return false;
	}
	return true;
}

void GameLoop::set_score(int score)
{
	this->score = score;
	this->set_score_txt();
}

void GameLoop::set_score_txt()
{
	this->score_text->set_text("score: " + std::to_string(this->score));
	this->textNodePath = this->window->get_aspect_2d().attach_new_node(this->score_text);
	this->textNodePath.set_scale(0.07);
	this->textNodePath.set_pos(-1.2, 0, 0.8);
}

SaveData* GameLoop::get_save_data()
{
	auto data = new SaveData();
	data->score = this->score;
	return data;
}
