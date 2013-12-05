#include "vibes.h"
#include "box.h"
#include "interval.h"
#include "vibes.h"
#include <vector>
#include <math.h>

#ifdef UNIX
#include <unistd.h>
#endif
#ifdef WIN32
#include <windows.h>
#endif

#define QUESTIONMARK_BOXES_NB 10
#define SCENE_WIDTH 1200
#define SCENE_HEIGHT 700

#define PLATFORM_WIDTH 20
#define PLATFORM_HEIGHT 160

#define RIGHT_PLAYER_FREQ 0.04
#define LEFT_PLAYER_FREQ 0.07

#define BALL_SPEED 0.5
#define SLEEP_MS 10

#define TIMER_TRESHOLD 300

void initScene();
void drawScene();
void updateScene();
void checkState();
void loseAnimation();
void crossSleepMs(int sleepMs);
