// This file is part of VIBes' C++ API examples non-guaranteed interval library
//
// Copyright (c) 2013 Luc Jaulin
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

#include "interval.h"

#include <cstdio>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "iboolean.h"
#include <cmath>

//! \todo Make QT specific inclusions conditional or implement Qt specific functions in a separate unit.
//#include <QDataStream>
//#include <QDebug>

#include "box.h"

using namespace std;

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923
#endif

const double PI = M_PI;
const double PI_2 = M_PI_2;
const double TROIS_PI_2 = 3*PI_2;
const double CINQ_PI_2 = 5*PI_2;
const double SEPT_PI_2 = 7*PI_2;
const double DEUX_PI = 4*PI_2;



double Arccossin(const double& x, const double& y)
{   if (y>0) return(acosl(x));
    else return(-acosl(x));
}
//----------------------------------------------------------------------
double Arg(const double& x, const double& y)
{   double r=sqrt(x*x+y*y);
    if (r==0) return 0;
    return Arccossin(x/r,y/r);
}
//----------------------------------------------------------------------
double Sign(const double& x)
{   if (x>0) return(1);
    else return(0);
}
//----------------------------------------------------------------------
double Chi(const double& a,const double& b,const double& c)
{   if (a<0) return(b);
    else return(c);
}
//**********************************************************************
//*****************     CONSTRUCTEURS    *******************************
//**********************************************************************
//----------------------------------------------------------------------


borne::borne()
{   val=0;  ouverture=0;
}
//----------------------------------------------------------------------
borne::borne (const double& a, const int& b)
{   val=a;  ouverture=b;
}
//----------------------------------------------------------------------
bool operator<(const borne &x, const borne &y)
 {
     return x.val < y.val;
 }






interval::interval ()
{   inf=-999;  sup=-999;
    isEmpty=true;
}
//----------------------------------------------------------------------
interval::interval (const double& m)
{   inf=m;  sup=m;
    isEmpty=false;
}
//----------------------------------------------------------------------
interval::interval (const double& a, const double& b)
{   if  (a>=b)   {inf = b ; sup = a;} //modif sur >=
    else        {inf = a ; sup = b;}
    isEmpty=false;
}
//----------------------------------------------------------------------

interval& interval::Intersect(const interval& Y)
{  interval X=*this;
   interval Z=Inter(X,Y);
   *this=Z;
   return *this;
}

//----------------------------------------------------------------------
interval::interval (const interval& a) {*this = a;}
//----------------------------------------------------------------------

//**********************************************************************
//************************      OPERATEURS     *************************
//**********************************************************************
interval& interval::operator= (const interval& v)
{(*this).inf=v.inf; (*this).sup=v.sup; (*this).isEmpty=v.isEmpty;
  return *this;}
//----------------------------------------------------------------------
interval operator& (const interval& x,const interval& y)
{ return (Inter(x,y));}
//----------------------------------------------------------------------
interval operator+ (const interval& x,const interval& y)
{ if (x.isEmpty||y.isEmpty) return interval();
  else  return (interval(x.inf+y.inf,x.sup+y.sup));}
//----------------------------------------------------------------------
interval operator- (const interval& a)
{ if (a.isEmpty) return interval();
  else return(interval(-(a.sup),-(a.inf))); }
//----------------------------------------------------------------------
interval operator- (const interval& x, const interval& y)
{ if (x.isEmpty||y.isEmpty) return interval();
  else return(x+(-y));}
//----------------------------------------------------------------------
interval operator* (const double& x, const interval& y)
{return interval(x*y.inf,x*y.sup); }
//----------------------------------------------------------------------
interval operator* (const interval& x, const double& y)
{return interval(y*x); }
//----------------------------------------------------------------------
interval operator* (const interval& x, const interval& y)
{
if (x.isEmpty||y.isEmpty) return interval();
double x1=x.inf,y1=y.inf,x2=x.sup,y2=y.sup;
   if (x1>=0)
   {
   if  (y1>=0) return interval(x1*y1,x2*y2);
   else
      if  (y2<=0) return interval(x2*y1,x1*y2);
      else return interval(x2*y1,x2*y2);
   }
   else
   if (x2<=0)
   {
   if  (y1>=0) return interval(x1*y2,x2*y1);
   else if (y2<=0) return interval(x2*y2,x1*y1);
         else return interval(x1*y2,x1*y1);
   }
   else
      {
      if (y1>=0) return interval(x1*y2,x2*y2);
      else if (y2<=0) return interval(x2*y1,x1*y1);
           else return interval(min(x1*y2,x2*y1),max(x1*y1,x2*y2));
      }
}
//----------------------------------------------------------------------
interval operator/  (const interval& a, const interval& b)
{
if (a.isEmpty||b.isEmpty) return interval();
if (b.inf> 0)
    {
    if    (a.inf>=0) return interval(a.inf/b.sup, a.sup/b.inf);
    else if (a.sup<0)  return interval(a.inf/b.inf, a.sup/b.sup);
           else  return interval(a.inf/b.inf, a.sup/b.inf);
    }
    else if (b.sup<0)
          {
          if (a.inf>=0) return interval(a.sup/b.sup, a.inf/b.inf);
            else if (a.sup<0)  return interval(a.sup/b.inf, a.inf/b.sup);
                  else return interval(a.sup/b.sup, a.inf/b.sup);
          }
          else return interval(-oo,oo);
}
//------------------------------------------------------------------------------
ostream& operator<< (ostream& os, const interval& a)
{
if      (a.isEmpty)     os<<"EmptyInterval";
    else if (a.inf!=a.sup)
          {os << "[" <<setprecision(4)<<a.inf << ", " << setprecision(4)<<a.sup << "] ";   }
         else os << a.inf ;
return os;
}

//QDataStream & operator<< (QDataStream &s, const interval&i)
//{
//    s << i.inf << i.sup << i.isEmpty;
//    return s;
//}

//QDataStream & operator>> (QDataStream &s, interval&i)
//{
//    s >> i.inf >> i.sup >> i.isEmpty;
//    return s;
//}

//! \todo Factorize ostream and QDebug formatted output code.
//! \todo Relocate Qt specific code to another CPP file and use conditional compilation.
//QDebug operator<< (QDebug os, const interval&a)
//{
//if      (a.isEmpty)     os.nospace()<<"EmptyInterval";
//    else if (a.inf!=a.sup)
//          {os.nospace() << "["<<a.inf << ", " << a.sup << "] ";   }
//         else os.nospace() << a.inf ;
//return os.space();
//}


//-----------------------------------------------------------------------
//*********************************************************************
//******************    FONCTION NON MEMBRES     **********************
//*********************************************************************
//----------------------------------------------------------------------
double Inf(const interval& a)
{ if (a.inf<a.sup) return a.inf;
  else return a.sup;
}
//----------------------------------------------------------------------
double Sup(const interval& a)
{ if (a.sup>a.inf) return a.sup;
  else return a.inf;
}
//----------------------------------------------------------------------
double Width(const interval& a)
{
if (a.isEmpty) return (-1);
else return (a.sup-a.inf);
}
//----------------------------------------------------------------------
double Rad(const interval& a)
{
if (a.isEmpty) return (-1);
else return (a.sup-a.inf)/2.0;
}
//-----------------------------------------------------------------------
double Center(const interval& a)
{
 if (a.isEmpty) return oo;
 else  return (a.sup+a.inf)/2;
}
//----------------------------------------------------------------------
double Rand(const interval& a)
{
 if (a.isEmpty) return oo;
 else  return a.inf+(a.sup-a.inf)*rand()/RAND_MAX;
}
//----------------------------------------------------------------------
double Marge(const interval& a, const interval& b)
{  return min(a.inf-b.inf,b.sup-a.sup); }
//----------------------------------------------------------------------
interval Union(const interval& a, const interval& b)
{
if (a.isEmpty) return b;
if (b.isEmpty) return a;
interval r(0,0);
if (a.inf<=b.inf) r.inf=a.inf; else r.inf=b.inf;
if (a.sup>=b.sup) r.sup=a.sup; else r.sup=b.sup;
return r;
}
//----------------------------------------------------------------------
interval Inter (vector<interval> x)
{   interval r=x[0];
    for (unsigned int i=1;i<x.size();i++)
       r=Inter(r,x[i]);
    return r;
}
//----------------------------------------------------------------------
interval Union (vector<interval> x)
{   interval r=x[0];
    for (unsigned int i=1;i<x.size();i++)
       r=Union(r,x[i]);
    return r;
}
 //----------------------------------------------------------------------
interval Inter(const interval& a, const interval& b)
{
if (a.isEmpty || b.isEmpty) return interval();
interval r(0,0);
if ((a.inf>b.sup)||(b.inf>a.sup)) return interval();
if (a.inf<=b.inf) r.inf=b.inf; else r.inf=a.inf;
if (a.sup>=b.sup) r.sup=b.sup; else r.sup=a.sup;
return r;
}
//------------------------------------------------------------------------------
bool Disjoint(const interval& a, const interval& b)
{   if (a.isEmpty ||b.isEmpty) return true;
    return ((a.sup<b.inf)||(b.sup<a.inf));
}
//----------------------------------------------------------------------
interval Abs  (const interval& a)
{
    if (a.isEmpty) return interval();
    double a1=a.inf, a2=a.sup;
    if ((a1>=0)||(a2<=0))   return interval(fabs(a1),fabs(a2));
    if (fabs(a1)>fabs(a2))  return interval(0,fabs(a1));
    else                    return interval(0,fabs(a2));
}
//----------------------------------------------------------------------
double AbsMax (const interval& x)              // Absolute maximum of
{   if (x.isEmpty) return 0;
    double a = fabs(x.inf), b = fabs(x.sup);
    return max(a,b);
}
//----------------------------------------------------------------------
interval Cos(const interval& a)
{   if (a.isEmpty) return interval();
    else return Sin(a+PI_2);
}
//----------------------------------------------------------------------
interval Exp(const interval& a)
{   if (a.isEmpty) return(interval());
    else return(interval(exp(a.inf),exp(a.sup)));
}


interval Step(const interval& X)
{ if (X.isEmpty) return(interval());
  if (X.inf>0) return (interval(1));
  if (X.sup<0) return(interval(0));
  return (interval(0,1));
}



//----------------------------------------------------------------------
bool In(double a,const interval& b)
{   interval z=Inflate(b,1e-6);;
    if (b.isEmpty) return false;
    return (z.inf<=a && a<=z.sup);
}
//----------------------------------------------------------------------
iboolean In(const interval& F, const interval& Y)
{   interval Z=Inflate(Y,1e-17);
    if (Disjoint (F,Z)) return false;
    if (Subset   (F,Z)) return true;
    return iboolean(iperhaps);
}
//----------------------------------------------------------------------
double Eloignement(const interval& x, const interval& y)
{   if ((x.isEmpty)||(y.isEmpty)) return oo;
    double r=max(0.0, x.inf-y.sup);
    r=max(r, y.inf-x.sup);
    return r;
}
//----------------------------------------------------------------------
interval Inflate(const interval& a,double eps)
{   interval r(a.inf-eps,a.sup+eps);
    r.isEmpty=a.isEmpty;
    return interval(r);
}
//----------------------------------------------------------------------
void Min1(double& zmin0,double& zmin1,double a,double b,double c)
{   if ((a<=b)&&(a<=c)) {zmin0=a; zmin1=min(b,c); return;};
    if ((b<=a)&&(b<=c)) {zmin0=b; zmin1=min(a,c); return;};
    if ((c<=b)&&(c<=a)) {zmin0=c; zmin1=min(a,b); return;};
    return;
}
//----------------------------------------------------------------------
void Max1(double& zmax0,double& zmax1,double a,double b,double c)
{   if ((a>=b)&&(a>=c)) {zmax0=a; zmax1=max(b,c); return;};
    if ((b>=a)&&(b>=c)) {zmax0=b; zmax1=max(a,c); return;};
    if ((c>=b)&&(c>=a)) {zmax0=c; zmax1=max(a,b); return;};
}
//----------------------------------------------------------------------
void Inter1(interval& r0, interval& r1, const interval &a, const interval &b, const interval &c)
{   interval r(-999);
    if (a.isEmpty) {r0=a;r1=Inter(b,c);return;};
    if (b.isEmpty) {r0=b;r1=Inter(a,c);return;};
    if (c.isEmpty) {r0=c;r1=Inter(a,b);return;};
    double zmin0,zmax0,zmin1,zmax1;
    //Min1(zmin0,zmin1,Inf(a),Inf(b),Inf(c));
    //Max1(zmax0,zmax1,Sup(a),Sup(b),Sup(c));
    Max1(zmin0,zmin1,a.inf,b.inf,c.inf);
    Min1(zmax0,zmax1,a.sup,b.sup,c.sup);
    if (zmin0>zmax0)  r0.isEmpty=true; else r0=interval(zmin1,zmax1);
    if (zmin1>zmax1)  r1.isEmpty=true; else r1=interval(zmin1,zmax1);
    return;
}
//----------------------------------------------------------------------
void Sucre(interval& P, const interval& S)
{   if (Disjoint(P,S)||Subset(S,P)||Subset(P,S)) return;
    if (In(S.inf,P)) {P.sup=S.inf; return;}
    if (In(S.sup,P)) {P.inf=S.sup; return;}
}
//----------------------------------------------------------------------
interval Log(const interval& a)
{
   if ((a.isEmpty)||(a.sup<=0)) return interval();
   interval b=Abs(a);
   if (In(0,a)) return interval(-oo,log(a.sup));
   return interval(log(b.inf),log(b.sup));}
//----------------------------------------------------------------------
double Min (vector<double>& x)
{
    double d=oo;
    for (unsigned int i=0;i<x.size();i++)
       d=min(x[i],d);
    return d;
}
//----------------------------------------------------------------------
double Max (vector<double>& x)
{
    double d=-oo;
    for (unsigned int i=0;i<x.size();i++)
       d=max(x[i],d);
    return d;
}
//----------------------------------------------------------------------
interval Enveloppe (vector<double>& x)
{   if (x.size()==0) return (interval());
    return(interval(Min(x),Max(x)));
}
//----------------------------------------------------------------------
interval Min (const interval& x, const interval& y)
{
    if (x.isEmpty||y.isEmpty) return interval();
    double a = min(x.sup,y.sup);
    double b = min(x.inf,y.inf);
    if ((x.sup==x.inf)&&(y.sup==y.inf)) return interval(b);
    return interval(a,b);
}
//----------------------------------------------------------------------
interval Min (const interval& x, const interval& y,const interval& z)
{   return(Min(Min(x,y),z));
}
//----------------------------------------------------------------------
interval Max (const interval& x,const interval& y)
    { return (-Min(-x,-y)); }
//----------------------------------------------------------------------
interval InterMin (const interval& a, const interval& b, char c)
{   interval y(0,0);
    if (c=='-')  {interval temp(a.inf,oo); y=Inter(temp,b);}
    else {interval temp(0, a.sup); y=Inter(temp,b);}
    return y;
}
//----------------------------------------------------------------------
interval Modulo(const interval& a, double x)
{   if ((a.inf>=0)&&(a.inf<x)) return (a);
    int k = (long)floorl((a.inf/x));
    double offset = x * k;
    return interval(a.inf-offset,a.sup-offset);
}
//----------------------------------------------------------------------
bool OverLap(const interval& a, const interval& b)
{   return (a.sup>b.inf)&&(b.sup>a.inf);}
//----------------------------------------------------------------------
interval Parabole(const interval& x,double a,double b,double c)
{   return (  a*Sqr(x+(b/(2*a)))-(b*b)/(4*a)+c);}
//----------------------------------------------------------------------
interval Sin (const interval& a)
{
    if (a.isEmpty) return interval();
    if (a.sup==a.inf) return sin(a.inf);
    interval b;
    double sin1,sin2,r1,r2;
    b = Modulo(a,DEUX_PI);
    if (Width(a)>DEUX_PI) return (interval(-1,1));
    sin1=sin(b.inf);   sin2=sin(b.sup);
    if ((b.inf < TROIS_PI_2)&&(b.sup > TROIS_PI_2)) r1=-1.0;
    else if ((b.inf < SEPT_PI_2)&&(b.sup > SEPT_PI_2)) r1=-1.0;
    else r1=((sin1 < sin2)? sin1 : sin2);
    if ((b.inf < PI_2)&&(b.sup > PI_2)) r2=1.0;
    else if ((b.inf < CINQ_PI_2)&&(b.sup > CINQ_PI_2)) r2=1.0;
    else r2=((sin1 > sin2)? sin1 : sin2);
    return Inter(interval(-1,1),interval(r1,r2));
}
//----------------------------------------------------------------------
interval Sqr(const interval& a)
{   double a1=a.inf, a2=a.sup;
    if (a.isEmpty) return interval();
    if ((a1>=0)||(a2<=0))   return interval(a1*a1,a2*a2);
    if (fabs(a1)>fabs(a2))  return interval(0,a1*a1);
    else                    return interval(0,a2*a2);
}
//----------------------------------------------------------------------
interval Sqrt(const interval& a)
{   if ((a.isEmpty)||(a.sup<0)) return interval();
    double a1=a.inf, a2=a.sup;
    if (a1>=0)  return (interval(sqrt(a1),sqrt(a2)));
    else return (interval(0,sqrt(a2)));
}
//----------------------------------------------------------------------
interval InvSqr(const interval& a)
{  if ((a.isEmpty)||(a.sup<0)) return interval();
   double a2=max(a.inf,a.sup);
   //if (In(a1,interval(a2))) return sqrt(a1); else
   return interval(-sqrt(a2),sqrt(a2));
}
//----------------------------------------------------------------------
bool Subset(const interval& a,const interval& b)
{ if (a.isEmpty) return true;
  if (b.isEmpty) return false;
  return ((a.inf>=b.inf)&&(a.sup<=b.sup));
}
//----------------------------------------------------------------------
bool SubsetStrict(const interval& a,const interval& b)
{ if (a.isEmpty) return true;
  if (b.isEmpty) return false;
  return ((a.inf>b.inf)&&(a.sup<b.sup));
}
//----------------------------------------------------------------------
bool Subset(const interval& a, const interval& b,double epsilon)
{  if (a.isEmpty||b.isEmpty) return false;
   else return (Subset(a,b)&&((b.inf<a.inf-epsilon)||(a.sup<b.sup-epsilon)));
}
//----------------------------------------------------------------------------
static double Power (double x, int n, int RndMode )
{ int  ChangeRndMode;     // for x < 0 and odd n
  double p, z;
  ChangeRndMode = ( (x < 0.0) && (n % 2 == 1) );
  if (ChangeRndMode) {z=-x; RndMode=-RndMode;}  else     z= x;
  p = 1.0;
  switch (RndMode) {                             // Separate while-loops used
        case -1 :   while (n > 0)
                    {if (n % 2 == 1) p = p*z;   //--------------------------
                     n = n / 2; if (n > 0) z = z*z;}break;
        case +1 :   while (n > 0)
                    {if (n % 2 == 1) p = p*z;
                     n = n / 2;
                     if (n > 0) z = z*z;}break;  }
  if (ChangeRndMode)   return -p; else  return p;
}
//----------------------------------------------------------------------------
interval Power (const interval& x, int n)
{ int  m;   double Lower, Upper;
  if (n == 0) return(interval(1.0,1.0));
  if (n > 0)  m = n;  else  m = -n;
  if ( (0.0 < x.inf) || (m % 2 == 1) )
  {  Lower = Power(x.inf,m,-1);    Upper = Power(x.sup,m,+1);}
  else if (0.0 > x.sup)
  {  Lower = Power(x.sup,m,-1);    Upper = Power(x.inf,m,+1);}
  else  {Lower = 0.0;Upper = Power(AbsMax(x),m,+1); }
  if (n > 0) return(interval(Lower,Upper));
  else   return(1.0/interval(Lower,Upper));    // if 0 in 'x'.
}
//----------------------------------------------------------------------------
interval Pow (const interval& x, int n)
/* [x]^n */
{   if (x.isEmpty) return interval();
    double a1=min(x.inf,x.sup), a2=max(x.inf,x.sup);
    if (n>0)
       { if (n%2==0)
            { if (a1*a2<0) return interval(0,pow(a2,n));
              else return interval(pow(a1,n),pow(a2,n)); }
         else return interval(pow(a1,n),pow(a2,n));
       }
    else return interval(1.0)/Pow(x,abs(n));
}
//----------------------------------------------------------------------------
interval Pow (const interval& x, int num, int den)
/* [x]^(num/den) */
{    if (x.isEmpty) return interval();
     if (num*den>0)
        { double a1=min(x.inf,x.sup), a2=max(x.inf,x.sup);
          double n=num, m=den;
          if (den%2==0)
             { if ((a1>=0)||(a1*a2<=0))
                   return interval(-pow(a2,n/m),pow(a2,n/m));
               else return interval();
             }
          else
             { if (a1*a2<=0)
                  return interval(-pow(fabs(a1),n/m),pow(a2,n/m));
               if (a1>0) return interval(pow(a1,n/m),pow(a2,n/m));
               else return interval(-pow(fabs(a1),n/m),-pow(fabs(a2),n/m));
             }
        }
     else return interval(1.0)/Pow(x,abs(num),abs(den));
}
//----------------------------------------------------------------------
interval PowRoot (const interval& x, int num, int den)
/* [x]^(num/den) */
{    if (x.isEmpty) return interval();
     if (num*den>0)
        { double a1=min(x.inf,x.sup), a2=max(x.inf,x.sup), n=num, m=den;
          if (den%2==0)
             { if (a1>=0) return interval(pow(a1,n/m),pow(a2,n/m));
               if (a1*a2<=0) return interval(0,pow(a2,n/m));
               else return interval(); }
          else
             { if (a1*a2<=0) return interval(-pow(fabs(a1),n/m),pow(a2,n/m));
               if (a1>0) return interval(pow(a1,n/m),pow(a2,n/m));
               else return interval(-pow(fabs(a1),n/m),-pow(fabs(a2),n/m)); }
        }
     else return interval(1.0)/Pow(x,abs(num),abs(den));
}
//----------------------------------------------------------------------
interval Tan(const interval& a) {return Sin(a)/Cos(a);}
//----------------------------------------------------------------------
double Todouble (const interval& a)
    {   if (a.sup!=a.inf) return -9999;
        return a.inf;
    }
//----------------------------------------------------------------------
//**********************************************************************
//**                      CONTRACTEUR ELEMENTAIRES                    **
//**********************************************************************
//----------------------------------------------------------------------
//                   CONTRAINTES PARTICULIERES
//----------------------------------------------------------------------
void Csame_sign(interval& Y, interval& X)
/* max(A1,A2)>0
   A1=min(X,Y)
   A2=min(X,Y)  */
{ interval A1(0,oo), A2(-oo,oo), A3(-oo,oo);
  interval Xmoins(0), Ymoins(0);
  Xmoins=-X; Ymoins=-Y;
  Cmin(A2,X,Y,1);
  Cmin(A3,Xmoins,Ymoins,1);
  Cmax(A1,A2,A3,1);
  Cmax(A1,A2,A3,-1);
  Cmin(A3,Xmoins,Ymoins,-1);
  Y=Inter(Y,-Ymoins);
  X=Inter(X,-Xmoins);
  Cmin(A2,X,Y,-1); Cmin(A2,X,Y,1); Cmin(A2,X,Y,-1);
}
//----------------------------------------------------------------------
void Cgeq(interval& Y, interval& X)
{ if (Y.inf>=X.sup) return;
  interval Z=Y-X;
  Z=Inter(Z,interval(0,oo));
  Cmoins(Z,Y,X,-1);
}

//----------------------------------------------------------------------
//                   CONTRAINTES UNAIRES
//----------------------------------------------------------------------
void Cinteger(interval& X)
{ interval A1(ceil(X.inf),floor(X.sup));
  X=Inter(X,A1); }
//----------------------------------------------------------------------
void Cboolean(interval& X)
{ interval nul(0), one(1);
  interval A1=Inter(nul,X);
  interval A2=Inter(one,X);
  interval A=Union(A1,A2);
  X=Inter(X,A); }
//----------------------------------------------------------------------
//                   CONTRAINTES BINAIRES
//----------------------------------------------------------------------
void Csin(interval& Y, interval& X, int sens)
/* Y=sin(X)    =>  sens=1;
   X=sin-1(Y)  =>  sens=-1;*/
{
if (sens==1) {Y=Inter(Y, Sin(X));}
if (sens==-1)
{ Y=Inter(Y, interval(-1,1));
  if (Y.isEmpty) X=interval();
  else { interval Y1=Inter(Inflate(Y,1e-5), interval(-1,1));
         double amoins=-999, aplus=-999,
         hmoins=asin(Y1.inf), hplus=asin(Y1.sup);
         int k;

         if (In(sin(X.inf),Y1)) {amoins=X.inf;}
         else  { // calcul de a-
                 k=ceil((X.inf-hmoins)/DEUX_PI);
                 amoins=hmoins+(k*DEUX_PI);
                 k=ceil((X.inf+hmoins-PI)/DEUX_PI);
                 amoins=min(amoins, PI-hmoins+(k*DEUX_PI));
                 k=ceil((X.inf-hplus)/DEUX_PI);
                 amoins=min(amoins, hplus+(k*DEUX_PI));
                 k=ceil((X.inf+hplus-PI)/DEUX_PI);
                 amoins=min(amoins, PI-hplus+(k*DEUX_PI));
               }
         if (amoins>X.sup) {X=interval();}
         else { if (In(sin(X.sup),Y1)) {aplus=X.sup;}
                   else {//calcul de a+
                         k=floor((X.sup-hmoins)/DEUX_PI);
                         aplus=hmoins+(k*DEUX_PI);
                         k=floor((X.sup+hmoins-PI)/DEUX_PI);
                         aplus=max(aplus, PI-hmoins+(k*DEUX_PI));
                         k=floor((X.sup-hplus)/DEUX_PI);
                         aplus=max(aplus, hplus+(k*DEUX_PI));
                         k=floor((X.sup+hplus-PI)/DEUX_PI);
                         aplus=max(aplus, PI-hplus+(k*DEUX_PI));
                        }
               }
        X=Inter(X, interval(amoins,aplus));
       }
}
}
//----------------------------------------------------------------------
void Ccos(interval& Y, interval& X, int sens)
/* Y=Cos(X)    =>  sens=1;
   X=Cos-1(Y)  =>  sens=-1; */
{ if (sens==1)  {Y=Inter(Y, Cos(X));}
  if (sens==-1) {interval X0=X+PI_2; Csin(Y,X0,-1); X=Inter(X,X0-PI_2);}}
//-----------------------------------------------------------------------
void Csinc(interval& Y, interval& X, int sens)
  /* Y=sinc(X)=sin(X)/X      =>  sens=1;
     X=sinc-1(X)             =>  sens=-1; */
{ interval Z(-oo,oo);
  if (sens==1) { interval U(-oo,oo), V(-oo,oo), W(-oo,oo), G(-oo,oo);
                 interval M(-oo,oo), N(-oo,oo), H(-oo,oo), I(-oo,oo);
                 interval C(-oo,oo), un(1);
                 /*Cabs(M,X,1); Cdiv(N,un,M,1);
                 if (!N.isEmpty) Y=Inter(Y,interval(-oo,N.sup));
                 Cabs(C,X,1); Cmoins(H,C,1); Cdiv(I,un,H,1);
                 if (!I.isEmpty) Y=Inter(Y,interval(I.inf,oo));*/
                 Csqr(G,X,1); U=Inter(U,G/6);
                 Cmoins(V,U,1); Cplus(W,V,un,1);
                 Y=Inter(Y,interval(-0.3,1));
                 if (!W.isEmpty) Y=Inter(Y,interval(W.inf,oo));
                 Csin(Z,X,1); Cdiv(Y,Z,X,1); }
  if (sens==-1) { Cdiv(Y,Z,X,-1); Csin(Z,X,-1); }
}
//----------------------------------------------------------------------
void Cexp(interval& Y, interval& X, int sens)
/* Y=Exp(X)           =>  sens=1;
   X=EXP-1(Y)=Log(Y)  =>  sens=-1; */
{ if (sens==1)  {Y=Inter(Y,Exp(X));}
  if (sens==-1) {X=Inter(X,Log(Y));}}
//----------------------------------------------------------------------
void Clog(interval& Y, interval& X, int sens)
/* Y=log(X)           =>  sens=1;
   X=log-1(Y)=Exp(Y)  =>  sens=-1; */
{ if (sens==1)  {Y=Inter(Y,Log(X));}
  if (sens==-1) {X=Inter(X,Exp(Y));}}
//----------------------------------------------------------------------
void Ctan(interval& Y, interval& X, int sens)
/* Y=Tan(X)           =>  sens=1;
   X=Tan-1(Y)=Atan(Y)  =>  sens=-1; */
{ if (sens==1)  {Y=Inter(Y,Tan(X));}
  if (sens==-1)
  { double amoins=-999, aplus=-999, hmoins=atan(Y.inf), hplus=atan(Y.sup);

    if (In(tan(X.inf),Y)) amoins=X.inf;
    else amoins=hmoins+(ceil((X.inf-hmoins)/PI)*PI);

    if (amoins>X.sup) X=interval();
    else { if (In(tan(X.sup),Y)) aplus=X.sup;
           else aplus=hplus+(floor((X.sup-hplus)/PI)*PI); }
     X=Inter(X,interval(amoins,aplus));
  }
} //----------------------------------------------------------------------
void Catan(interval& Y, interval& X, int sens)
/* Y=atan(X)                  =>  sens=1;
   X=atan-1(Y)=tan(Y)         =>  sens=-1; */
{ Ctan(X,Y,-sens); }
//----------------------------------------------------------------------
void Csqr(interval& Y, interval& X, int sens)
/* Y=sqr(X)           =>  sens=1;
   X=sqr-1(Y)         =>  sens=-1; */
{  interval Yd, Yg, Xd=Inter(X, interval(0,oo)), Xg=Inter(X,interval(-oo,0));
   if (!Xd.isEmpty) {Xd=Inter(Xd,Sqrt(Y));  Yd=Inter(Y,Sqr(Xd));}
   if (!Xg.isEmpty) {Xg=Inter(Xg,-Sqrt(Y)); Yg=Inter(Y,Sqr(Xg));}
   if (sens==1)     Y=Inter(Y,Union(Yd,Yg));
   else             X=Inter(X,Union(Xd,Xg));
}
//----------------------------------------------------------------------
void Csqrt(interval& Y, interval& X, int sens)
/* Y=sqrt(X)           =>  sens=1;
   X=sqrt-1(Y)         =>  sens=-1; */
{ if (sens==1) Y=Inter(Y,Sqrt(X));
  if (sens==-1) Csqr(X,Y,-sens);
}
//----------------------------------------------------------------------
void Cpower(interval& Y, interval& X, int n, int sens)
/* Y=Power(X,n)        =>   sens=1;
   X=Power(Y,1/n)      =>   sens=-1; */
{  if (n>0)
      { interval Yd, Yg;
        interval Xd=Inter(X,interval(0,oo)), Xg=Inter(X,interval(-oo,0));

        if (!Xd.isEmpty) { Xd=Inter(Xd,PowRoot(Y,1,n));
                           Yd=Inter(Y,Pow(Xd,n)); }

        if (!Xg.isEmpty) { if (n%2==0) Xg=Inter(Xg,-PowRoot(Y,1,n));
                           else Xg=Inter(Xg,PowRoot(Y,1,n));
                           Yg=Inter(Y,Pow(Xg,n)); }

        if (sens==1)  Y=Inter(Y,Union(Yd,Yg));
        else X=Inter(X,Union(Xd,Xg));  }

   else { interval Z=Pow(X,abs(n)), I(1);
          if (sens==1) { Cpower(Z,X,abs(n),1); Cdiv(Y,I,Z,1); }
          else { Cdiv(Y,I,Z,-1); Cpower(Z,X,abs(n),-1); }
        }
}
//----------------------------------------------------------------------
void Cmoins(interval& Y, interval& X, int sens)
/* Y=-X           =>  sens=1;
   X=-Y           =>  sens=-1; */
{if (sens==1)  Y=Inter(Y,-X);
 if (sens==-1) X=Inter(X,-Y);}
//----------------------------------------------------------------------
void Cegal(interval& Y, interval& X, int sens)
/* Y=X           =>  sens=1;
   X=Y           =>  sens=-1; */
{if (sens==1)   Y=Inter(Y,X);
 if (sens==-1)  X=Inter(X,Y);}
//----------------------------------------------------------------------
void Cegal(interval& Y, interval& X)
{Y=Inter(Y,X); X=Y;}
//----------------------------------------------------------------------
void Cabs (interval& Y, interval& X, int sens)
/* Y=|X|=max(X,-X)     =>     sens=1
   X=Abs-1(Y)          =>     sens=-1*/
{  interval Xd=Inter(X, interval(0,oo)), Xg=Inter(X,interval(-oo,0));
   if (sens==1)  { interval Yd=Inter(Y,Xd), Yg=Inter(Y,-Xg); Y=Union(Yd,Yg);}
   else          { Xd=Inter(Xd,Y); Xg=Inter(Xg,-Y); X=Union(Xd,Xg);}
}
//----------------------------------------------------------------------
void Cheaviside(interval& Y, interval& X, int sens, double a)
/* Y=heavyside(X)=1 if X.inf>=a
            =0 if X.sup<a
   X=heavyside-1(Y)=Empty if Inter(Y,[0,1])=Empty
              =Empty if X.sup<a & 0 \not in Y
              =Empty if X.inf>a & 1 \not in Y
              =X if X.sup<a & 0 \in Y
              =X if X.inf>a & 1 \in Y
              =X if X.inf=<a<=X.sup   =>  sens=-1 */
{ interval Z=Inter(interval(0),Y);
  interval W=Inter(interval(1),Y);
  if (sens==1)
      { interval U=Union(Z,W);
        if (X.isEmpty) Y=X;
            else {  if (Z.isEmpty && W.isEmpty) Y=Z;
                    else { Y=Inter(Y,Union(Z,W));
                           if (X.inf>=a)  Y=Inter(Y,interval(1));
                           else if (X.sup<a) Y=Inter(Y,interval(0)); }
                 }
       }
   if (sens==-1)
      {
        if (Z.isEmpty && W.isEmpty) X=Y=Z;
           else { if ((!Z.isEmpty)&&(!W.isEmpty)) X=Inter(X,interval(-oo,oo));
                  if (Z.isEmpty) X=Inter(X,interval(0,oo));
                  if (W.isEmpty) X=Inter(X,interval(-oo,0));
                }
      }
}
//----------------------------------------------------------------------
void Cstep(interval& Y, interval& X, int sens, double a)
/* Y=step(X)=1 if X.inf>a
            =0 if X.sup<a
            =[0,1] if X.inf=<a<=X.sup   =>  sens=1

   X=step-1(Y)=Empty if Inter(Y,[0,1])=Empty
              =Empty if X.sup<a & 0 \not in Y
              =Empty if X.inf>a & 1 \not in Y
              =X if X.sup<a & 0 \in Y
              =X if X.inf>a & 1 \in Y
              =X if X.inf=<a<=X.sup   =>  sens=-1 */
{ Y=Inter(Y,interval(0,1));
  if (sens==1)
     { if (!X.isEmpty)
          { if (X.inf>a) Y=Inter(Y,interval(1));
            if (X.sup<a) Y=Inter(Y,interval(0)); }
       else Y=interval();
     }
  if (sens==-1)
     { if (!Y.isEmpty)
          { interval Yh(-oo,oo), Yb(-oo,oo), Xg(-oo,oo), Xd(-oo,oo);
            Yh=Inter(Y,interval(1)); Yb=Inter(Y,interval(0));
            Xg=Inter(X,interval(-oo,a)); Xd=Inter(X,interval(a,oo));
            if (Yh.isEmpty) Xd=Inter(Xd,interval(-oo,a));
            if (Yb.isEmpty) Xg=Inter(Xg,interval(a,oo));
            X=Inter(X,Union(Xg,Xd));
          }
       else X=interval(); }

}
//----------------------------------------------------------------------
void Cramp(interval& Y, interval& X, int sens, double a)
/* Y=ramp(X)=max(0,X);
   X=ramp-1(Y) */
{ Y=Inter(Y,interval(0,oo));
  if (sens==1)  { interval Zero(a); Cmax(Y,X,Zero,1); }
  if (sens==-1) { interval Xd=Inter(X,interval(a,oo));
                  interval Xg=Inter(X,interval(-oo,a));
                  if (Y.isEmpty) Xg=Xd=interval();
                  else { Xd=Inter(Xd,Y);
                         if (Y.inf>0) Xg=interval(); }
                  X=Union(Xd,Xg);
                }
}
//----------------------------------------------------------------------
void Csign(interval& Y, interval& X, int sens, double a)
/* sign(X)=2*step(X)-1*/
{ interval un(1), deux(2);
  if (sens==1)
     { interval A1(-oo,oo); Cstep(A1,X,1,a);
       Y=Inter(Y,(deux*A1)-un);
       //interval Z=Inter(X,interval(0));
       //if (!Z.isEmpty) Y=Union(Y,interval(0));
     }
  if (sens==-1)
     { interval A2=(Y+un)/deux;
       Cstep(A2,X,-1,a); }
}
//----------------------------------------------------------------------
void Crect(interval& Y, interval& X, int sens)
{ interval V(1); Crect(Y,X,V,sens); }
//----------------------------------------------------------------------
void Ctriangle(interval& Y, interval& X, int sens)
{ interval Xg=Inter(X,interval(-oo,-0.5));
  interval Xd=Inter(X,interval(0.5,oo));
  interval Xm=Inter(X,interval(-0.5,0.5));
  interval un(1), deux(2);
  Y=Inter(Y,interval(0,1));
  if (sens==1)
     { interval Yg(0), Yd(0), Ym(0);
       if (Xg.isEmpty) Yg=interval();
       if (Xd.isEmpty) Yd=interval();
       if (Xm.isEmpty) Ym=interval();
       else Ym=Inter(Y,(-deux*Abs(X))+un);
       Y=Inter(Y,Union(Yd,Union(Yg,Ym))); }
  if (sens==-1)
     { if (Y.isEmpty) X=interval();
       else { interval A(-oo,oo);
              if (Y.inf>0) Xg=Xd=interval();
              A=Inter(A,(un-Y)/deux);
              Cabs(A,Xm,-1);
              X=Inter(X,Union(Union(Xg,Xm),Xd)); }
     }
}
//----------------------------------------------------------------------
//                    CONTRAITES TERNAIRES
//----------------------------------------------------------------------
void Crect(interval& Z, interval& X, interval& Y, int sens)
{ interval A1(-oo,oo), A2(-oo,oo), A3(-oo,oo), A4(-oo,oo), deux(2);
  X=Inter(X,interval(0,oo));
  interval aire=Y/deux;
  if (sens==1)
     { A1=Inter(A1,X+aire); Cstep(A2,A1,1);
       A3=Inter(A3,X-aire); Cstep(A4,A3,1);
       Z=Inter(Z,A2-A4); }
  if (sens==-1)
     { A2=Inter(A2,interval(0,1)); A4=Inter(A4,interval(0,1));
       A2=Inter(A2,Z+A4); A4=Inter(A4,A2-Z);
       Cstep(A2,A1,-1); Cplus(A1,X,aire,-1); Y=Inter(Y,deux*aire);
       aire=Inter(aire,Y/deux); Cstep(A4,A3,-1); Cmoins(A3,X,aire,-1);
       Y=Inter(Y,deux*aire); }
}
//----------------------------------------------------------------------
void Cchi(interval& F, interval& A, interval& B, interval& C)
{   if (A.sup<0) {Cegal(B,F);}
    else if (A.inf>0) {Cegal(C,F);};
    if (Disjoint(F,B)) {A=Inter(A,interval(0,oo));};
    if (Disjoint(F,C)) {A=Inter(A,interval(-oo,0));};
    F=Union(Inter(F,B),Inter(F,C));
}

//----------------------------------------------------------------------
void Cplus(interval& Z, interval& Y, interval& X, int sens)
/* Z=Y+X         =>  sens=1;
   Y=Z-X; X=Z-Y  =>  sens=-1; */
{ if (sens==1)  {Z=Inter(Z,Y+X);}
  if (sens==-1) {Y=Inter(Y,Z-X); X=Inter(X,Z-Y);}}
//----------------------------------------------------------------------
void Cplus(interval& Z, double& X, interval& Y,int sens)
{  if (sens==1)  {Z=Inter(Z,X+Y);};
   if (sens==-1) {Y=Inter(Y,Z-X);};
}
//----------------------------------------------------------------------
void Cplus(double& Z, interval& Y, interval& X, int sens)
{ //Luc
  if (sens==-1) {Y=Inter(Y,Z-X); X=Inter(X,Z-Y);}}
//----------------------------------------------------------------------
void Cmoins(interval& Z, interval& Y, interval& X, int sens)
/* Z=Y-X           =>  sens=1;
   Y=Z+X; X=Y-Z    =>  sens=-1; */
{ if (sens==1)  {Z=Inter(Z,Y-X);}
  if (sens==-1) {Y=Inter(Y,Z+X); X=Inter(X,Y-Z);}}
//----------------------------------------------------------------------
void Cmoins(double& Z, interval& Y, interval& X, int sens)
// Luc
{ if (sens==-1) {Y=Inter(Y,Z+X); X=Inter(X,Y-Z);}}
//----------------------------------------------------------------------
void Cmoins(interval& Z, interval& Y, double& x, int sens)
//Luc
{ if (sens==1)  {Z=Inter(Z,Y-x);}
  if (sens==-1) {Y=Inter(Y,Z+x);}
}
//----------------------------------------------------------------------
void Cmoins(interval& Z, double& y, interval& X, int sens)
//Luc
{ if (sens==1)  {Z=Inter(Z,y-X);}
  if (sens==-1) {X=Inter(X,y-Z);}
}
//----------------------------------------------------------------------
void Cprod(interval& Z, double& y, interval& X, int sens)
/*  Luc  */
{ if (sens==1)  {Z=Inter(Z,y*X);}
  if (sens==-1) {X=Inter(X,Z/y);}
}
//----------------------------------------------------------------------
void CAngle1(interval& X2,interval& Y2,interval& Theta,interval& X1,interval& Y1)
/*  Luc  */
{ interval SinTheta=Sin(Theta);
  interval CosTheta=Cos(Theta);
  interval CosThetaX1=CosTheta*X1;
  interval SinThetaX1=SinTheta*X1;
  interval CosThetaY1=CosTheta*Y1;
  interval SinThetaY1=SinTheta*Y1;
  X2=Inter(CosThetaX1-SinThetaY1,X2);
  Y2=Inter(SinThetaX1+CosThetaY1,Y2);
  Cplus (Y2,SinThetaX1,CosThetaY1,-1);
  Cmoins(X2,CosThetaX1,SinThetaY1,-1);
  Cprod(SinThetaY1,SinTheta,Y1,-1);
  Cprod(CosThetaY1,CosTheta,Y1,-1);
  Cprod(SinThetaX1,SinTheta,X1,-1);
  Cprod(CosThetaX1,CosTheta,X1,-1);
  Ccos(CosTheta,Theta,-1);
  Csin(SinTheta,Theta,-1);
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void CNorm(interval& N,interval& X,interval& Y)
{
   interval SqrX,SqrY;
    SqrX=Sqr(X);
    SqrY=Sqr(Y);
    N=Inter(N,SqrX+SqrY);
    Cplus(N,SqrX,SqrY,-1);
    Csqr(SqrY,Y,-1);
    Csqr(SqrX,X,-1);
}
//----------------------------------------------------------------------
double Det(double& ux, double& uy, double& vx, double& vy)
{   return(ux*vy-vx*uy);
}
//----------------------------------------------------------------------
interval Det(interval& ux, interval& uy, interval& vx, interval& vy)
{   return(ux*vy-vx*uy);
}
//----------------------------------------------------------------------
interval Det(interval& ux, interval& uy, double& vx, double& vy)
{   return(vy*ux-vx*uy);
}
//----------------------------------------------------------------------
void CDet(interval& det,interval& ux, interval& uy, interval& vx, interval& vy,int sens)
{   interval z1=ux*vy;
    interval z2=vx*uy;
    Cmoins(det,z1,z2,1);
    if (sens==1) return;
    Cmoins(det,z1,z2,-1);
    Cprod(z2,vx,uy,-1);
    Cprod(z1,ux,vy,-1);
}
//----------------------------------------------------------------------
void CScal(interval& s,interval& ux, interval& uy, interval& vx, interval& vy)
{   interval z1=ux*vx;
    interval z2=uy*vy;
    Cplus(s,z1,z2,1);
    Cplus(s,z1,z2,1);
    Cprod(z2,uy,vy,-1);
    Cprod(z1,ux,vx,-1);
}
//----------------------------------------------------------------------
void CDet(interval& det,double& ux, double& uy, interval& vx, interval& vy, int sens)
{   interval z1=ux*vy;
    interval z2=uy*vx;
    Cmoins(det,z1,z2,-1);
    if (sens==1) return;
    Cmoins(det,z1,z2,-1);
    Cprod(z2,uy,vx,-1);
    Cprod(z1,ux,vy,-1);
}
//----------------------------------------------------------------------
void CScal(interval& s,double& ux, double& uy, interval& vx, interval& vy)
{   interval z1=ux*vx;
    interval z2=uy*vy;
    Cplus(s,z1,z2,-1);
    Cplus(s,z1,z2,-1);
    Cprod(z2,uy,vy,-1);
    Cprod(z1,ux,vx,-1);
}

//----------------------------------------------------------------------
void CDet(interval& det,interval& ux, interval& uy, double& vx, double& vy, int sens)
{   interval z1=vy*ux;
    interval z2=vx*uy;
    Cmoins(det,z1,z2,-1);
    if (sens==1) return;
    Cmoins(det,z1,z2,-1);
    Cprod(z2,vx,uy,-1);
    Cprod(z1,vy,ux,-1);
}

//----------------------------------------------------------------------
double DistanceDirSegment(double& mx, double& my, double& theta, double& ax, double& ay, double& bx, double& by)
{      // Distance directionnelle du point m au segment [a,b]. La direction est donn\E9e par theta
       double ma_x=ax-mx;
       double ma_y=ay-my;
       double mb_x=bx-mx;
       double mb_y=by-my;
       double ab_x=bx-ax;
       double ab_y=by-ay;
       double ux=cos(theta);
       double uy=sin(theta);
       double z1=Det(ma_x,ma_y,ux,uy);
       double z2=Det(ux,uy,mb_x,mb_y);
       double z3=Det(ma_x,ma_y,ab_x,ab_y);
       double z4=Det(ux,uy,ab_x,ab_y);
       double z5=min(z1,min(z2,z3));
       double d1=z3/z4;
       return(Chi(z5,oo,d1));
}
//----------------------------------------------------------------------
double DistanceDirSegments(double& mx, double& my, double& theta, vector<double> ax, vector<double> ay, vector<double> bx, vector<double> by)
{      // Distance directionnelle relativement \E0 un polygone
       vector<double> dist(ax.size());
        for (int j=0;j<ax.size();j++)
               dist[j]=DistanceDirSegment(mx,my,theta,ax[j],ay[j],bx[j],by[j]);
        double distmin=Min(dist);
        return(distmin);
}
//----------------------------------------------------------------------
double DistanceDirCercle(double& mx, double& my, double& theta, double& cx, double& cy, double& r)
{      // Distance directionnelle du point m au cercle de centre c et de rayon r.
       double ux,uy,alpha,beta,a,b,c,delta,px1,py1,px2,py2,d1,d2;
       ux=cos(theta);
       uy=sin(theta);
       if (fabs(uy)>0.00)
       {   alpha=ux/uy;
           beta=mx-my*alpha;
           a=alpha*alpha+1;
           b=2*alpha*(beta-cx)-2*cy;
           c=(beta-cx)*(beta-cx)+cy*cy-r*r;
           delta=b*b-4*a*c;
           if (delta<0) return(oo);
           py1=(-b-sqrt(delta))/(2*a);
           px1=alpha*py1+beta;
           py2=(-b+sqrt(delta))/(2*a);
           px2=alpha*py2+beta;
           d1=Chi((px1-mx)*ux+(py1-my)*uy, oo, sqrt((px1-mx)*(px1-mx)+(py1-my)*(py1-my)));
           d2=Chi((px2-mx)*ux+(py2-my)*uy, oo, sqrt((px2-mx)*(px2-mx)+(py2-my)*(py2-my)));
           return min(d1,d2);
       }
}
//----------------------------------------------------------------------
double DistanceDirCercles(double& mx, double& my, double& theta, vector<double> cx, vector<double> cy, vector<double> r)
{      // Distance directionnelle relativement \E0 plusieurs cercles
       vector<double> dist(cx.size());
        for (int j=0;j<cx.size();j++)
               dist[j]=DistanceDirCercle(mx,my,theta,cx[j],cy[j],r[j]);
        double distmin=Min(dist);
        return(distmin);
}
//----------------------------------------------------------------------
int CinSegment(interval& mx, interval& my, double& ax, double& ay, double& bx,double& by)
{      // contracte relativement \E0 la contrainte : "m appartient au segment [a,b]"
       mx=Inter(mx,Union(interval(ax,ax),interval(bx,bx)));
       my=Inter(my,Union(interval(ay,ay),interval(by,by)));
       interval ma_x=ax-mx;
       interval ma_y=ay-my;
       double ab_x=bx-ax;
       double ab_y=by-ay;
       interval z1=interval(0,0);
       CDet(z1,ab_x,ab_y,ma_x,ma_y,-1);
       Cmoins(ma_y,ay,my,-1);
       Cmoins(ma_x,ax,mx,-1);
       if ((mx.isEmpty)||(my.isEmpty))  {mx.isEmpty=true; my.isEmpty=true;};
}
//----------------------------------------------------------------------
int CinCircle(interval& mx, interval& my, double& cx, double& cy, double& r)
{      // contracte relativement \E0 la contrainte : "m appartient au cercle de centre c et de rayon r"
       interval d_x=mx-cx;
       interval d_y=my-cy;
       interval d2_x=Sqr(d_x);
       interval d2_y=Sqr(d_y);
       interval r2=r*r;
       Cplus(r2,d2_x,d2_y,-1);
       Csqr(d2_x,d_x,-1);
       Csqr(d2_y,d_y,-1);
       mx=Inter(mx,d_x+cx);
       my=Inter(my,d_y+cy);
       if ((mx.isEmpty)||(my.isEmpty))  {mx.isEmpty=true; my.isEmpty=true;};
}
//----------------------------------------------------------------------
void CinSegments(interval& mx, interval& my, vector<double> ax, vector<double> ay, vector<double> bx, vector<double> by)
{      // contracte relativement \E0 la contrainte : "m appartient au polygone dont les segments sont les [ai,bi]"
        vector<interval> Mx(ax.size());
        vector<interval> My(ax.size());
        for (int j=0;j<ax.size();j++)
             {  interval mx0=mx;
                interval my0=my;
                CinSegment(mx0,my0,ax[j],ay[j],bx[j],by[j]);
                Mx[j]=mx0;
                My[j]=my0;
             }
        mx=Union(Mx);
        my=Union(My);
}
//----------------------------------------------------------------------
void CinCircles(interval& mx, interval& my, vector<double> cx, vector<double> cy, vector<double> r)
{      // contracte relativement \E0 la contrainte : "m appartient \E0 un des cercles de centre ci et de rayon ri"
        vector<interval> Mx(cx.size());
        vector<interval> My(cx.size());
        for (int j=0;j<cx.size();j++)
             {  interval mx0=mx;
                interval my0=my;
                CinCircle(mx0,my0,cx[j],cy[j],r[j]);
                Mx[j]=mx0;
                My[j]=my0;
             }
        mx=Union(Mx);
        my=Union(My);
}
//----------------------------------------------------------------------
void CinSegmentsOrCircles(interval& mx, interval& my,  vector<double> ax, vector<double> ay, vector<double> bx, vector<double> by, vector<double> cx, vector<double> cy, vector<double> r)
{      // contracte relativement \E0 la contrainte : "m appartient soit au polygone soit \E0 un des cercles de centre ci et de rayon ri"
        vector<interval> Mx(2);
        vector<interval> My(2);

        interval mx0=mx;
        interval my0=my;
        CinSegments(mx0,my0,ax,ay,bx,by);
        Mx[0]=mx0;
        My[0]=my0;

        mx0=mx;
        my0=my;
        CinCircles(mx0,my0,cx,cy,r);
        Mx[1]=mx0;
        My[1]=my0;

        mx=Union(Mx);
        my=Union(My);
}
//----------------------------------------------------------------------
void CLegOnWalls(interval& dist, interval& px, interval& py, interval& theta, vector<double> ax, vector<double> ay, vector<double> bx, vector<double> by)
{      // Toutes les pattes doivent \EAtre sur le mur
       interval ux=Cos(theta);
       interval uy=Sin(theta);
       interval dx=ux*dist;
       interval dy=uy*dist;
       interval leg_x=px+dx;
       interval leg_y=py+dy;
       CinSegments(leg_x, leg_y, ax, ay, bx, by);
       Cplus(leg_y,py,dy,-1);
       Cplus(leg_x,px,dx,-1);
       Cprod(dy,uy,dist,-1);
       Cprod(dx,ux,dist,-1);
       Csin(uy,theta,-1);
       Ccos(ux,theta,-1);
}
//----------------------------------------------------------------------
void CLegOnWallsOrCircles(interval& dist, interval& px, interval& py, interval& theta, vector<double> ax, vector<double> ay, vector<double> bx, vector<double> by, vector<double> cx, vector<double> cy, vector<double> r)
{      // Toutes les pattes doivent \EAtre sur le mur ou sur un des cercles
       interval ux=Cos(theta);
       interval uy=Sin(theta);
       interval dx=ux*dist;
       interval dy=uy*dist;
       interval leg_x=px+dx;
       interval leg_y=py+dy;
       CinSegmentsOrCircles(leg_x, leg_y, ax, ay, bx, by,cx,cy,r);
       Cplus(leg_y,py,dy,-1);
       Cplus(leg_x,px,dx,-1);
       Cprod(dy,uy,dist,-1);
       Cprod(dx,ux,dist,-1);
       Csin(uy,theta,-1);
       Ccos(ux,theta,-1);
}
//----------------------------------------------------------------------
int Cmin (interval& a, vector<interval>& x, int sens)
{   vector<interval> z(x.size());
    z[0]=x[0];
    for (int i=1;i<x.size();i++)
       { z[i]=interval(-oo,oo);
         Cmin(z[i],x[i],z[i-1],1);
       };
    Cegal(a,z[x.size()-1]);
    if (sens==1) return 1;
    for (int i=x.size()-1;i>=1;i--)
       { Cmin(z[i],x[i],z[i-1],-1); ;
         if (z[i].isEmpty) return -1;
       }
}
//----------------------------------------------------------------------
int q_in (double x, vector<interval>& y)
{   int n=0;
    for (int i=0;i<y.size();i++)
       if (In(x,y[i])) n++;
    return n;
}
//----------------------------------------------------------------------
void C_q_in (interval& x, int q, vector<interval>& y)
{   vector<borne> V;
    V.push_back(borne(x.inf-0.00001,0));
    V.push_back(borne(x.sup+0.00001,0));
    for (int i=0;i<y.size();i++)
        {V.push_back(borne(y[i].inf,1));
         V.push_back(borne(y[i].sup,-1));
        };
    sort(V.begin(),V.end());
    int sum=0;
    int imin=V.size();
    int imax=-1;
    for (int i=0;i<V.size();i++)
    {  sum=sum+V[i].ouverture;
       if ((sum>=q)&(V[i].ouverture==1)&(V[i].val>=x.inf)&(V[i].val<=x.sup))
          {imin=min(imin,i); };
       if ((sum>=q-1)&(V[i].ouverture==-1)&(V[i].val>=x.inf)&(V[i].val<=x.sup))
          {imax=max(imax,i); };
    }
    if (imax==-1) x=interval();
    else x=interval(V[imin].val,V[imax].val);
}
//----------------------------------------------------------------------
int CDistanceDirSegment(interval& dist,interval& mx, interval& my, interval& theta, double& ax, double& ay, double& bx,double& by, int sens)
// la distance dist entre le point m=(mx,my) au segment [a,b] suivant le vecteur u
{      if ((dist.isEmpty)||(mx.isEmpty)||(my.isEmpty)||(theta.isEmpty))
               {  dist.isEmpty=true; mx.isEmpty=true; my.isEmpty=true; theta.isEmpty=true; return -1;};
       interval ma_x=ax-mx;       interval ma_y=ay-my;
       interval mb_x=bx-mx;       interval mb_y=by-my;
       double ab_x=bx-ax;         double ab_y=by-ay;
       interval ux=Cos(theta);    interval uy=Sin(theta);
       interval z1=Det(ma_x,ma_y,ux,uy);
       interval z2=Det(ux,uy,mb_x,mb_y);
       interval z3=Det(ma_x,ma_y,ab_x,ab_y);
       interval z4=Det(ux,uy,ab_x,ab_y);
       interval z5=Min(z1,z2,z3);
       interval d1=z3/z4;
       interval infty=interval(oo,oo);
       Cchi(dist,z5,infty,d1);
            //REDONDANT
               Cchi(dist,z3,infty,dist);
               Cchi(dist,z4,infty,dist);
       if (sens==1) return(1);
       Cdiv(d1,z3,z4, -1);
       Cmin(z5,z1,z2,z3,-1);
       CDet(z4,ux,uy,ab_x,ab_y,-1);
       CDet(z3,ma_x,ma_y,ab_x,ab_y,-1);
       CDet(z2,ux,uy,mb_x,mb_y,-1);
       CDet(z1,ma_x,ma_y,ux,uy,-1);
       Csin(uy,theta,-1);           Ccos(ux,theta,-1);
       Cmoins(mb_y,by,my,-1);       Cmoins(mb_x,bx,mx,-1);
       Cmoins(ma_y,ay,my,-1);       Cmoins(ma_x,ax,mx,-1);
}
//----------------------------------------------------------------------
void CDistanceDirSegments(interval& distmin, interval& mx, interval& my, interval& theta, vector<double> ax, vector<double> ay, vector<double> bx, vector<double> by)
{      // Distance directionnelle relativement \E0 un polygone
        vector<interval> dist(ax.size());
        for (int j=0;j<ax.size();j++)  dist[j]=interval(0,oo);
        for (int j=0;j<ax.size();j++)
               CDistanceDirSegment(dist[j],mx,my,theta,ax[j],ay[j],bx[j],by[j],1);
        Cmin(distmin,dist,-1);
        for (int j=ax.size()-1;j>=0;j--)
               CDistanceDirSegment(dist[j],mx,my,theta,ax[j],ay[j],bx[j],by[j],-1);
}
//----------------------------------------------------------------------
int CAngle(interval& X2,interval& Y2,interval& Theta,interval& X1,interval& Y1,bool StrongAngle)
{   CAngle1(X2,Y2,Theta,X1,Y1);
    if (StrongAngle)
    {   interval Theta2=-Theta;
        CAngle1(X1,Y1,Theta2,X2,Y2);
        Theta=-Theta2;
        interval SqrX1,SqrY1,SqrX2,SqrY2,N2;
        N2=Inter(Sqr(X1)+Sqr(Y1),Sqr(X2)+Sqr(Y2));
        CNorm(N2,X1,Y1);
        CNorm(N2,X2,Y2);
    }
    if (X2.isEmpty||Y2.isEmpty||Theta.isEmpty||X1.isEmpty||Y1.isEmpty) return -1;
}
//----------------------------------------------------------------------
void Cprod(interval& Z, interval& Y, interval& X, int sens)
/*  Z=Y*X           =>  sens=1;
   Y=Z/X; X=Z/Y    =>  sens=-1;  */
{ if (sens==1)  {Z=Inter(Z,Y*X);}
  if (sens==-1) {
if (In(0,Z)==false)
{
  interval Xd, Xg;
  interval Yd, Yg;
  Yg=Inter(Y,interval(-oo,0)), Yd=Inter(Y,interval(0,oo));
  Xg=Inter(X,interval(-oo,0)), Xd=Inter(X,interval(0,oo));

  if (Z.inf>0)
  {
        if (  Xd.sup*Yd.sup<Z.inf)
        {
        Xd=interval();
        Yd=interval();
        }
        else
        {
                if ((Xd.sup!=0)&&(Yd.sup!=0))
                {
                        Xd=Inter(Xd,interval(Z.inf/Yd.sup,oo));
                        Yd=Inter(Yd,interval(Z.inf/Xd.sup,oo));
                }
        }
        if (  Xg.inf*Yg.inf<Z.inf)
        {
                Xg=interval();
                Yg=interval();
        }
        else
        {       if ((Xg.inf!=0)&&(Yg.inf!=0))
                {
                        Xg=Inter(Xg,interval(Z.inf/Yg.inf,-oo));
                        Yg=Inter(Yg,interval(Z.inf/Xg.inf,-oo));
                }
        }
  X=Inter(X,Union(Xd,Xg));
  Y=Inter(Y,Union(Yd,Yg));
  }
  if (Z.sup<0)
  {
        if (  Xg.inf*Yd.sup>Z.sup)
        {
                Xg=interval();
                Yd=interval();
        }
        else
        {
                if ((Xg.inf!=0)&&(Yd.sup!=0))
                {
                        Xg=Inter(Xg,interval(Z.sup/Yd.sup,-oo));
                        Yd=Inter(Yd,interval(Z.sup/Xg.inf,oo));
                }
        }
        if (  Xd.sup*Yg.inf>Z.sup)
        {
                Xd=interval();
                Yg=interval();
        }
        else
        {
                if ((Xd.sup!=0)&&(Yg.inf!=0))
                {
                        Xd=Inter(Xd,interval(Z.sup/Yg.inf,oo));
                        Yg=Inter(Yg,interval(Z.sup/Xd.sup,-oo));
                }
        }
        X=Inter(X,Union(Xd,Xg));
        Y=Inter(Y,Union(Yd,Yg));
  }
}
  Y=Inter(Y,Z/X); X=Inter(X,Z/Y); //Y=Inter(Y,Z/X);
                }
}
//----------------------------------------------------------------------
void Cdiv(interval& Z, interval& Y, interval& X, int sens)
   // Z=Y/X           =>  sens=1;
    // Y=Z*X; X=Y/Z    =>  sens=-1;
{ if (sens==1)   Z=Inter(Z,Y/X);
  if (sens==-1) {Y=Inter(Y,Z*X); X=Inter(X,Y/Z);}}
//----------------------------------------------------------------------
void Cmin (interval& a, interval& b, interval& c, int sens)
/* a=min(b,c)                       =>  sens=1;
   b=min-1(a,c); c=min-1(a,b)       =>  sens=-1; */
{   //if ((a.isEmpty||b.isEmpty)||c.isEmpty) a=b=c=interval();
    if (sens==1)   { a=Inter(a,Min(b,c)); }
    if (sens==-1)  { if (Disjoint(a,b)) c=Inter(c,a);
                     else { if (Disjoint(a,c)) b=Inter(b,a);}
                     interval temp(a.inf,oo);
                     b=Inter(b,temp); c=Inter(c,temp);}
}
//----------------------------------------------------------------------
void Cmin (interval& a, interval& b, interval& c, interval& d,  int sens)
// contrainte quaternaire   a=min(b,c,d)
{   interval z1=Min(b,c);
    Cmin(a,z1,d,1);
    if (sens==1) return;
    Cmin(a,z1,d,-1);
    Cmin(z1,b,c,-1);
}
//----------------------------------------------------------------------
void Cmax (interval& a, interval& b, interval& c, int sens)
/* a=max(b,c)                       =>  sens=1;
   b=max-1(a,c); c=max-1(a,b)       =>  sens=-1; */
{   if (sens==1)   { a=Inter(a,Max(b,c)); }
    if (sens==-1)  { if (Disjoint(a,b)) c=Inter(c,a);
                     else { if (Disjoint(a,c)) b=Inter(b,a);}
                                interval temp(-oo,a.sup);
                                b=Inter(b,temp); c=Inter(c,temp);}
}

//----------------------------------------------------------------------










