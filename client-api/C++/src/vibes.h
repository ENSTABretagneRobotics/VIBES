#ifndef VIBES_CPP_API_H
#define VIBES_CPP_API_H

#include <vector>
#include <string>
#include <map>
#include <sstream>

//#define VIBES_DEBUG_API
#ifdef VIBES_DEBUG_API
#include <iostream>
#define VIBES_DEBUG(i) std::cout << "VibesDebug " << __FILE__ << "(" << __LINE__ << ") : " << i << std::endl;
#else
#define VIBES_DEBUG(i)
#endif

namespace vibes {class Params;
    /// Value class hold any type supported by vibes properties system, an provides JSON serialization
    class Value {
        enum value_type_enum{
            vt_none, vt_integer, vt_string, vt_decimal, vt_array, vt_object
        } _type;

        union {
        int _integer;
        double _decimal;
        const Params *_object;
        };
        std::string _string;
        std::vector<Value> _array;

    public:
        Value() : _type(vt_none) {}
        Value(int i) : _integer(i), _type(vt_integer) {}
        Value(const double &d) : _decimal(d), _type(vt_decimal) {}
        Value(const std::string &s) : _string(s), _type(vt_string) {}
        Value(const char *s) : _string(s), _type(vt_string) {}
        Value(const std::vector<Value> &a) : _array(a), _type(vt_array) {}
        /*explicit */Value(const Params &p) : _object(&p), _type(vt_object) {}
        bool empty() {return (_type == vt_none);}
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
        typedef std::map<std::string, Value> KeyValueMap;
        KeyValueMap _values;
    public:
        Params() {}
        template<typename T> Params(const std::string & name, const T &p) {_values[name] = p;}
        Value & operator[](const std::string &key) {return _values[key];}
        Value pop(const std::string &key, const Value &value_not_found = Value());
        NameHelper operator, (const std::string &s);
        Params& operator& (const Params &p) { for(KeyValueMap::const_iterator it = p._values.begin(); it != p._values.end(); ++it) _values[it->first] = it->second; return *this;}
        std::size_t size() const { return _values.size(); }
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

    inline Params::NameHelper Params::operator, (const std::string &s) { return NameHelper(*this, s); }


//#define VIBES_PARAMS_SUBCLASS(Name, type) \
//    class Name : public Params { public: Name(const type &val) : Params( #Name , val ) {} }
//    /* \
//    void set##Name(int vibesID, cont type &val) {} \
//    void set##Name(const char * vibesName, cont type &val) {} */

//    VIBES_PARAMS_SUBCLASS(Figure, std::string);
//    VIBES_PARAMS_SUBCLASS(Parent, std::string);
//    VIBES_PARAMS_SUBCLASS(Color, std::string);
//    VIBES_PARAMS_SUBCLASS(LineWidth, double);

/// Macro to simplify contruction of a Params object
#define VibesParams(...) (vibes::Params(), __VA_ARGS__)


  /**
  * Connects to the named pipe, not implemented yet.
  */
  void connect();
  void connect(const std::string &fileName);
  
  void disconnect();
  
  void figure(const std::string &figureName = std::string());
  void clear(const std::string &figureName = std::string());
  void saveImage(const std::string &fileName = std::string(), const std::string &figureName = std::string());
  
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, Params params);
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub,
               const std::string &color="b", Params params=Params());
  void drawEllipse(const double &cx, const double &cy, const double &a, const double &b, const double &rot,
                   const std::string &color="b", Params params=Params());
  void drawConfidenceEllipse(const double &cx, const double &cy, const double &sxx, const double &sxy, const double &syy,
                             const double &sigma=3.0, const std::string &color="b", Params params=Params());
  void drawCircle(const double &cx, const double &cy, const double &rad,
                  const std::string &color="b", Params params=Params());

}

#endif //#ifndef VIBES_CPP_API_H
