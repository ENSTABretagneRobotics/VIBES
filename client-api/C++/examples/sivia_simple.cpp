#include "vibes.h"              // <== Includes the VIBES C++ API
#include "interval.h"
#include "box.h"
#include <list>
const double EPSILON = 0.1;

using namespace std;

interval dist(box robot, box landmark)
{
    return Sqrt(Sqr(robot[1]-landmark[1])+Sqr(robot[2]-landmark[2]));
}

int main()
{
  vibes::beginDrawing();           // <== Initializes the VIBES "connection"
  vibes::newFigure("SIVIA");       // <== Create a new VIBes figure

  box robot(interval(-10,10),interval(-10,10));
  box landmark(interval(0),interval(0));
  
  interval range(6.0,8.5);
  
  list<box> l;l.push_back(robot);
  
  while(!l.empty())
  {
    box top = l.front();l.pop_front();
    
    if(Inter(range,dist(top,landmark)).IsEmpty())
      // draw outter boxes
      vibes::drawBox(top[1].inf,top[1].sup,top[2].inf,top[2].sup,"blue");
    else if(Subset(dist(top,landmark),range))
      // draw inner boxes
      vibes::drawBox(top[1].inf,top[1].sup,top[2].inf,top[2].sup,"red");
    else if(Width(top)<EPSILON)
      // draw undeterminated boxes
      vibes::drawBox(top[1].inf,top[1].sup,top[2].inf,top[2].sup,"yellow");
    else
    {
      box b1,b2;
      Bisect(top,b1,b2);
      l.push_back(b1);l.push_back(b2);
    }
  }
  
  vibes::endDrawing();      // <== closes the VIBES "connection"
  return 0;
}
