#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Entities.h"
#include "j1Scene.h"
#include "j1Transition.h"
#include "j1Window.h"
#include "j1App.h"

j1Transition::j1Transition()
{
	name.create("transition");
}

j1Transition::~j1Transition()
{

}

bool j1Transition::Start()
{
	bool ret = true;

	LOG("Fading to black...");

	screen = { 0,0, App->win->screen_surface->w,App->win->screen_surface->h };

	return ret;
}

bool j1Transition::PostUpdate()
{
	bool ret = true;

	if (current_step == fade_step::none)
	{
		ret = true;
	}
	else 
	{
		switch (current_step)
		{
		case fade_step::fadeToBlack:
		{

		}
			break;
		}
	}
}

bool j1Transition::FadeToBlack(j1Module* off, j1Module* on)
{
	bool ret = true;

	if (current_step == fade_step::none)
	{
		module_on = on;
		module_off = off;
		current_step = fade_step::fadeToBlack;
		start_time = SDL_GetTicks();


		ret = true;
	}
	return ret;
}