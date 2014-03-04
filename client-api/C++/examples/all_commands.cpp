#include "vibes.h"
#include <iostream>
#include <ctime>

int main()
{
    using namespace std;
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

  // Testing Vibes params system
  cout << "Testing Vibes params system" << endl;
  cout << "1." << endl;
  vibes::Params p1 = VibesParams("LineWidth",5,"Parent",9.8897f,"Name","this is my name","Color",(vibes::RGB){14,45,255});
  cout << VibesParams("action", "draw", "figure", "fig_name", "shape", (vibes::Value)p1).toJSON() << endl;

  cout << "2." << endl;
  cout << (VibesParams("LineWidth",6, "Yes",1) & p1).toJSON() << endl;

  cout << "3." << endl;
/*  vibes::Params p2 = VibesParams("action", "draw",
                           "figure", "fig_name",
                           "shape", VibesParams("LineWidth",5,
                                                "Parent",9.8897f,
                                                "Name","this is my name",
                                                "Color",(vibes::RGB){14,45,255}
                                                )
                           );
  std::cout << p2.toJSON() << std::endl;*/
/*  clock_t t1, t2;
  const int nb_bench = 100000;
  printf ("Benchmarking: drawing %d boxes...\n", nb_bench);
    t1 = clock();
    for (int i=0; i<nb_bench; ++i)
        vibes::drawBox(0,0,0,0,"b");
    t1 = clock() - t1;

    t2 = clock();
    vibes::Params p("color","r");
    for (int i=0; i<nb_bench; ++i)
        vibes::drawBox(0,0,0,0,"b",p);
    t2 = clock() - t2;

    printf ("Classic draw took %d clicks (%f seconds).\n",t1,((float)t1)/CLOCKS_PER_SEC);
    printf ("Param based draw took %d clicks (%f seconds).\n",t2,((float)t2)/CLOCKS_PER_SEC);*/
  return 0;
}
