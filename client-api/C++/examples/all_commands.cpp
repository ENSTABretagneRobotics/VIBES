#include "../src/vibes.h"
#include <iostream>

int main()
{
  vibes::connect();
  std::cout << "open" << std::endl;
  vibes::figure();
  std::cout << "figure" << std::endl;
  vibes::figure("figureTest");
  std::cout << "figuretest"<<std::endl;
  vibes::drawBox(0,1,0,1);
  std::cout << "drawBox" << std::endl;
  vibes::drawBox(0,1,0,1,'r');
  std::cout << "drawBos with color" << std::endl;
  vibes::disconnect();
  std::cout << "disconnect" << std::endl;
  return 0;
}
