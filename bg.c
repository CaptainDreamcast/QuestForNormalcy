#include "bg.h"

#include <tari/animation.h>
#include <tari/tweening.h>
#include <tari/stagehandler.h>

static struct {
	TextureData mBGTexture;
	int mBGID;
	int mLayer;

	double mWobbleFactor;
} gData;

static double wobbleTweeningFunction(double t) {
	if (t < 0.1) return 1 - t / 0.1;

	return quadraticTweeningFunction(t);
}

static void restartTween(void* tCaller) {
	(void)tCaller;
	
	tweenDouble(&gData.mWobbleFactor, 1.0, 1.5, wobbleTweeningFunction, 20, restartTween, NULL);
}

static void loadBG(void* tData) {
	(void)tData;

	gData.mLayer = addScrollingBackground(1.0, 1.0);
	gData.mBGTexture = loadTexture("assets/sprites/BG.pkg");
	gData.mBGID = playOneFrameAnimationLoop(makePosition(-50, -10, 1), &gData.mBGTexture);
	setAnimationScreenPositionReference(gData.mBGID, getScrollingBackgroundPositionReference(gData.mLayer));

	gData.mWobbleFactor = 1;
	restartTween(NULL);
}

ActorBlueprint BackgroundBP = {
	.mLoad = loadBG,
};


void wobbleAnimation(int tAnimationID, Position tCenter)
{
	setAnimationScale(tAnimationID, makePosition(1+ (gData.mWobbleFactor - 1.0) / 5,gData.mWobbleFactor,1), tCenter);

}

Position * getGameScreenPositionReference()
{
	return getScrollingBackgroundPositionReference(gData.mLayer);
}
