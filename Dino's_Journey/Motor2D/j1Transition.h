#ifndef __j1TRANSITION_H__
#define __j1TRANSITION_H__

#include"j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1Transition :public j1Module
{
public:

	j1Transition();
	j1Transition();

	bool Start();
	bool PostUpdate();
	bool FadeToBlack(j1Module* off, j1Module* on);

private:

	enum fade_step
	{
		none,
		fadeToBlack,
		fadeFromBlack
	}
	
	current_step = fade_step::none;

	uint32 start_time = 0;
	uint32 total_time = 0;

	SDL_Rect screen;

	j1Module* module_off;
	j1Module* module_on;

};

#endif // !__j1TRANSITION_H__

