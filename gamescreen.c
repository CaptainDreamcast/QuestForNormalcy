#include "gamescreen.h"

#include <stdio.h>

#include <tari/input.h>
#include <tari/sound.h>

#include "player.h"
#include "enemygenerator.h"
#include "ui.h"
#include "debris.h"
#include "bg.h"
#include "title.h"
#include "readytext.h"

static void loadGameScreen() {
	loadDebris();
	instantiateActor(BackgroundBP);
	instantiateActor(Player);
	instantiateActor(UserInterface);
	instantiateActor(EnemyGenerator);
	instantiateActor(ReadyTextBP);
	playTrack(3);
}

static Screen* getGameScreenNextScreen() {
	if (hasPressedAbortFlank()) {
		return &TitleScreen;
	}
	return NULL;
}


Screen GameScreen = {
	.mLoad = loadGameScreen,
	.mGetNextScreen = getGameScreenNextScreen,
};