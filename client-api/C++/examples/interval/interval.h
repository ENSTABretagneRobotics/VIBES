#ifndef __INTERVAL__
#define __INTERVAL__

#include "iboolean.h"

#include <iosfwd> // Forward declarations for iostream
#include <vector> //!< \todo Forward declare template classes vector<int>, vector<interval> and vector<double>
#include <limits>

class QDebug;

//using namespace std; //!< \todo Remove "using namespace std" from interval include.

//! \todo Put all interval stuff in interval namespace

class QDataStream;
class QDebug;

// #define oo 1/0.0 //definition de l'infini noté oo
const double oo = std::numeric_limits<double>::infinity();

double Arccossin(const double&,const double&);
double Arg      (const double&,const double&);
double Sign     (const double& x);
double Chi      (const double& a,const double& b,const double& c);


class borne   // used by q_in only
{
public:
    double val;
    int ouverture;
    borne ();
    borne (const double&, const int&);
    friend bool     operator<(const borne &x, const borne &y);
};

//! \todo Data members of interval should be private, but it implies work on box.cpp too.
class interval
{
public:
    double inf;
    double sup;
    bool isEmpty;

public:
    //--------------------------------------------------------------------------
    //                           CONSTRUCTEURS
    //--------------------------------------------------------------------------
    interval ();
    interval (const double&);    // Pas const pour conversion double -> interval
    interval (const double&, const double&);
    interval (const interval&);
    interval& operator= (const interval&);
    interval& Intersect(const interval&);
    bool IsEmpty(void) const {return isEmpty;}
};

//--------------------------------------------------------------------------
//                          OPERATEURS
//--------------------------------------------------------------------------
interval operator& (const interval&,const interval&);
interval operator+ (const interval&,const interval&);
interval operator- (const interval&);
interval operator- (const interval&, const interval&);
interval operator* (const double&, const interval&);
interval operator* (const interval&, const double&);
interval operator* (const interval&, const interval&);
interval operator/ (const interval& ,const interval&);

// Display
std::ostream& operator<< (std::ostream&, const interval&);
QDebug operator<< (QDebug, const interval&);
// Binary stream i/o
QDataStream & operator<< (QDataStream &, const interval&);
QDataStream & operator>> (QDataStream &, interval&);

//--------------------------------------------------------------------------
//                 FONCTION  ET PROCEDURES AMIES
//--------------------------------------------------------------------------
interval  Abs        (const interval&);
interval  Cos        (const interval&);
double    Det        (double&,double&,double&,double&);
interval  Det        (interval&, interval&, interval&, interval&);
double    DistanceDirSegment(double& mx, double& my, double& theta, double& ax, double& ay, double& bx, double& by);
double    DistanceDirSegments(double& mx, double& my, double& theta, std::vector<double> ax, std::vector<double> ay, std::vector<double> bx, std::vector<double> by);
double    DistanceDirCercles(double& mx, double& my, double& theta, std::vector<double> cx, std::vector<double> cy, std::vector<double> r);

interval  Exp        (const interval&);
interval  Inflate    (const interval&,double);
interval  Inter      (const interval&,const interval&);
interval  Inter      (std::vector<interval> x);
interval  InterMin   (const interval&,const interval&,char);
interval  Log        (const interval&);
interval  Max        (const interval&,const interval&);
double    Max        (std::vector<double>& x);
interval  Min        (const interval&,const interval&);
interval  Min        (const interval&,const interval&,const interval&);
double    Min        (std::vector<double>& x);
interval  Parabole   (const interval&, double, double, double);
interval  Power      (const interval&,int);
interval  Pow        (const interval&, int);
interval  Pow        (const interval&, int, int);
interval  PowRoot    (const interval&, int, int);
interval  Sin        (const interval&);
interval  Sqr        (const interval&);
interval  Sqrt       (const interval&);
interval  Step       (const interval&);

interval  Modulo     (const interval&, double);
interval  InvSqr     (const interval&);

interval  InvSqrt    (const interval&);
interval  Union      (const interval&, const interval&);
interval  Union      (std::vector<interval> x);
interval  Tan        (const interval&);

iboolean  In         (const interval&,const interval&);

bool      Disjoint   (const interval&,const interval&);
bool      In         (double,const interval&);
bool      Subset     (const interval&,const interval&);
bool      SubsetStrict(const interval& a,const interval& b);
bool      Subset     (const interval&,const interval&,double);
bool      OverLap    (const interval&,const interval&);

void      Sucre      (interval&, const interval&);
void      Inter1     (interval&, interval&,const interval&,const interval&, const interval&);

double      Inf        (const interval&);
double      Sup        (const interval&);
double      Rand       (const interval&);
double      Center     (const interval&);
double      Eloignement(const interval&, const interval&);
double      Marge      (const interval&, const interval&);
double      AbsMax     (const interval&);
double      Todouble     (const interval&);
double      Width      (const interval&);
double      Rad      (const interval&);
//--------------------------------------------------------------------------
//    PROCEDURES TRADUISANT LE C.S.P POUR (SIN, COS, PLUS, EXP, ...)
//--------------------------------------------------------------------------
//             Contrainte Spécifique
void      Csame_sign(interval&, interval&);
void      Cgeq(interval&, interval&);
//            Contraintes Unaires
void      Cinteger    (interval&);
void      Cboolean    (interval&);

//             Contraintes Binaires
void      Csin    (interval&, interval&, int);
void      Ccos    (interval&, interval&, int);
void      Csinc   (interval&, interval&, int);
void      Cexp    (interval&, interval&, int);
void      Clog    (interval&, interval&, int);
void      Ctan    (interval&, interval&, int);
void      Catan   (interval&, interval&, int);
void      Csqr    (interval&, interval&, int);
void      Csqrt   (interval&, interval&, int);

void      Cmoins  (interval&, interval&, int);
void      Cegal   (interval&, interval&, int);
void      Cegal   (interval&, interval&);
void      Cabs    (interval&, interval&, int);
void      Crect   (interval&, interval&, int);
void      Cpower  (interval&, interval&, int, int);
void      Ctriangle (interval&, interval&, int);
void      Cramp   (interval&, interval&, int, double a=0);
void      Cstep   (interval&, interval&, int, double a=0);
void      Cheaviside (interval&, interval&, int, double a=0);
void      Csign   (interval&, interval&, int, double a=0);
//              Contraintes Ternaires
void      Crect   (interval&, interval&, interval&, int);
void      Cplus   (interval&, interval&, interval&, int);
void      Cplus   (double&,   interval&, interval&, int);
void      Cplus   (interval&, double&,   interval& ,int);
void      Cmoins  (interval&, interval&, interval&, int);
void      Cmoins  (double&,   interval&, interval&, int);
void      Cmoins  (interval&, interval&, double& x, int);
void      CNorm   (interval&,interval&,interval& );
void      Cprod   (interval&, interval&, interval&, int);
void      Cprod   (interval& , double& , interval& , int);
void      Cdiv    (interval&, interval&, interval&, int);
void      Cmin    (interval&, interval&, interval&, int);
void      Cmin    (interval&, interval&, interval&, interval&, int);
int       Cmin    (interval&, std::vector<interval>&, int);
void      Cmax    (interval&, interval&, interval&, int);
void      C_q_in  (interval& x, int, std::vector<interval>&);

int      CAngle  (interval&,interval&,interval&,interval&,interval&,bool);
void     Cchi    (interval&, interval&, interval&, interval&);
int     CDistanceDirSegment(interval&,interval&,interval&, interval&, double&, double&, double&,double&, int);
void     CDistanceDirSegments(interval&, interval&, interval&, interval&, std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>);

int CinSegment(interval& mx, interval& my, double& ax, double& ay, double& bx,double& by);
int CinCircle(interval& mx, interval& my, double& cx, double& cy, double& r);
void     CinSegments(interval&, interval&, std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>);
void     CinSegmentsOrCircles(interval& mx, interval& my,  std::vector<double> ax, std::vector<double> ay, std::vector<double> bx, std::vector<double> by, std::vector<double> cx, std::vector<double> cy, std::vector<double> r);
void CLegOnWallsOrCircles(interval& dist, interval& px, interval& py, interval& theta, std::vector<double> ax, std::vector<double> ay, std::vector<double> bx, std::vector<double> by, std::vector<double> cx, std::vector<double> cy, std::vector<double> r);


void     CLegOnWalls(interval&, interval&, interval&, interval&, std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>);
void CPatteCroiseAucunSegment(interval& dist, interval& px, interval& py, interval& theta, std::vector<double> ax, std::vector<double> ay, std::vector<double> bx, std::vector<double> by);





//------- Procédure de réduction élémentaires sur les intervalles ----------
void      Contract0       (char, interval&, interval&, int, int n=0);
void      Contract0       (char, interval&, interval&, interval&, int);
void      Contract0       (char, interval&, double&    , interval&, int); //Luc

void      Contract0       (char, interval&);
//--------------------------------------------------------------------------
//
void      IntButterfly    (interval&, interval, interval, interval&, interval, int);

#endif
