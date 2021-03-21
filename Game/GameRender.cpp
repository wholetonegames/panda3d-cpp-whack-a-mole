#include "GameRender.h"

GameRender::GameRender(PandaFramework *framework)
{
	this->framework = framework;
	this->setup_prc();
	this->setup_window();
	this->setup_shaders();
	this->setup_lights();
}

GameRender::~GameRender()
{
}

void GameRender::setup_lights()
{
	PT(DirectionalLight) sun;
	sun = new DirectionalLight("sun");
	sun->set_color(LColor(1, 1, 1, 1));
	sun->set_scene(this->window->get_render());
	NodePath slnp = this->window->get_render().attach_new_node(sun);
	slnp.set_tag("sunlight", "1");
	this->window->get_render().set_light(slnp);
}

void GameRender::setup_window()
{
	WindowProperties props;
	props.set_size(1280, 720);
	props.set_title("Clicking Game");

	this->window = this->framework->open_window(props, 0, nullptr, nullptr);

	// listening for any key events (keyboard, mouse or gamepad)
	this->window->enable_keyboard();

	// Setting the window background colour to white
	DisplayRegion *dr = this->window->get_display_region_3d();
	dr->set_clear_color(LColorf(1, 1, 1, 1));
}

void GameRender::setup_shaders()
{
	auto tempNode = new NodePath(new PandaNode("temp node"));
	tempNode->set_attrib(LightRampAttrib::make_hdr0());

	auto camera = this->window->get_camera(0);
	camera->set_initial_state(tempNode->get_state());

	auto render = this->window->get_render();
	render.set_shader_auto(0);
	render.set_antialias(AntialiasAttrib::M_multisample);
}

void GameRender::setup_prc()
{
	char confauto[] = R"(
		load-display pandagl
		audio-library-name p3openal_audio
		framebuffer-multisample 1
		multisamples 2)";

	load_prc_file_data("internal", confauto);
}
