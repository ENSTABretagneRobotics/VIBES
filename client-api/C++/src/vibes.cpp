#include "vibes.h"
#include <sstream>

namespace vibes
{    
  FILE *channel;
  /**
  * Connects to the named pipe, not implemented yet.
  */
  void connect()
  {
    channel=fopen("vibes.json","a");
  }
  
  void connect(std::string fileName)
  {
    channel=fopen(fileName.c_str(),"a");
  }
  
  void disconnect()
  {
    fclose(channel);
  }
  
  void figure()
  {
    std::string msg;
    msg ="{\"action\":\"new\",\"figure\":\"default\"}\n\n";
    fputs(msg.c_str(),channel);
  }
  
  void figure(std::string figureName)
  {
    std::string msg;
    msg ="{\"action\":\"new\",\"figure\":\""+figureName+"\"}\n\n";
    fputs(msg.c_str(),channel);
  }
  
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub)
  {
    std::stringstream msg;
    msg<<"{\"action\":\"draw\",\"figure\":\"default\",\"shape\":{\"type\":\"box\",\"bounds\":["<<x_lb<<","<<x_ub<<","<<y_lb<<","<<y_ub<<"]}}\n\n";
    fputs(msg.str().c_str(),channel);
  }
  
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, char color)
  {
    std::stringstream msg;
    msg<<"{\"action\":\"draw\",\"figure\":\"default\",\"shape\":{\"type\":\"box\",\"color\":\""<<color<<"\",\"bounds\":["<<x_lb<<","<<x_ub<<","<<y_lb<<","<<y_ub<<"]}}\n\n";
    fputs(msg.str().c_str(),channel);
  }
}
