#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include "types.h"
#include <stddef.h>

typedef struct {
	int animation_steps;
	int animation_counter;
	size_t movable_count;
	Movable **movables;
} AnimationController;

AnimationController AC_init(int animation_steps, Movable **movables, size_t movable_count);
void AC_animate(AnimationController *ac);
void AC_startAnimation(AnimationController *ac);
int AC_isAnimating(AnimationController *ac);

#endif
