#include "Menu.h"


Menu::Menu(PandaFramework *framework, std::string model_dir, MouseCollision *mouseCollision, EventHandler *myEventHandler)
{
	this->framework = framework;
	this->myEventHandler = myEventHandler;
	this->window = this->framework->get_window(0);
	this->model_dir = model_dir;

	this->mouseWatcher = mouseCollision->mouseWatcher;
	this->pickerRay = mouseCollision->pickerRay;
	this->myTraverser = mouseCollision->myTraverser;
	this->myHandler = mouseCollision->myHandler;
}


Menu::~Menu()
{
	this->stage.remove_node();
}


void Menu::setup_stage(std::string model_name, std::string target_name)
{
	this->stage = this->window->load_model(this->framework->get_models(), this->model_dir + model_name);
	this->stage.reparent_to(this->window->get_render());


	auto pos = this->stage.find("**/camPos").get_pos();
	auto camera = this->window->get_camera_group();
	camera.set_pos(pos);
	auto lookAt = this->stage.find(target_name).get_pos();
	camera.look_at(lookAt);

	auto sun = this->window->get_render().find("**/=sunlight"); //equals means search for tag
	pos = this->stage.find("**/sunPos").get_pos();
	sun.set_pos(pos);
}
