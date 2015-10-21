#include "pong.h"

using namespace std;

box leftPlayer, rightPlayer, ball, background;
box questionMark[QUESTIONMARK_BOXES_NB];

box leftBorder(interval(-SCENE_WIDTH,-SCENE_WIDTH/2.0f),interval(-SCENE_HEIGHT,SCENE_HEIGHT));
box rightBorder(interval(SCENE_WIDTH/2.0f,SCENE_WIDTH),interval(-SCENE_HEIGHT,SCENE_HEIGHT));
box topBorder(interval(-SCENE_WIDTH,SCENE_WIDTH),interval(SCENE_HEIGHT/2.0f,SCENE_HEIGHT));
box bottomBorder(interval(-SCENE_WIDTH,SCENE_WIDTH),interval(-SCENE_HEIGHT/2.0f,-SCENE_HEIGHT));

vector<box> leftScore;
vector<box> rightScore;

int iterations;

int timer;

float angleInit;
float angle;

int main()
{
  vibes::beginDrawing();
  initScene();
  while(true)
    updateScene();
  return 0;
}

void initScene()
{
  vibes::newFigure("Interval Pong");
  angleInit=angle=9.0f;
  timer=0;
  iterations=0;
  leftPlayer=box(interval(-SCENE_WIDTH/2.0f,-SCENE_WIDTH/2.0f+PLATFORM_WIDTH),interval(-PLATFORM_HEIGHT/2.0f,PLATFORM_HEIGHT/2.0f));
  rightPlayer=box(interval(SCENE_WIDTH/2.0f-PLATFORM_WIDTH,SCENE_WIDTH/2.0f),interval(-PLATFORM_HEIGHT/2.0f,PLATFORM_HEIGHT/2.0f));
  background=box(interval(-SCENE_WIDTH/2.0f,SCENE_WIDTH/2.0f),interval(-SCENE_HEIGHT/2.0f,SCENE_HEIGHT/2.0f));
  ball=box(interval(-10,10),interval(-10,10));
  vibes::drawBox(background[1].inf,background[1].sup,background[2].inf,background[2].sup,"b");
}

void drawScene()
{
  iterations++;
  if(iterations%25==0)
  {
    vibes::clearFigure("Interval Pong");
    vibes::drawBox(background[1].inf,background[1].sup,background[2].inf,background[2].sup,"b");
  }
  vibes::drawBox(leftPlayer[1].inf,leftPlayer[1].sup,leftPlayer[2].inf,leftPlayer[2].sup,"y");
  vibes::drawBox(rightPlayer[1].inf,rightPlayer[1].sup,rightPlayer[2].inf,rightPlayer[2].sup,"r");
  vibes::drawCircle(ball[1].inf/2+ball[1].sup/2,ball[2].inf/2+ball[2].sup/2,(ball[2].sup-ball[2].inf)/2,"k[k]");
  vibes::axisLimits(-SCENE_WIDTH/2.0f,SCENE_WIDTH/2.0f,-SCENE_HEIGHT/2.0f,SCENE_HEIGHT/2.0f);
  crossSleepMs(SLEEP_MS);
}

void updateScene()
{
  ball=ball+BALL_SPEED*box(interval(cos(angle*3.14/180)),interval(sin(angle*3.14/180)));
  leftPlayer[2].inf=0.5*(SCENE_HEIGHT-PLATFORM_HEIGHT)*sin(2*3.14*LEFT_PLAYER_FREQ*SLEEP_MS*iterations/1000.0f)-PLATFORM_HEIGHT/2.0f;
  leftPlayer[2].sup=0.5*(SCENE_HEIGHT-PLATFORM_HEIGHT)*sin(2*3.14*LEFT_PLAYER_FREQ*SLEEP_MS*iterations/1000.0f)+PLATFORM_HEIGHT/2.0f;
  rightPlayer[2].inf=0.5*(SCENE_HEIGHT-PLATFORM_HEIGHT)*sin(2*3.14*RIGHT_PLAYER_FREQ*SLEEP_MS*iterations/1000.0f)-PLATFORM_HEIGHT/2.0f;
  rightPlayer[2].sup=0.5*(SCENE_HEIGHT-PLATFORM_HEIGHT)*sin(2*3.14*RIGHT_PLAYER_FREQ*SLEEP_MS*iterations/1000.0f)+PLATFORM_HEIGHT/2.0f;
  checkState();
  drawScene();
}

void checkState()
{
  if(timer>TIMER_TRESHOLD)
  {
  angle+=360;
  angle=((int)angle%360);
  if(!Subset(ball,background))
  {
    timer=0;
    if(!Inter(ball,topBorder).IsEmpty())
    {
      if(angle<90&&angle>=0)
        angle=-angle;
      else if(angle>=90&&angle<180)
        angle=360-angle;
    }
    else if(!Inter(ball,bottomBorder).IsEmpty())
    {
      if(angle>=180&&angle<270){
        angle=360-angle;
        }
      else if(angle>=270&&angle<360)
      {
        angle=(360-angle);
        }
    }
    else if(!Inter(ball,leftBorder).IsEmpty())
    {
      loseAnimation();
    }
    else if(!Inter(ball,rightBorder).IsEmpty())
    {
      loseAnimation();
    }
  }
  else if(!Inter(ball,leftPlayer).IsEmpty())
  {
    timer=0;
    if(angle<180)
      angle=(180-angle);
    else if(angle>=180)
      angle=(180-angle);
  }
  else if(!Inter(ball,rightPlayer).IsEmpty())
  {
    if(angle<90)
      angle=180-angle;
    else if(angle<360)
      angle=180+angle;
    timer=0;
  }
  }
}

void loseAnimation()
{
  ball=box(interval(-10,10),interval(-10,10));
  angleInit+=30;
  angle=angleInit;
}

void crossSleepMs(int sleepMs)
{
  #ifdef UNIX
    usleep(sleepMs*1000);
  #endif
  #ifdef WIN32
    Sleep(sleepMs);
  #endif
  timer+=sleepMs;
}
