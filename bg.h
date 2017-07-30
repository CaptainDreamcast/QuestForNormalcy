#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

extern ActorBlueprint BackgroundBP;


void wobbleAnimation(int tAnimationID, Position tCenter);

Position* getGameScreenPositionReference();