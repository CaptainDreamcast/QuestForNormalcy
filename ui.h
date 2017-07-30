#pragma once

#include <tari/actorhandler.h>

extern ActorBlueprint UserInterface;

int isInUIRange(double x);
int isBelowUIRange(double x);
void increaseUIRange();
void decreaseUIRange();