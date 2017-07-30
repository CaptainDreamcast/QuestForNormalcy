#include "button.h"

#include <tari/animation.h>
#include <tari/texture.h>
#include <tari/input.h>
#include <tari/actorhandler.h>
#include <tari/math.h>
#include <tari/physicshandler.h>

#include "ui.h"
#include "bg.h"

typedef int(*ButtonCheckFunc)();

static struct {
	TextureData mTexture[8];
	ButtonCheckFunc mFunc[8];

	double mX;
} gData;

void loadButtons()
{
	gData.mTexture[0] = loadTexture("assets/sprites/A.pkg");
	gData.mFunc[0] = hasPressedAFlank;

	gData.mTexture[1] = loadTexture("assets/sprites/B.pkg");
	gData.mFunc[1] = hasPressedBFlank;

	gData.mTexture[2] = loadTexture("assets/sprites/X.pkg");
	gData.mFunc[2] = hasPressedXFlank;

	gData.mTexture[3] = loadTexture("assets/sprites/Y.pkg");
	gData.mFunc[3] = hasPressedYFlank;

	gData.mTexture[4] = loadTexture("assets/sprites/UP.pkg");
	gData.mFunc[4] = hasPressedUpFlank;

	gData.mTexture[5] = loadTexture("assets/sprites/DOWN.pkg");
	gData.mFunc[5] = hasPressedDownFlank;

	gData.mTexture[6] = loadTexture("assets/sprites/LEFT.pkg");
	gData.mFunc[6] = hasPressedLeftFlank;

	gData.mTexture[7] = loadTexture("assets/sprites/RIGHT.pkg");
	gData.mFunc[7] = hasPressedRightFlank;
}

typedef struct {
	int mAnimationID;
	int mPhysics;
	int mType;
	int mActor;

	int mIsActive;
} Button;

static void loadButton(void* tData) {
	Button* e = tData;

	e->mType = randfromInteger(0, 7);
	e->mPhysics = addToPhysicsHandler(makePosition(650+128-32, 50, 5));
	e->mAnimationID = playOneFrameAnimationLoop(makePosition(0, 0, 0), &gData.mTexture[e->mType]);
	setAnimationBasePositionReference(e->mAnimationID, getHandledPhysicsPositionReference(e->mPhysics));
	e->mIsActive = 1;
}

static void unloadButton(void* tData) {
	Button* e = tData;

	removeHandledAnimation(e->mAnimationID);
	removeFromPhysicsHandler(e->mPhysics);

}

static void updateButton(void* tData) {
	Button* e = tData;

	Velocity* vel = getHandledPhysicsVelocityReference(e->mPhysics);
	vel->x = -gData.mX;
	
	Position p = *getHandledPhysicsPositionReference(e->mPhysics);
	if (p.x < -400) {
		removeActor(e->mActor);
		return;
	}

	if (!e->mIsActive) return;
	wobbleAnimation(e->mAnimationID, makePosition(32, 32, 0));

}

ActorBlueprint ButtonBP = {
	.mLoad = loadButton,
	.mUnload = unloadButton,
	.mUpdate = updateButton,
};


int addButton()
{
	Button* e = allocMemory(sizeof(Button));
	e->mActor = instantiateActorWithData(ButtonBP, e, 1);
	return e->mActor;
}

void removeButton(int tID)
{
	Button* e = getActorData(tID);
	e->mIsActive = 0;
	setAnimationSize(e->mAnimationID, makePosition(0,0,0), makePosition(0,0,0));
}

int hasPressedButton(int tID)
{
	Button* e = getActorData(tID);
	double x = getHandledPhysicsPositionReference(e->mPhysics)->x;
	int isInRange = isInUIRange(x+32);
	return isInRange && gData.mFunc[e->mType]();
}

int isButtonBelowRange(int tID)
{
	Button* e = getActorData(tID);
	double x = getHandledPhysicsPositionReference(e->mPhysics)->x;
	return isBelowUIRange(x + 32);
}

void setButtonSpeed(double x)
{
	gData.mX = x;
}

