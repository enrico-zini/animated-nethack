#include "AnimationController.h"

AnimationController AC_init(int animation_steps) {
	return (AnimationController) {
		.animation_steps = animation_steps,
		.animation_counter = 0
	};
}

void AC_animate(AnimationController *ac, Movable *movables[], size_t movable_count) {
	if (ac->animation_counter >= ac->animation_steps) return;
	
	ac->animation_counter++;
	float animation_progress = (float) ac->animation_counter / ac->animation_steps;
	
	for (size_t i = 0; i < movable_count; i++) {
		// UPDATE CURRENT POSITION
		Movable *mov = movables[i];
		mov->current_pos.x = (mov->tile_pos.x + mov->dir.x * animation_progress);
		mov->current_pos.y = (mov->tile_pos.y + mov->dir.y * animation_progress);

		// ANIMATION END -> UPDATE TILE POSITION
		if (ac->animation_counter == ac->animation_steps) {
			mov->tile_pos.x += mov->dir.x;
			mov->tile_pos.y += mov->dir.y;
		
			mov->current_pos.x = mov->tile_pos.x;
			mov->current_pos.y = mov->tile_pos.y;
		} 
	}
}

void AC_startAnimation(AnimationController *ac) {
	ac->animation_counter = 0;
}

int AC_isAnimating(AnimationController *ac) {
	return ac->animation_counter > 0 && ac->animation_counter < ac->animation_steps;
}
