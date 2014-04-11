///
/// \file vibes.hpp
/// \brief Vibes C++ API Header
/// \author Vincent Drevelle, Jeremy Nicolas
/// \date 2013-2014
/// \version 0.0.3alpha
///

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
        template <typename T> Value(const std::vector<T> &v) : _array(v.begin(),v.end()), _type(vt_array) {}
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
    /// Define useful types for colors, vectors and points
    typedef Vec<int,3> RGB;
    typedef Vec<int,4> RGBA;
    typedef Vec<double,2> Vec2d;
    typedef Vec<double,3> Vec3d;
    typedef Vec<double,4> Vec4d;

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

  // Convenience drawing functions with color selection
#define VIBES_COLOR_PARAM_NAME "color"
#define VIBES_FUNC_COLOR_PARAM_1(func_name, T1, a) \
  void func_name(T1 a, Params params); \
  inline void func_name(T1 a, \
              const std::string &color="b", Params params=Params()) {func_name(a,(params,VIBES_COLOR_PARAM_NAME,color));}
#define VIBES_FUNC_COLOR_PARAM_2(func_name, T1, a, T2, b) \
  void func_name(T1 a, T2 b, Params params); \
  inline void func_name(T1 a, T2 b, \
              const std::string &color="b", Params params=Params()) {func_name(a,b,(params,VIBES_COLOR_PARAM_NAME,color));}
#define VIBES_FUNC_COLOR_PARAM_3(func_name, T1, a, T2, b, T3, c) \
  void func_name(T1 a, T2 b, T3 c, Params params); \
  inline void func_name(T1 a, T2 b, T3 c, \
              const std::string &color="b", Params params=Params()) {func_name(a,b,c,(params,VIBES_COLOR_PARAM_NAME,color));}
#define VIBES_FUNC_COLOR_PARAM_4(func_name, T1, a, T2, b, T3, c, T4, d) \
  void func_name(T1 a, T2 b, T3 c, T4 d, Params params); \
  inline void func_name(T1 a, T2 b, T3 c, T4 d, \
              const std::string &color="b", Params params=Params()) {func_name(a,b,c,d,(params,VIBES_COLOR_PARAM_NAME,color));}
#define VIBES_FUNC_COLOR_PARAM_5(func_name, T1, a, T2, b, T3, c, T4, d, T5, e) \
  void func_name(T1 a, T2 b, T3 c, T4 d, T5 e, Params params); \
  inline void func_name(T1 a, T2 b, T3 c, T4 d, T5 e, \
              const std::string &color="b", Params params=Params()) {func_name(a,b,c,d,e,(params,VIBES_COLOR_PARAM_NAME,color));}
#define VIBES_FUNC_COLOR_PARAM_6(func_name, T1, a, T2, b, T3, c, T4, d, T5, e, T6, f) \
  void func_name(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, Params params); \
  inline void func_name(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, \
              const std::string &color="b", Params params=Params()) {func_name(a,b,c,d,e,f,(params,VIBES_COLOR_PARAM_NAME,color));}

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

  // View setting
  void axisAuto(Params params=Params());
  void axisLimits(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, Params params=Params());

  // Drawing functions
  VIBES_FUNC_COLOR_PARAM_4(drawBox,const double &,x_lb, const double &,x_ub, const double &,y_lb, const double &,y_ub)
  VIBES_FUNC_COLOR_PARAM_1(drawBox,const std::vector<double> &,bounds)

  VIBES_FUNC_COLOR_PARAM_5(drawEllipse,const double &,cx, const double &,cy, const double &,a, const double &,b, const double &,rot)
  VIBES_FUNC_COLOR_PARAM_6(drawConfidenceEllipse,const double &,cx, const double &,cy,
                                                 const double &,sxx, const double &,sxy, const double &,syy,
                                                 const double &,K/*=3.0*/)
  VIBES_FUNC_COLOR_PARAM_3(drawConfidenceEllipse,const std::vector<double> &,center,
                                                 const std::vector<double> &,cov,
                                                 const double &,K/*=3.0*/)
  VIBES_FUNC_COLOR_PARAM_3(drawCircle,const double &,cx, const double &,cy, const double &,r)

  VIBES_FUNC_COLOR_PARAM_1(drawBoxes,const std::vector< std::vector<double> > &,bounds)
  VIBES_FUNC_COLOR_PARAM_1(drawBoxesUnion,const std::vector< std::vector<double> > &,bounds)

  VIBES_FUNC_COLOR_PARAM_1(drawLine,const std::vector< std::vector<double> > &,points)
  VIBES_FUNC_COLOR_PARAM_2(drawLine,const std::vector<double> &,x, const std::vector<double> &,y)

  // Ibex enabled functions
  #ifdef _IBEX_INTERVAL_H_
    VIBES_FUNC_COLOR_PARAM_2(drawBox,const ibex::Interval &,x, const ibex::Interval &,y)
  #endif //#ifdef _IBEX_INTERVAL_H_
  #ifdef __IBEX_INTERVAL_VECTOR_H__
    VIBES_FUNC_COLOR_PARAM_1(drawBox,const ibex::IntervalVector &,box);
  #endif //#ifdef __IBEX_INTERVAL_VECTOR_H__


  //
  // Inline Implementations
  //

  inline void drawCircle(const double &cx, const double &cy, const double &r, const Params &params) {
    drawEllipse(cx,cy,r,r,0.,params);
  }
  // Ibex enabled functions
  #ifdef _IBEX_INTERVAL_H_
    inline void drawBox(const ibex::Interval &x, const ibex::Interval &y, Params params) {
        drawBox(x.lb(),x.ub(),y.lb(),y.ub(),params);
    }
  #endif //#ifdef _IBEX_INTERVAL_H_
  #ifdef __IBEX_INTERVAL_VECTOR_H__
  /// \todo N-dimensionanl Ibex Inteval vector support
    inline void drawBox(const ibex::IntervalVector &box, Params params) {
        drawBox(box[0], box[1], params);
    }
  #endif //#ifdef __IBEX_INTERVAL_VECTOR_H__
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
#define vibesDrawConfidenceEllipse(cx,cy,sxx,sxy,syy,K,...) vibes::drawConfidenceEllipse(cx,cy,sxx,sxy,syy,K,vibesParams(__VA_ARGS__))
#define vibesDrawCircle(cx,cy,r,...) vibes::drawCircle(cx,cy,r,vibesParams(__VA_ARGS__))
#endif //#ifdef VIBES_GENERATE_vibesXXX_MACROS

#endif //#ifndef VIBES_CPP_API_H
