#include "enemy.h"

#include <tari/actorhandler.h>
#include <tari/animation.h>
#include <tari/physicshandler.h>
#include <tari/math.h> 

#include "button.h"
#include "player.h"
#include "debris.h"
#include "bg.h"

static struct {
	Animation mIdleAnimation;
	TextureData mIdleTexture[4][10];
	
	int mStage;
} gData;

static double gSpeeds[] = { 5, 10, 12.5, 15 };

void loadEnemies() {
	gData.mIdleAnimation = createEmptyAnimation();
	gData.mIdleAnimation.mFrameAmount = 2;
	gData.mIdleAnimation.mDuration = 4;
	loadConsecutiveTextures(gData.mIdleTexture[0], "assets/sprites/ENEMY0_IDLE.pkg", gData.mIdleAnimation.mFrameAmount);
	loadConsecutiveTextures(gData.mIdleTexture[1], "assets/sprites/ENEMY1_IDLE.pkg", gData.mIdleAnimation.mFrameAmount);
	loadConsecutiveTextures(gData.mIdleTexture[2], "assets/sprites/ENEMY2_IDLE.pkg", gData.mIdleAnimation.mFrameAmount);
	loadConsecutiveTextures(gData.mIdleTexture[3], "assets/sprites/ENEMY3_IDLE.pkg", gData.mIdleAnimation.mFrameAmount);

	gData.mStage = 0;
	setButtonSpeed(gSpeeds[gData.mStage]);
}

typedef struct {
	int mAnimationID;
	int mPhysicsID;
	int mActor;
	int mButton;
	int mType;
} Enemy;

static void loadEnemy(void* tData) {
	Enemy* e = tData;

	e->mType = randfromInteger(0, 3);
	e->mPhysicsID = addToPhysicsHandler(makePosition(650, 200, 2));

	e->mAnimationID = playAnimationLoop(makePosition(0,0,0), gData.mIdleTexture[e->mType], gData.mIdleAnimation, makeRectangleFromTexture(gData.mIdleTexture[e->mType][0]));
	setAnimationBasePositionReference(e->mAnimationID, getHandledPhysicsPositionReference(e->mPhysicsID));
	setAnimationScreenPositionReference(e->mAnimationID, getGameScreenPositionReference());

	e->mButton = addButton();
}

static void unloadEnemy(void* tData) {
	Enemy* e = tData;

	removeButton(e->mButton);
	removeHandledAnimation(e->mAnimationID);
	removeFromPhysicsHandler(e->mPhysicsID);
}

static void updateEnemy(void* tData) {
	Enemy* e = tData;
	Position p = *getHandledPhysicsPositionReference(e->mPhysicsID);
	Velocity* vel = getHandledPhysicsVelocityReference(e->mPhysicsID);
	vel->x = -gSpeeds[gData.mStage];

	wobbleAnimation(e->mAnimationID, makePosition(128, 256, 0));

	if (hasPressedButton(e->mButton)) {
		punchEnemy();
		addDebris(makeGeoRectangle(p.x+64, p.y, 100, 200));
		removeActor(e->mActor);
		return;
	}

	
	if (isButtonBelowRange(e->mButton)) {
		healPlayer();
		addHeartDebris(makeGeoRectangle(p.x + 64, p.y, 100, 200));
		removeActor(e->mActor);
		return;
	}
}

ActorBlueprint EnemyBP = {
	.mLoad = loadEnemy,
	.mUnload = unloadEnemy,
	.mUpdate = updateEnemy,

};


void addEnemy()
{
	Enemy* e = allocMemory(sizeof(Enemy));

	e->mActor = instantiateActorWithData(EnemyBP, e, 1);
}

void increaseEnemySpeed()
{
	gData.mStage++;
	setButtonSpeed(gSpeeds[gData.mStage]);
}

void decreaseEnemySpeed()
{
	gData.mStage--;
	setButtonSpeed(gSpeeds[gData.mStage]);
}
