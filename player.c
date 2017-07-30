#include "player.h"

#include <tari/animation.h>
#include <tari/wrapper.h>

#include <tari/stagehandler.h>

#include "ui.h"
#include "bg.h"
#include "congratsscreen.h"
#include "enemygenerator.h"
#include "enemy.h"

static struct {
	TextureData mIdleTextures[10];

	TextureData mFistTextures[10];
	TextureData mPunchTextures[10];

	int mNextAnimationID;
	int mAnimationID;
	int mFistAnimationID;

	int mIdleState;
	double mInterpolation;

	Duration mNow;
	Duration mPunchDuration;
	int mIsPunching;

	int mIsTurning;
} gData;

static Duration gStateDurations[] = { 10*60, 30*60, 60*60, 7*60 };

static void loadPlayer(void* tData) {
	(void)tData;

	loadConsecutiveTextures(gData.mIdleTextures, "assets/sprites/PLAYER_IDLE.pkg", 6);
	loadConsecutiveTextures(gData.mFistTextures, "assets/sprites/PLAYER_HERO_FIST.pkg", 6);
	loadConsecutiveTextures(gData.mPunchTextures, "assets/sprites/PLAYER_HERO_PUNCH.pkg", 6);

	gData.mIdleState = 0;
	gData.mInterpolation = 0;

	gData.mAnimationID = playOneFrameAnimationLoop(makePosition(40, 200, 3), &gData.mIdleTextures[gData.mIdleState]);
	setAnimationScreenPositionReference(gData.mAnimationID, getGameScreenPositionReference());
	
	gData.mFistAnimationID = playOneFrameAnimationLoop(makePosition(40, 200, 5), &gData.mFistTextures[gData.mIdleState]);
	setAnimationScreenPositionReference(gData.mFistAnimationID, getGameScreenPositionReference());
	
	gData.mNextAnimationID = playOneFrameAnimationLoop(makePosition(40, 200, 4), &gData.mIdleTextures[gData.mIdleState + 1]);
	setAnimationScreenPositionReference(gData.mNextAnimationID, getGameScreenPositionReference());

	setAnimationTransparency(gData.mAnimationID, 1 - gData.mInterpolation);
	setAnimationTransparency(gData.mNextAnimationID, gData.mInterpolation);

	gData.mPunchDuration = 10;
	gData.mIsPunching = 0;

	gData.mIsTurning = 0;
}

static void updatePunch() {
	if (!gData.mIsPunching) return;

	if (handleDurationAndCheckIfOver(&gData.mNow, gData.mPunchDuration)) {
		gData.mIsPunching = 0;
		changeAnimation(gData.mFistAnimationID, &gData.mFistTextures[gData.mIdleState], createOneFrameAnimation(), makeRectangleFromTexture(gData.mFistTextures[gData.mIdleState]));
	}
}


static void setNextIdleStage() {
	if (gData.mIdleState == 4) return;

	gData.mIdleState++;
	gData.mInterpolation = 0;
	changeAnimation(gData.mAnimationID, &gData.mIdleTextures[gData.mIdleState], createOneFrameAnimation(), makeRectangleFromTexture(gData.mIdleTextures[gData.mIdleState]));
	changeAnimation(gData.mNextAnimationID, &gData.mIdleTextures[gData.mIdleState + 1], createOneFrameAnimation(), makeRectangleFromTexture(gData.mIdleTextures[gData.mIdleState + 1]));
	setAnimationTransparency(gData.mAnimationID, 1 - gData.mInterpolation);
	setAnimationTransparency(gData.mNextAnimationID, gData.mInterpolation);
	decreaseUIRange();
	increaseEnemyGeneration();
	increaseEnemySpeed();


	if (gData.mIdleState == 4) {
		setNewScreen(&CongratsScreen);
	}
}

static void setPreviousIdleStage() {

	if (gData.mInterpolation == 0 && gData.mIdleState) {
		gData.mIdleState--;
		increaseUIRange();
		decreaseEnemyGeneration();
		decreaseEnemySpeed();

	}
	gData.mInterpolation = 0;
	changeAnimation(gData.mAnimationID, &gData.mIdleTextures[gData.mIdleState], createOneFrameAnimation(), makeRectangleFromTexture(gData.mIdleTextures[gData.mIdleState]));
	changeAnimation(gData.mNextAnimationID, &gData.mIdleTextures[gData.mIdleState + 1], createOneFrameAnimation(), makeRectangleFromTexture(gData.mIdleTextures[gData.mIdleState + 1]));
	changeAnimation(gData.mFistAnimationID, &gData.mFistTextures[gData.mIdleState], createOneFrameAnimation(), makeRectangleFromTexture(gData.mFistTextures[gData.mIdleState]));
	setAnimationTransparency(gData.mAnimationID, 1 - gData.mInterpolation);
	setAnimationTransparency(gData.mNextAnimationID, gData.mInterpolation);

	gData.mIsTurning = 9;
}

static void updateInterpolation() {
	if (!gData.mIsTurning) return;

	double fac = 1.0 / gStateDurations[gData.mIdleState];

	gData.mInterpolation += fac;
	setAnimationTransparency(gData.mAnimationID, 1 - gData.mInterpolation);
	setAnimationTransparency(gData.mNextAnimationID, gData.mInterpolation);

	if (gData.mInterpolation >= 1.0) {
		gData.mInterpolation -= 1.0;
		setNextIdleStage();
	}

}


static void updatePlayer(void* tData) {
	(void)tData;
	
	wobbleAnimation(gData.mAnimationID, makePosition(128, 256, 0));
	wobbleAnimation(gData.mNextAnimationID, makePosition(128, 256, 0));
	wobbleAnimation(gData.mFistAnimationID, makePosition(128, 256, 0));
	updateInterpolation();
	updatePunch();
}

ActorBlueprint Player = {
	.mLoad = loadPlayer,
	.mUpdate = updatePlayer,
};




void punchEnemy()
{
	gData.mIsPunching = 1;
	gData.mNow = 0;
	gData.mIsTurning = 1;

	addStageHandlerScreenShake(20);
	
	changeAnimation(gData.mFistAnimationID, &gData.mPunchTextures[gData.mIdleState], createOneFrameAnimation(), makeRectangleFromTexture(gData.mPunchTextures[gData.mIdleState]));

	
}

void healPlayer()
{
	setPreviousIdleStage();
}
