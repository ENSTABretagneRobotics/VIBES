#include "vibes.h"
#include <iostream>

namespace vibes {
    int DrawTruc(int dummy, int dummy2, const Params &p = Params()) {
        std::cout << p.toJSON() << '\n';
        return p.size();
    }
}

int main()
{
  vibes::connect();
  std::cout << "open" << std::endl;
  vibes::figure();
  std::cout << "figure" << std::endl;
  vibes::figure("figureTest");
  std::cout << "figuretest"<<std::endl;
  vibes::drawBox(0,1.245,0,1);
  std::cout << "drawBox" << std::endl;
  vibes::drawBox(0,1,0,1.879,"r");
  std::cout << "drawBox with color" << std::endl;
  vibes::disconnect();
  std::cout << "disconnect" << std::endl;
  std::cout << vibes::DrawTruc(666, 6666, VibesParams("LineWidth",5,"Parent",9.8897f,"Name","this is my name","Color",(vibes::RGB){14,45,255}));
  return 0;
}
