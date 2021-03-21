#include "StartMenu.h"


StartMenu::StartMenu(
	PandaFramework *framework, 
	std::string model_dir, 
	MouseCollision *mouseCollision, 
	EventHandler *myEventHandler) : Menu(framework, model_dir, mouseCollision, myEventHandler)
{
	this->setup_stage("menu_intro.egg", "**/play");
	this->framework->define_key("mouse1", "Mouse 1 Action", StartMenu::on_left_click, this);
}


StartMenu::~StartMenu()
{
	this->stage.remove_node();
}



void StartMenu::on_left_click(const Event* eventPtr, void* dataPtr)
{
	StartMenu* _this = static_cast<StartMenu*>(dataPtr);
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

	if (pickedObj.get_name().find("play") == 0) {
		_this->myEventHandler->dispatch_event(new Event("newGame"));
	}
	else if (pickedObj.get_name().find("options") == 0) {
		_this->myEventHandler->dispatch_event(new Event("options"));
	}
	else if (pickedObj.get_name().find("visit") == 0) {
		system("cmd /c start https://wholetonegames.blogspot.com");
	}
	else if (pickedObj.get_name().find("quit") == 0) {
		exit(0);
	}
}
