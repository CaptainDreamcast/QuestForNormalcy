#include "congratsscreen.h"

#include <tari/animation.h>
#include <tari/input.h>
#include <tari/sound.h>

#include "title.h"

static struct {
	TextureData mBGTexture;
	int mBGID;
} gData;


static void loadCongratsScreen() {
	gData.mBGTexture = loadTexture("assets/sprites/CONGRATS.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(0,0,1), &gData.mBGTexture);	
	playTrack(4);
}

static Screen* getNextCongratsScreenScreen() {
	if (hasPressedAbortFlank()) {
		return &TitleScreen;
	}

	if (hasPressedStartFlank()) {
		return &TitleScreen;
	}

	return NULL;
}

Screen CongratsScreen = {
	.mLoad = loadCongratsScreen,
	.mGetNextScreen = getNextCongratsScreenScreen,
};