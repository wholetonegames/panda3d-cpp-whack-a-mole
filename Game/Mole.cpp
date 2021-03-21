#include "Mole.h"

MoleModel::MoleModel(MoleData* data, std::string model_dir, NodePath *stage, PandaFramework *framework)
{
	this->data = data;
	this->model_dir = model_dir;
	this->model_name = "game_mole.egg";
	this->stage = stage;
	this->framework = framework;
	this->window = this->framework->get_window(0);

	this->set_model();
}


MoleModel::~MoleModel()
{
	this->model.remove_node();
}

void MoleModel::set_data(MoleData* data)
{
	this->data = data;
}

MoleData* MoleModel::get_data()
{
	return this->data;
}

void MoleModel::set_model()
{
	this->model = this->window->load_model(this->framework->get_models(), this->model_dir + this->model_name);
	this->model.reparent_to(*this->stage);

	this->model.set_pos(this->data->pos);

	this->model.set_tag("mole", std::to_string(this->data->id));
}

void MoleModel::set_motion_type(MoleMotion state)
{
	this->data->state = state;
}

MoleMotion MoleModel::get_motion_type()
{
	return this->data->state;
}

void MoleModel::update_pos(double delta_time)
{
	if (this->data->pos.get_z() > 0.0f) {
		this->data->pos.set_z(0.0f);
		this->data->state = sinking;
	}

	if (this->data->pos.get_z() < -2.0f) {
		this->data->pos.set_z(-2.0f);
		this->data->state = rest;
	}

	switch (this->data->state)
	{
	case rising:
		this->data->pos.add_z(delta_time);
		break;
	case sinking:
		this->data->pos.add_z(-delta_time);
		break;
	default:
		// do nothing
		break;
	}

	this->model.set_pos(this->data->pos);
}
