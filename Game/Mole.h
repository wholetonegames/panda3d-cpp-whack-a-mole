#pragma once
#include "stdafx.h"

enum MoleMotion { rest, rising, sinking };

struct MoleData
{
	int id;
	LPoint3f pos;
	MoleMotion state;

	MoleData(int id, LPoint3f pos, MoleMotion state = rest)
	{
		this->id = id;
		this->pos = pos;
		this->state = state;
	}
};

class MoleModel
{
public:
	MoleModel(MoleData* data, std::string model_dir, NodePath *stage, PandaFramework *framework);
	~MoleModel();

	void set_data(MoleData* data);
	MoleData* get_data();
	void update_pos(double delta_time);
	void set_motion_type(MoleMotion state);
	MoleMotion get_motion_type();
private:
	MoleData* data;
	std::string model_name;
	std::string model_dir;
	NodePath* stage;
	WindowFramework* window;
	PandaFramework* framework;
	void set_model();
	NodePath model;
};
