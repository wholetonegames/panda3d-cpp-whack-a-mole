#include "OptionsMenu.h"


OptionsMenu::OptionsMenu(
	PandaFramework *framework,
	std::string model_dir,
	MouseCollision *mouseCollision,
	EventHandler *myEventHandler,
	std::string previousState) : Menu(framework, model_dir, mouseCollision, myEventHandler)
{
	this->setup_stage("menu_options.egg", "**/Text.002");
	this->framework->define_key("mouse1", "Mouse 1 Action", OptionsMenu::on_left_click, this);
	this->previousState = previousState;
}


OptionsMenu::~OptionsMenu()
{
	this->stage.remove_node();
}



void OptionsMenu::on_left_click(const Event* eventPtr, void* dataPtr)
{
	OptionsMenu* _this = static_cast<OptionsMenu*>(dataPtr);
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

	if (pickedObj.get_name().find("bgm_down") == 0) {
		_this->myEventHandler->dispatch_event(new Event("bgm_down"));
	}
	else if (pickedObj.get_name().find("bgm_up") == 0) {
		_this->myEventHandler->dispatch_event(new Event("bgm_up"));
	}
	else if (pickedObj.get_name().find("sfx_down") == 0) {
		_this->myEventHandler->dispatch_event(new Event("sfx_down"));
	}
	else if (pickedObj.get_name().find("sfx_up") == 0) {
		_this->myEventHandler->dispatch_event(new Event("sfx_up"));
	}
	else if (pickedObj.get_name().find("return") == 0) {

		if (_this->previousState == "StartState") {
			_this->myEventHandler->dispatch_event(new Event("start"));
		}
		else if (_this->previousState == "PauseState") {
			_this->myEventHandler->dispatch_event(new Event("pause"));
		}
		else {
			_this->myEventHandler->dispatch_event(new Event("newGame"));
		}
	}
}
