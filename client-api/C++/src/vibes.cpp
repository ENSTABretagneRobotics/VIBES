#include "vibes.h"
#include <sstream>
#include <cstdlib>
#include <cstdio>

///
/// Vibes properties key,value system implementation
///

namespace vibes {
    std::string Value::toJSONString() const {
        std::ostringstream ss;
        switch (_type) {
        case vt_integer:
            ss<<_integer; break;
        case vt_decimal:
            ss<<_decimal; break;
        case vt_string:
            ss<<'"'<<_string<<'"'; break;
        case vt_array:
            ss << '[';
            for (std::vector<Value>::const_iterator it = _array.begin(); it != _array.end(); ++it) {
                if (it != _array.begin()) ss << ',';
                ss << it->toJSONString();
            }
            ss << ']';
            break;
        case vt_object:
            ss << '{' << _object->toJSON() << '}';
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
            ss << (it==_values.begin()?"":", ") << "\"" << it->first << "\":" << it->second.toJSONString();
        return ss.str();
    }

    Value Params::pop(const std::string &key, const Value &value_not_found) {
        KeyValueMap::iterator it = _values.find(key);
        // Return empty value if not found
        if (it == _values.end())
            return value_not_found;
        // Otherwise, return corresponding value and remove it from map
        Value val = it->second;
        _values.erase(it);
        return val;
    }
}

///
/// Vibes messaging implementation
///

using namespace std;

namespace vibes
{
///
/// \section Global variables and utility functions
///

  /// Current communication file descriptor
  FILE *channel=0;

  /// Current figure name (client maintained state)
  string current_fig="default";

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
  /// \section Figure management
  ///

  void figure(const std::string &figureName)
  {
    std::string msg;
    if (!figureName.empty()) current_fig = figureName;
    msg ="{\"action\":\"new\","
          "\"figure\":\""+(figureName.empty()?current_fig:figureName)+"\"}\n\n";
    fputs(msg.c_str(),channel);
    fflush(channel);
  }
  
  void clear(const std::string &figureName)
  {
    std::string msg;
    msg="{\"action\":\"clear\","
         "\"figure\":\""+(figureName.empty()?current_fig:figureName)+"\"}\n\n";
    fputs(msg.c_str(),channel);
    fflush(channel);
  }

  void saveImage(const std::string &fileName, const std::string &figureName)
  {
      std::string msg;
      msg="{\"action\":\"export\","
           "\"figure\":\""+(figureName.empty()?current_fig:figureName)+"\","
           "\"file\":\""+fileName+"\"}\n\n";
      fputs(msg.c_str(),channel);
      fflush(channel);
  }

  ///
  /// \section View settings
  ///

  void axisAuto(Params params)
  {
     // { "action": "view", "figure": "Fig2", "box": "auto" }

    if (params["figure"].empty()) params["figure"] = current_fig;
    params["action"] = "view";
    params["box"] = "auto";

    fputs(Value(params).toJSONString().append("\n\n").c_str(),channel);
    fflush(channel);
  }

  void axisLimits(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, Params params)
  {
    //{ "action": "view", "figure": "Fig1", "box": [-6, -2, -3, 1] }

    if (params["figure"].empty()) params["figure"] = current_fig;
    params["action"] = "view";
    params["box"] = (Vec<double,4>){x_lb,x_ub,y_lb,y_ub};

    fputs(Value(params).toJSONString().append("\n\n").c_str(),channel);
    fflush(channel);
  }

  ///
  /// \section Drawing functions
  ///

  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, Params params)
  {
    Params msg;
    msg["action"] = "draw";
    msg["figure"] = params.pop("figure",current_fig);
    msg["shape"] = (params, "type", "box", "bounds", (Vec<double,4>){x_lb,x_ub,y_lb,y_ub});

    fputs(Value(msg).toJSONString().append("\n\n").c_str(),channel);
    fflush(channel);
  }


  void drawEllipse(const double &cx, const double &cy, const double &a, const double &b, const double &rot, Params params)
  {
      std::stringstream msg;
      msg << "{\"action\":\"draw\","
             "\"shape\":{\"type\":\"ellipse\","
                        "\"center\":["<<cx<<","<<cy<<"],"
                        "\"axis\":["<<a<<","<<b<<"],"
                        "\"orientation\":"<<rot<<"}}\n\n";
      fputs(msg.str().c_str(),channel);
      fflush(channel);
  }

  void drawConfidenceEllipse(const double &cx, const double &cy,
                             const double &sxx, const double &sxy, const double &syy,
                             const double &sigma, Params params)
  {
      std::stringstream msg;
      msg<<"{\"action\":\"draw\","
           "\"shape\":{\"type\":\"ellipse\","
                      "\"center\":["<<cx<<","<<cy<<"],"
                      "\"covariance\":["<<sxx<<","<<sxy<<","<<sxy<<","<<syy<<"],"
                      "\"sigma\":"<<sigma<<"}}\n\n";
      fputs(msg.str().c_str(),channel);
      fflush(channel);
  }
}
