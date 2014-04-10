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
#include <vector>

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
    // Megatest Wayne!!!
    for (int i=0; i<10000; ++i)
    {
        std::vector<double> bounds(4*2);
        for (int j=0; j<bounds.size(); j++)
        {
            if (! (j%2))
                bounds[j] = 25.0 * rand() / RAND_MAX;
            else
                bounds[j] = bounds[j-1] + 5.0 * rand() / RAND_MAX;
        }
        vibes::drawBox(bounds,"lightGray");
    }

    VIBES_TEST( vibes::figure("figureTest") );

    cout << "drawBox" << std::endl;
    VIBES_TEST( vibes::drawBox(0,1.245,0,1) );

    cout << "drawBox with color" << std::endl;
    VIBES_TEST( vibes::drawBox(0,1,0,1.879,"r") );

    cout << "drawBox with macro (single optional args)" << std::endl;
    VIBES_TEST( vibesDrawBox(-2,-1,0,1, "gray") );

    cout << "drawBox with macro (2 optional args)" << std::endl;
    VIBES_TEST( vibesDrawBox(-1,0,0,1,"color","green") );

    cout << "drawBox with vector of bounds" << std::endl;
    std::vector<double> bounds;
    bounds.push_back(-4); bounds.push_back(4);
    bounds.push_back(-5); bounds.push_back(-4.5);
    bounds.push_back(0); bounds.push_back(1);
    VIBES_TEST( vibes::drawBox(bounds,"lightGray") );


    VIBES_TEST( vibes::drawEllipse(-1,-1,2,3,30.0) );
    VIBES_TEST( vibesDrawEllipse(-1,-1,1.5,2,30.0, "parent",0.1, "g") );

    VIBES_TEST( vibes::drawEllipse(0,-4.75,4,0.25,0.0, "darkGray") );



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
