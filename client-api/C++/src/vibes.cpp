#include "vibes.h"
#include <sstream>
#include <cstdlib>

///
/// Vibes properties key,value system implementation
///

namespace vibes {
    std::string Value::toJSONString() const {
        std::ostringstream ss;
        switch (type) {
        case vt_integer:
            ss<<integer; break;
        case vt_decimal:
            ss<<decimal; break;
        case vt_string:
            ss<<'"'<<string<<'"'; break;
        case vt_array:
            ss << '[';
            for (std::vector<Value>::const_iterator it = array.begin(); it != array.end(); ++it) {
                if (it != array.begin()) ss << ',';
                ss << it->toJSONString();
            }
            ss << ']';
            break;
        case vt_none:
        default:
            break;
        }
        return ss.str();
    }

    std::string Params::toJSON() const {
        std::ostringstream ss;
        for(std::map<std::string, Value>::const_iterator it = _values.begin(); it != _values.end(); ++it)
            ss << ", \"" << it->first << "\" = " << it->second.toJSONString();
        return ss.str();
    }

    Params::NameHelper Params::operator, (const std::string &s) { return NameHelper(*this, s); }
}

///
/// Vibes messaging implementation
///

namespace vibes
{
///
/// \section Global variables
///

  /// Current communication file descriptor
  FILE *channel=0;

  /// Current figure name (client maintained state)
  std::string current_fig="default";

  ///
  /// \section Management of connection to the Vibes server
  ///

  /**
  * Connects to the named pipe, not implemented yet.
  */
  void connect()
  {
      // Retrieve user-profile directory from envirnment variable
      char * user_dir = getenv("USERPROFILE"); // Windows
      if (!user_dir)
          user_dir = getenv("HOME"); // POSIX
      if (user_dir)
      { // Environment variable found, connect to a file in user's profile directory
          std::string file_name(user_dir);
          file_name.append("/.vibes.json");
          connect(file_name);
      }
      else
      { // Connect to a file in working directory
          connect("vibes.json");
      }
  }
  
  void connect(const std::string &fileName)
  {
    channel=fopen(fileName.c_str(),"a");
  }
  
  void disconnect()
  {
    fclose(channel);
  }

  ///
  /// \section Drawing functions
  ///

  void figure(const std::string &figureName)
  {
    std::string msg;
    current_fig = figureName;
    msg ="{\"action\":\"new\",\"figure\":\""+figureName+"\"}\n\n";
    fputs(msg.c_str(),channel);
    fflush(channel);
  }
  
  void figure()
  {
    figure(current_fig);
  }
  void clear()
  {
    clear(current_fig);
  }
  
  void clear(const std::string &figureName)
  {
    std::string msg;
    msg="{\"action\":\"clear\",\"figure\":\""+figureName+"\"}\n\n";
    fputs(msg.c_str(),channel);
    fflush(channel);
  }

  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, const std::string &figureName, const std::string &color)
  {
    std::stringstream msg;
    msg<<"{\"action\":\"draw\",\"figure\":\""+figureName+"\",\"shape\":{\"type\":\"box\",\"color\":\""<<color<<"\",\"bounds\":["<<x_lb<<","<<x_ub<<","<<y_lb<<","<<y_ub<<"]}}\n\n";
    fputs(msg.str().c_str(),channel);
    fflush(channel);
  }
  
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, const std::string &color)
  {
    drawBox(x_lb,x_ub,y_lb,y_ub,current_fig,color);
  }
  
  void saveImage(const std::string &fileName)
  {
      saveImage(fileName, current_fig);
  }

  void saveImage(const std::string &fileName, const std::string &figureName)
  {
      std::string msg;
      msg="{\"action\":\"export\",\"figure\":\""+figureName+"\",\"file\":\""+fileName+"\"}\n\n";
      fputs(msg.c_str(),channel);
      fflush(channel);
  }

  void drawEllipse(const double &cx, const double &cy, const double &sxx, const double &sxy, const double &syy, const std::string &figureName)
  {
      std::stringstream msg;
      msg<<"{\"action\":\"draw\",\"figure\":\""+figureName+"\",\"shape\":{\"type\":\"ellipse\",\"center\":["<<cx<<","<<cy<<"],\"covariance\":["<<sxx<<","<<sxy<<","<<sxy<<","<<syy<<"],\"sigma\":5}}\n\n";
      fputs(msg.str().c_str(),channel);
      fflush(channel);
  }

  void drawEllipse(const double &cx, const double &cy, const double &sxx, const double &sxy, const double &syy)
  {
      drawEllipse(cx,cy,sxx,sxy,syy,current_fig);
  }
}
