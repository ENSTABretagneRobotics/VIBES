/**
* \file all_commands.cpp
* \author Vincent Drevelle and Jeremy Nicola
* \date 2013-2014
*
* \brief A simple C++ program testing different Vibes functions.
**/

#include "vibes.h"

#include <iostream>
#include <ctime>

// Utility macro to log executed instructions on terminal
#define VIBES_TEST(instruction) std::cout << "[l." << __LINE__ << "] " << #instruction << "..."; instruction; std::cout << " OK" << std::endl;

int main()
{
    using namespace std;
    cout << "-------------------------------" "\n"
            "| VIBES C++ API commands test |" "\n"
            "-------------------------------" << endl;

    VIBES_TEST( vibes::connect() );

    cout << "Figure creation function" << endl;
    VIBES_TEST( vibes::figure() );
    VIBES_TEST( vibes::figure("figureTest") );

    cout << "drawBox" << std::endl;
    VIBES_TEST( vibes::drawBox(0,1.245,0,1) );

    cout << "drawBox with color" << std::endl;
    VIBES_TEST( vibes::drawBox(0,1,0,1.879,"r") );

    cout << "drawBox with macro (single optional args)" << std::endl;
    VIBES_TEST( vibesDrawBox(-2,-1,0,1, "gray") );

    cout << "drawBox with macro (2 optional args)" << std::endl;
    VIBES_TEST( vibesDrawBox(-1,0,0,1,"color","green") );

    VIBES_TEST( vibes::axisAuto() );
    //  VIBES_TEST( vibes::axisLimits(-1,1, -3,2) );

    VIBES_TEST( vibes::saveImage("vibes_test.png") );
    VIBES_TEST( vibes::saveImage("vibes_test.jpg") );
    VIBES_TEST( vibes::saveImage("vibes_test.bmp") );
    VIBES_TEST( vibes::saveImage("vibes_test.svg") );

    std::cout << "disconnect" << std::endl;
    VIBES_TEST( vibes::disconnect() );

    // Testing Vibes params system
    cout << "Testing Vibes params system" << endl;
    cout << "1." << endl;
    vibes::Params p1 = vibesParams("LineWidth",5,"Parent",9.8897f,"Name","this is my name","Color",(vibes::RGB){14,45,255});
    cout << vibesParams("action", "draw", "figure", "fig_name", "shape", (vibes::Value)p1).toJSON() << endl;

    cout << "2." << endl;
    cout << (vibesParams("LineWidth",6, "Yes",1) & p1).toJSON() << endl;

    cout << "3." << endl;
    /*  vibes::Params p2 = vibesParams("action", "draw",
                           "figure", "fig_name",
                           "shape", vibesParams("LineWidth",5,
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
