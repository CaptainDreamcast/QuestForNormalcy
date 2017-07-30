#include "ui.h"

#include <tari/animation.h>

#include "button.h"

static struct {
	TextureData mUIBG;
	int mUIBGAnimationID;

	TextureData mRangeBegin;
	int mRangeBeginAnimationID;

	TextureData mRangeMiddle;
	int mRangeMiddleAnimationID;

	TextureData mRangeEnd;
	int mRangeEndAnimationID;

	double mPositionOffset;

	int mCurrentRange;
	double mRangeSize;
} gData;

static double gRanges[] = {300, 250, 200, 150, 100};

static void setRangeSize() {
	double rangeSize = gData.mRangeSize;
	setAnimationSize(gData.mRangeMiddleAnimationID, makePosition(rangeSize, 64, 1), makePosition(0, 0, 0));
	setAnimationPosition(gData.mRangeEndAnimationID, makePosition(200+ rangeSize -32, 50, 4));
}

static void loadUserInterface(void* tData) {
	(void)tData;

	gData.mPositionOffset = 200;

	gData.mUIBG = loadTexture("assets/sprites/UIBG.pkg");
	gData.mUIBGAnimationID = playOneFrameAnimationLoop(makePosition(0, 50, 2), &gData.mUIBG);
	
	gData.mRangeBegin = loadTexture("assets/sprites/RANGE_BEGIN.pkg");
	gData.mRangeBeginAnimationID = playOneFrameAnimationLoop(makePosition(200, 50, 4), &gData.mRangeBegin);
	
	gData.mRangeMiddle = loadTexture("assets/sprites/RANGE_MIDDLE.pkg");
	gData.mRangeMiddleAnimationID = playOneFrameAnimationLoop(makePosition(200, 50, 3), &gData.mRangeMiddle);

	gData.mRangeEnd = loadTexture("assets/sprites/RANGE_END.pkg");
	gData.mRangeEndAnimationID = playOneFrameAnimationLoop(makePosition(200, 50, 4), &gData.mRangeEnd);

	gData.mCurrentRange = 0;
	gData.mRangeSize = gRanges[gData.mCurrentRange];
	setRangeSize();


	loadButtons();
}

static void updateUserInterface(void* tData) {
	(void)tData;
	double target = gRanges[gData.mCurrentRange];
	double dx = (target - gData.mRangeSize);
	gData.mRangeSize += 0.1*dx;

	setRangeSize();
}

ActorBlueprint UserInterface = {
	.mLoad = loadUserInterface,
	.mUpdate = updateUserInterface,
};

int isInUIRange(double x)
{
	return x >= gData.mPositionOffset+32 && x <= gData.mPositionOffset+gData.mRangeSize-32;
}

int isBelowUIRange(double x)
{
	return x < gData.mPositionOffset + 32;
}

void decreaseUIRange()
{
	gData.mCurrentRange++;
}

void increaseUIRange()
{
	gData.mCurrentRange--;	
}
