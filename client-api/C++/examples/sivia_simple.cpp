#include "vibes.h"                                                     // <== Includes the VIBES C++ API
#include "interval.h"
#include "box.h"
#include <list>
#define EPSILON 0.1

using namespace std;

interval dist(box robot, box landmark){return Sqrt(Sqr(robot[1]-landmark[1])+Sqr(robot[2]-landmark[2]));}

int main()
{
  vibes::connect();                                                    // <== Initializes the VIBES "connection"
  box robot(interval(-100,100),interval(-100,100));
  box landmark(interval(0),interval(0));
  
  interval range(10,18);
  interval test(13.6,14.4);
  
  list<box> l;l.push_back(robot);
  
  while(!l.empty())
  {
    box top = l.front();l.pop_front();
    
    if(Inter(range,dist(top,landmark)).IsEmpty())
      vibes::drawBox(top[1].inf,top[1].sup,top[2].inf,top[2].sup,'b'); // <== draws outter boxes
    else if(Subset(dist(top,landmark),range))
      vibes::drawBox(top[1].inf,top[1].sup,top[2].inf,top[2].sup,'r'); // <== draws inner boxes
    else if(Width(top)<EPSILON)
      vibes::drawBox(top[1].inf,top[1].sup,top[2].inf,top[2].sup,'y'); // <== draws undeterminated boxes
    else
    {
      box b1,b2;
      Bisect(top,b1,b2);
      l.push_back(b1);l.push_back(b2);
    }
  }
  
  vibes::disconnect();                                                 // <== closes the VIBES "connection"
  return 0;
}
