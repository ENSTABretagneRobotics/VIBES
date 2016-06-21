/**
* \file   all_commands.cpp
* \author Vincent Drevelle, Jeremy Nicola, Simon Rohou, Benoit Desrochers
* \date   2013-2015
*
* \brief  A simple C++ program testing different Vibes functions.
**/

// This file is part of VIBes' C++ API examples
//
// Copyright (c) 2013-2015 Vincent Drevelle, Jeremy Nicola, Simon Rohou, 
//                         Benoit Desrochers
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

#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <cstdlib>

// Utility macro to log executed instructions on terminal
#define VIBES_TEST(instruction) \
{   clock_t t = clock(); \
    std::cout << "[l." << __LINE__ << "@" << ((float)t)/CLOCKS_PER_SEC << "s] " << #instruction << "..."; \
    t = clock(); \
    instruction; \
    t = clock() - t; \
    std::cout << " DONE (" << ((float)t)/CLOCKS_PER_SEC << "s)" << std::endl; \
}

int main()
{
    using namespace std;
    cout << "-------------------------------" "\n"
            "| VIBES C++ API commands test |" "\n"
            "-------------------------------" << endl;

    VIBES_TEST( vibes::beginDrawing() );

    cout << "Figure creation function" << endl;
    VIBES_TEST( vibes::newFigure() );

    cout << "Test bug GitHub #58" << endl;
    VIBES_TEST( vibes::newGroup("group","[red]darkRed") );
    //VIBES_TEST( vibes::drawCircle(0.0,0.0,1.0,vibesParams("group","red group")) );
    //VIBES_TEST( vibes::clearGroup("group") );
    VIBES_TEST( vibes::clearFigure() );

    std::cout << "Testing Vibes Custom Colors ans Transparency" << std::endl;
    VIBES_TEST( vibes::newFigure("CustomColor") );
    VIBES_TEST( vibes::drawBox(-1,0,-2,-1, "[red]"));
    VIBES_TEST( vibes::drawBox(0,1,0,1, "#FF3300AA[#00345678]"));
    VIBES_TEST( vibes::drawBox(0.5,1.5,0.5,1.5, "#FF0022AA[#FF0056AA]"));

    cout << "Megatest Wayne!" << std::endl;
    {
        const int nbBoxesMegaTest = 10000;
        const int dimBoxesMegaTest = 4;

        std::vector< std::vector<double> > boxes_bounds;
        std::vector<double> box_bounds(dimBoxesMegaTest*2);
        for (int i=0; i<nbBoxesMegaTest; ++i)
        {
            for (int j=0; j<box_bounds.size(); j++)
            {
                if (! (j%2))
                    box_bounds[j] = 25.0 * rand() / RAND_MAX;
                else
                    box_bounds[j] = box_bounds[j-1] + 5.0 * rand() / RAND_MAX;
            }
            boxes_bounds.push_back(box_bounds);
        }
        VIBES_TEST( vibes::newFigure("Megatest with boxes") );
        VIBES_TEST( vibes::drawBoxes(boxes_bounds,"[darkYellow]") );
        VIBES_TEST( vibes::setFigureProperties(vibesParams("x",0,"y",40,"width",150,"height",150)) );

        VIBES_TEST( vibes::newFigure("Megatest with boxes union") );
        VIBES_TEST( vibes::drawBoxesUnion(boxes_bounds,"[darkGreen]") );
        VIBES_TEST( vibes::setFigureProperties(vibesParams("x",150,"y",40,"width",150,"height",150)) );
    }

    cout << "Test of groups" << std::endl;
    {
        const int nbBoxesMegaTest = 10;
        const int dimBoxesMegaTest = 3;

        VIBES_TEST( vibes::newFigure("Groups: red and blue") );
        VIBES_TEST( vibes::setFigureProperties(vibesParams("x",300,"y",40,"width",150,"height",150)) );

        VIBES_TEST( vibes::newGroup("red group","[red]darkRed") );
        VIBES_TEST( vibes::newGroup("blue group","darkBlue[blue]") );

        VIBES_TEST( vibes::drawCircle(0.0,0.0,1.0,vibesParams("group","red group")) );
        VIBES_TEST( vibes::clearGroup("red group") );
        cout << " Clear empty group" << endl;
        VIBES_TEST( vibes::clearGroup("red group") );
        VIBES_TEST( vibes::clearGroup("undefined group") );

        std::vector<double> box_bounds(dimBoxesMegaTest*2);
        for (int i=0; i<nbBoxesMegaTest; ++i)
        {
            for (int j=0; j<box_bounds.size(); j++)
            {
                if (! (j%2))
                    box_bounds[j] = 25.0 * rand() / RAND_MAX;
                else
                    box_bounds[j] = box_bounds[j-1] + 5.0 * rand() / RAND_MAX;
            }
            VIBES_TEST( vibes::drawBox(box_bounds,vibesParams("group","red group")) );
            for (int j=0; j<box_bounds.size(); j++)
            {
                if (! (j%2))
                    box_bounds[j] = 25.0 * rand() / RAND_MAX;
                else
                    box_bounds[j] = box_bounds[j-1] + 5.0 * rand() / RAND_MAX;
            }
            VIBES_TEST( vibes::drawBox(box_bounds,vibesParams("group","blue group")) );
        }
        VIBES_TEST( vibes::setObjectProperty("blue group","format","cyan[blue]") );
    }

    cout << "Plotting y=sin(x) and y=cos(x)" << std::endl;
    {
        const int nbPts = 1000;
        const double xStep = 0.01;

        std::vector< double > vect_x;
        std::vector< double > vect_y;
        std::vector< std::vector<double> > points;
        std::vector<double> point(3);
        for (int i=0; i<nbPts; ++i)
        {
            const double x = xStep * i;
            point[0] = x;
            point[1] = sin(x);
            point[2] = cos(x);
            points.push_back(point);
            vect_x.push_back(x);
            vect_y.push_back(cos(x));
        }
        VIBES_TEST( vibes::newFigure("sin and cos") );
        VIBES_TEST( vibes::drawLine(points,"blue") );
        VIBES_TEST( vibes::drawLine(vect_x,vect_y,"red") );

        VIBES_TEST( vibes::axisAuto() );

        VIBES_TEST( vibes::setFigureProperties("sin and cos",
                                                vibesParams("x",0,"y",220,"width",450,"height",100)) );
        std::vector<std::string> labels;
        labels.push_back("x"); labels.push_back("sin x"); labels.push_back("cos x");
        VIBES_TEST( vibes::axisLabels(labels) );
    }


    VIBES_TEST( vibes::newFigure("figureTest") );

    cout << "drawBox" << std::endl;
    VIBES_TEST( vibes::drawBox(0,1.245,0,1) );

    cout << "drawBox with color" << std::endl;
    VIBES_TEST( vibes::drawBox(0,1,0,1.879,"[r]blue") );

    cout << "drawBox with macro (single optional args)" << std::endl;
    VIBES_TEST( vibesDrawBox(-2,-1,0,1, "red[gray]") );

    cout << "drawBox with macro (2 optional args)" << std::endl;
    VIBES_TEST( vibesDrawBox(-1,0,0,1,"FaceColor","green") );

    cout << "drawBox with vector of bounds" << std::endl;
    std::vector<double> bounds;
    bounds.push_back(-4); bounds.push_back(4);
    bounds.push_back(-5); bounds.push_back(-4.5);
    bounds.push_back(0); bounds.push_back(1);
    VIBES_TEST( vibes::drawBox(bounds,"[lightGray]") );


    VIBES_TEST( vibes::drawEllipse(-1,-1,2,3,30.0, vibesParams("name","ellipse")) );
    VIBES_TEST( vibes::removeObject("ellipse") );

    VIBES_TEST( vibesDrawEllipse(-1,-1,1.5,2,30.0, "parent",0.1, "g") );

    VIBES_TEST( vibes::drawEllipse(0,-4.75,4,0.25,0.0, "darkGray") );

    cout << "drawPolygon with vector of bounds" << std::endl;
    {
        vector<double> x, y;
        x.push_back(-2);    y.push_back(7);
        x.push_back(-1);    y.push_back(7);
        x.push_back(0);     y.push_back(5);
        x.push_back(1);     y.push_back(7);
        x.push_back(2);     y.push_back(7);
        x.push_back(0.5);   y.push_back(4);
        x.push_back(-0.5);  y.push_back(4);
        VIBES_TEST( vibes::drawPolygon(x, y, "yellow[red]") );
    }

    cout << "drawVehicle" << std::endl;
    VIBES_TEST( vibes::drawVehicle(4, 36, 52, 5, "green") ); // cx, cy, rotation, length
    VIBES_TEST( vibes::drawVehicle(-5, 20, 0, 3, "blue[lightGray]") );

    cout << "drawAUV" << std::endl;
    VIBES_TEST( vibes::drawAUV(22, -16, 52, 5, "blue[yellow]") );
    VIBES_TEST( vibes::drawAUV(40, 0, 0, 3, "blue[green]") );

    cout << "drawArrow" << std::endl;
    VIBES_TEST( vibes::drawArrow(20., 0., 40., 20., 1., "black[yellow]") );
    VIBES_TEST( vibes::drawArrow(40., 30., 20., 10., 2., "black[yellow]") );
    VIBES_TEST( vibes::drawArrow(20., 20., 40., 40., 3., "black[yellow]") );
    VIBES_TEST( vibes::drawArrow(40., 50., 20., 30., 4., "black[yellow]") );

    VIBES_TEST( vibes::drawArrow(20., 65., 40., 65., 2., "black[red]") );
    VIBES_TEST( vibes::drawArrow(40., 62., 20., 62., 2., "black[red]") );

    VIBES_TEST( vibes::drawArrow(65., 70., 65., 50., 2., "black[red]") );
    VIBES_TEST( vibes::drawArrow(62., 50., 62., 70., 2., "black[red]") );

    cout << "drawSector" << std::endl;
    VIBES_TEST( vibes::drawSector(0,0,3,3,20, 120, "black[red]") );
    VIBES_TEST( vibes::drawSector(0,0,3,1,-20, -220, "black[red]") );
    VIBES_TEST( vibes::drawSector(10,-3,3,3,700, 800, "black[red]") );
    VIBES_TEST( vibes::drawSector(0,0,3,3,20, 120, "black[red]") ); 

    cout << "drawPie" << std::endl;
    VIBES_TEST( vibes::drawPie(0,-20,3,4,20, 120, "black[red]") );
    VIBES_TEST( vibes::drawPie(0,-20,3,7,-20, -220, "black[red]") );
    VIBES_TEST( vibes::drawPie(10,-20,3,4,700, 800, "black[red]") );
    VIBES_TEST( vibes::drawPie(5,-20,3,3,20, 120, "black[red]") ); 
    
    cout << "drawRing"<< endl;
    VIBES_TEST( vibes::drawRing(42,42,20,23,"black[red]"));

    cout << "drawBoxes with vector of vector of bounds" << std::endl;
    {
        std::vector< std::vector<double> > boxes_bounds;
        std::vector<double> box_bounds(4*2);
        for (int i=0; i<10; ++i)
        {
            for (int j=0; j<box_bounds.size(); j++)
            {
                if (! (j%2))
                    box_bounds[j] = pow(i, 1.0 / ((j/2)+1));
                else
                    box_bounds[j] = pow(i+1.0, 1.0 / ((j/2)+1));;
            }
            boxes_bounds.push_back(box_bounds);
        }
        VIBES_TEST( vibes::drawBoxes(boxes_bounds,"red[y]") );
    }

    cout << "drawPoint"<<endl;
    VIBES_TEST(vibes::drawPoint(17,17,"red[darkyellow]"));
    VIBES_TEST(vibes::drawPoint(27,27,2,vibesParams("FaceColor","magenta","EdgeColor","none","Draggable",true,"FixedScale",false)));
    
    VIBES_TEST( vibes::axisAuto() );
    //  VIBES_TEST( vibes::axisLimits(-1,1, -3,2) );

    VIBES_TEST( vibes::saveImage("vibes_test.png") );
    VIBES_TEST( vibes::saveImage("vibes_test.jpg") );
    VIBES_TEST( vibes::saveImage("vibes_test.bmp") );
    VIBES_TEST( vibes::saveImage("vibes_test.svg") );
    
    vibes::newFigure("Points");
    vector<double> x,y;
    for(unsigned int i=0;i<500;i++)
    {
        x.push_back(25.0*rand()/RAND_MAX);
        y.push_back(25.0*rand()/RAND_MAX);
    }
    
    vibes::drawPoints(x,y,vibesParams("FaceColor","blue","EdgeColor","darkBlue","Draggable",true));
    
    x.clear();
    y.clear();
    
    for(unsigned int i=0;i<500;i++)
    {
        x.push_back(34+25.0*rand()/RAND_MAX);
        y.push_back(40+25.0*rand()/RAND_MAX);
    }
    
    vibes::drawPoints(x,y,vibesParams("FaceColor","red","EdgeColor","darkRed","Radius",100));
    
    std::cout << "end drawing" << std::endl;
    VIBES_TEST( vibes::endDrawing() );

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
