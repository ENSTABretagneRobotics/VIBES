// This file is part of VIBes' C++ API examples
//
// Copyright (c) 2013-2014 Jeremy Nicola, Vincent Drevelle
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
  // Not needed anymore: vibes::beginDrawing();           // <== Initializes the VIBES "connection"
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
  
  // Not needed anymore: vibes::endDrawing();      // <== closes the VIBES "connection"
  return 0;
}
