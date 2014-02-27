#ifndef VIBES_CPP_API_H
#define VIBES_CPP_API_H

#include <vector>
#include <string>
#include <map>
#include <sstream>

namespace vibes {
    /// Value class hold any type supported by vibes properties system, an provides JSON serialization
    class Value {
        enum value_type_enum{
            vt_none, vt_integer, vt_string, vt_decimal, vt_array
        } type;

        int integer;
        std::string string;
        double decimal;
        std::vector<Value> array;

    public:
        Value() : type(vt_none) {}
        Value(int i) : integer(i), type(vt_integer) {}
        Value(const double &d) : decimal(d), type(vt_decimal) {}
        Value(const std::string &s) : string(s), type(vt_string) {}
        Value(const char *s) : string(s), type(vt_string) {}
        Value(const std::vector<Value> &a) : array(a), type(vt_array) {}
        std::string toJSONString() const;
    };

    /// Static array
    template<typename T, int N>
    struct Vec {
        T _data[N];
        operator Value() {return Value(std::vector<Value>(_data,_data+N));}
    };
    /// Define useful types for colors
    typedef Vec<int,3> RGB;
    typedef Vec<int,4> RGBA;

    /// Params holds a list of parameters, and provides JSON serialization
    class Params {
        class NameHelper;
        std::map<std::string, Value> _values;
    public:
        Params() {}
        template<typename T> Params(const std::string & name, const T &p) {_values[name] = p;}
        //template<typename T> Params(NameHelper &incomp, const T &p);
        Value & operator[](const std::string &key) {return _values[key];}
        NameHelper operator, (const std::string &s);
        Params& operator, (const Params &p) { for(std::map<std::string, Value>::const_iterator it = p.begin(); it != p.end(); ++it) _values[it->first] = it->second; return *this;}
        std::size_t size() const { return _values.size(); }
        typedef std::map<std::string, Value>::const_iterator iterator;
        iterator begin() const {return _values.begin();}
        iterator end() const {return _values.end();}
        std::string toJSON() const;
    };

    /// Params::NameHelper is a helper class for comma-separated key,value input of parameters
    class Params::NameHelper {
    public:
        Params &_params;
        std::string _name;
        NameHelper(Params & list, const std::string & name) : _params(list), _name(name) {}
        Params & operator, (const Value &value) { _params[_name] = value; return _params; }
    };

//#define VIBES_PARAMS_SUBCLASS(Name, type) \
//    class Name : public Params { public: Name(const type &val) : Params( #Name , val ) {} }
//    /* \
//    void set##Name(int vibesID, cont type &val) {} \
//    void set##Name(const char * vibesName, cont type &val) {} */

//    VIBES_PARAMS_SUBCLASS(Figure, std::string);
//    VIBES_PARAMS_SUBCLASS(Parent, std::string);
//    VIBES_PARAMS_SUBCLASS(Color, std::string);
//    VIBES_PARAMS_SUBCLASS(LineWidth, double);
}

#define VibesParams(...) (vibes::Params(), __VA_ARGS__)

namespace vibes
{
  //extern FILE *channel;  

  /**
  * Connects to the named pipe, not implemented yet.
  */
  void connect();
  void connect(const std::string &fileName);
  
  void disconnect();
  
  void figure();
  void figure(const std::string &figureName);
  
  void clear();
  void clear(const std::string &figureName);
  
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, const std::string &figureName, const std::string &color);
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, const std::string &color="b");

  void drawEllipse(const double &cx, const double &cy, const double &sxx, const double &sxy, const double &syy, const std::string &figureName);
  void drawEllipse(const double &cx, const double &cy, const double &sxx, const double &sxy, const double &syy);

  void saveImage(const std::string &fileName = std::string());
  void saveImage(const std::string &fileName, const std::string &figureName);
}

#endif //#ifndef VIBES_CPP_API_H
