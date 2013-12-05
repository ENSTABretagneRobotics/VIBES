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
    connect("vibes.json");
  }
  
  void connect(const std::string &fileName)
  {
    channel=fopen(fileName.c_str(),"a");
  }
  
  void disconnect()
  {
    fclose(channel);
  }
  
  void figure(const std::string &figureName)
  {
    std::string msg;
    msg ="{\"action\":\"new\",\"figure\":\""+figureName+"\"}\n\n";
    fputs(msg.c_str(),channel);
  }
  
  void figure()
  {
    figure("default");
  }
  void clear()
  {
    clear("default");
  }
  
  void clear(const std::string &figureName)
  {
    std::string msg;
    msg="{\"action\":\"clear\",\"figure\":\""+figureName+"\"}\n\n";
    fputs(msg.c_str(),channel);
  }

  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, std::string figureName, char color)
  {
    std::stringstream msg;
    msg<<"{\"action\":\"draw\",\"figure\":\""+figureName+"\",\"shape\":{\"type\":\"box\",\"color\":\""<<color<<"\",\"bounds\":["<<x_lb<<","<<x_ub<<","<<y_lb<<","<<y_ub<<"]}}\n\n";
    fputs(msg.str().c_str(),channel);
  }
  
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, char color)
  {
    drawBox(x_lb,x_ub,y_lb,y_ub,"default",color);
  }

  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub)
  {
    drawBox(x_lb,x_ub,y_lb,y_ub,"default",'b');
  }
}
