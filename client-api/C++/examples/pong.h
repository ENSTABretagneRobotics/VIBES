// This file is part of VIBes' C++ API examples
//
// Copyright (c) 2013 Jeremy Nicola
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

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
