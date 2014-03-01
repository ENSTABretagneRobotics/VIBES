#ifndef VIBES_CPP_API_H
#define VIBES_CPP_API_H

///
/// Vibes API configuration defines (def or undef as needed)
///

/// Generate "vibesFunctionName(arg1,arg2,argN,...)" macros for each Vibes function,
/// to allow Matlab style variadic calls with properies.
/// Also defines the "vibesParams(...)" macro to help specifying parameters.
/// Undef this if you don't want to define macros
#define VIBES_GENERATE_vibesXXX_MACROS

/// Verbose debugging functions (logs to standard output), only useful for library devs
#undef VIBES_DEBUG_API


///
/// Includes
///

#include <vector>
#include <string>
#include <map>
#include <sstream>

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
        #ifdef VIBES_GENERATE_vibesXXX_MACROS
        // Conversion of a singleton parameter to a color (for use with macros)
        operator Params&() {_params["color"] = _name; return _params;}
        #endif
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


  /**
  * Connects to the named pipe, not implemented yet.
  */
  void connect();
  void connect(const std::string &fileName);
  
  void disconnect();
  
  // Figure management
  void figure(const std::string &figureName = std::string());
  void clear(const std::string &figureName = std::string());
  void saveImage(const std::string &fileName = std::string(), const std::string &figureName = std::string());

  // Drawing functions
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, Params params);
  void drawEllipse(const double &cx, const double &cy, const double &a, const double &b, const double &rot, Params params=Params());
  void drawConfidenceEllipse(const double &cx, const double &cy, const double &sxx, const double &sxy, const double &syy,
                             const double &sigma=3.0, Params params=Params());
  inline void drawCircle(const double &cx, const double &cy, const double &r, const Params &params=Params()) { drawEllipse(cx,cy,r,r,0.,params); }

  // Convenience drawing functions with color selection
#define VIBES_FUNC_COLOR_PARAM_DDDD_IMPL(func_name) \
  inline void func_name(const double &a, const double &b, const double &c, const double &d, \
              const std::string &color="b", Params params=Params()) {func_name(a,b,c,d,(params,"color",color));}


VIBES_FUNC_COLOR_PARAM_DDDD_IMPL(drawBox)

/*
  inline void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub,
               const std::string &color="b", Params params=Params()) {drawBox(x_lb,x_ub,y_lb,y_ub,(params,"color",color));}
*/
  inline void drawEllipse(const double &cx, const double &cy, const double &a, const double &b, const double &rot,
                   const std::string &color="b", Params params=Params()) {drawEllipse(cx,cy,a,b,rot,(params,"color",color));}

  inline void drawConfidenceEllipse(const double &cx, const double &cy, const double &sxx, const double &sxy, const double &syy,
                             const double &sigma=3.0, const std::string &color="b", Params params=Params()) {
                             drawConfidenceEllipse(cx,cy,sxx,sxy,syy,sigma,(params,"color",color));}

  inline void drawCircle(const double &cx, const double &cy, const double &r,
                  const std::string &color="b", const Params &params=Params()) {drawEllipse(cx,cy,r,r,0.,color,params);}
}

///
/// Convenience macros
///
#ifdef VIBES_GENERATE_vibesXXX_MACROS
/// Macro to simplify contruction of a Params object
#define vibesParams(...) (vibes::Params(), __VA_ARGS__)
/// Convenience macros for "Matlab style" variadic calls
#define vibesDrawBox(x_lb,x_ub,y_lb,y_ub,...) vibes::drawBox(x_lb,x_ub,y_lb,y_ub,vibesParams(__VA_ARGS__))
#define vibesDrawEllipse(cx,cy,a,b,rot,...) vibes::drawEllipse(cx,cy,a,b,rot,vibesParams(__VA_ARGS__))
#define vibesDrawConfidenceEllipse(cx,cy,sxx,sxy,syy,sigma,...) vibes::drawConfidenceEllipse(cx,cy,sxx,sxy,syy,sigma,vibesParams(__VA_ARGS__))
#define vibesDrawCircle(cx,cy,r,...) vibes::drawCircle(cx,cy,r,vibesParams(__VA_ARGS__))
#endif

#endif //#ifndef VIBES_CPP_API_H
