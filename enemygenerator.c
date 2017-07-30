#include "enemygenerator.h"

#include <tari/datastructures.h>
#include <tari/animation.h>

#include "enemy.h"
#include "button.h"

static struct {
	Duration mNow;
	int mStage;

	int mIsActive;
} gData;

static Duration gFrequencies[] = { 120, 100, 90, 80, 60 };

static void loadEnemyGenerator(void* tData) {
	(void)tData;
	gData.mNow = 0;
	gData.mStage = 0;

	gData.mIsActive = 0;

	loadEnemies();
}

static void updateEnemyGenerator(void* tData) {
	(void)tData;
	if (!gData.mIsActive) return;

	if (handleDurationAndCheckIfOver(&gData.mNow, gFrequencies[gData.mStage])) {
		gData.mNow = 0;

		addEnemy();
	}
}

ActorBlueprint EnemyGenerator = {
	.mLoad = loadEnemyGenerator,
	.mUpdate = updateEnemyGenerator,
};

void increaseEnemyGeneration()
{
	gData.mStage++;
}

void decreaseEnemyGeneration()
{
	gData.mStage--;
}

void setEnemyGeneratorActive()
{
	gData.mIsActive = 1;
}
