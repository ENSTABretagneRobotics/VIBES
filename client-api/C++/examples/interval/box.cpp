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

#include "box.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <math.h>

//! \todo Make QT specific inclusions conditional or implement Qt specific functions in a separate unit.
// #include <QDebug>

using namespace std;

//*********************************************************************
//******************       CONSTRUCTEURS         **********************
//*********************************************************************
box::box ()
{   dim=0;
    data=new interval[1];
}
//----------------------------------------------------------------------
box::box (int a)
{   dim=a;
    data=new interval[dim];
}
//----------------------------------------------------------------------
box::box (interval x)
{   dim=1;
    data=new interval[dim];
    (*this)[1]=x;
}
//----------------------------------------------------------------------
box::box (interval x,interval y)
{   dim=2;
    data=new interval[dim];
    (*this)[1]=x;
    (*this)[2]=y;
}
//----------------------------------------------------------------------
box::box (interval x,interval y, interval z)
{   dim=3;
    data=new interval[dim];
    (*this)[1]=x;
    (*this)[2]=y;
    (*this)[3]=z;
}
//----------------------------------------------------------------------
box::box (interval x,int n)
{   dim=n;
    data=new interval[dim];
    for (int k=1; k<=dim; k++) (*this)[k]=x;
}
//----------------------------------------------------------------------
box::box (const box &X)
{       if (&X==this) return;
    dim=X.dim;
    data=new interval[dim];
    for (int k=1; k<=dim; k++) (*this)[k]=X[k];
}
//----------------------------------------------------------------------
box::~box () {delete [] data;}
//----------------------------------------------------------------------
//*********************************************************************
//******************    FUNCTIONS MEMBRES     *************************
//*********************************************************************

//------------------------------------------------------------------------------
interval& box::operator[] (int i)  const
{return data[i-1];}
//----------------------------------------------------------------------
void box::Resize (int dim1)
{  box X(dim1);
   for (int k=1; k<=dim1; k++)
       { X[k]=(*this)[k];
     X[k].isEmpty=false;
       }
   delete [] data; (*this)=X;
}

bool box::IsEmpty(void) const
{  if (dim==0) return true;
   for (int i=1; i<=dim; i++)
   { if ((*this)[i].isEmpty)  return true;}
   return false;
}

double box::Width(void) const
{   box X=*this;
    int i=AxePrincipal(X);
    interval Xi=X[i];
    double w=Xi.sup-Xi.inf;
    return (w);
}

double box::SumWidth(void)
{  box X=*this;
   if (X.IsEmpty()) return -oo;
   double w=X[1].sup-X[1].inf;
   for (int i=2; i<=X.dim; i++)
        w=w+X[i].sup-X[i].inf;
   return w;
}


box& box::Intersect(const box& Y)
{  box X=*this;
   box Z=Inter(X,Y);
   *this=Z;
   return *this;
}



//----------------------------------------------------------------------
box& box::operator= (const box& X)
{  delete [] data;
   dim=X.dim;
   data=new interval[dim];
   for (int k=1; k<=dim; k++) (*this)[k]=X[k];
   return *this;
}
//----------------------------------------------------------------------
box operator&(const box& X, const box& Y)
{  return Inter(X,Y);
}

//----------------------------------------------------------------------
box operator|(const box& X, const box& Y)
{  return Union(X,Y);
}


//----------------------------------------------------------------------
box operator+(const box& X, const box& Y)
{  box Ans(Size(X));
   for (int k=1; k<=Size(Ans); k++) Ans[k]=X[k]+Y[k];
   return Ans;
}
//----------------------------------------------------------------------
box operator-(const box& X)
{  box Ans(Size(X));
   for (int k=1; k<=Size(Ans); k++) Ans[k]=-X[k];
   return Ans;
}
//----------------------------------------------------------------------
box operator-(const box& X, const box& Y) {return (X+(-Y)); }
//----------------------------------------------------------------------
box operator*(const interval& a,const box& X)
    { box Ans(Size(X));
          for (int k=1; k<=Size(Ans); k++) Ans[k]=a*X[k];
      return Ans;
    }
//----------------------------------------------------------------------
box operator*(const double a,const box& X)
    { box Ans(Size(X));
          for (int k=1; k<=Size(Ans); k++) Ans[k]=a*X[k];
      return Ans;
    }
//----------------------------------------------------------------------
box operator*(const box& X, const interval& a)
    {return (a*X);}

//----------------------------------------------------------------------
ostream& operator<<(ostream& os, const box& X)
{   os << "box :"<<"\t dim="<<Size(X)<<"\n";
    if (X.IsEmpty()) os<<"EmptyBox";
        for (int i=1; i<=Size(X); i++)
    os << "  " <<i<< ": "<<X[i]<< "\n";
    return (os);
}

//----------------------------------------------------------------------
//! \todo Factorize ostream and QDebug formatted output code.
//QDebug operator<<(QDebug os, const box&X)
//{   os.nospace() << "box :"<<"\t dim="<<Size(X)<<"\n";
//    if (X.IsEmpty()) os.nospace()<<"EmptyBox";
//    for (int i=1; i<=Size(X); i++)
//        os.nospace() << "  " <<i<< ": "<<X[i]<< "\n";
//    return (os.space());
//}




//*********************************************************************
//******************    FONCTION NON MEMBRES     **********************
//*********************************************************************
void Update(box& X)
{ for (int i=1; i<=Size(X); i++)
  { if (X[i].isEmpty)
       { for (int j=1; j<=Size(X); j++)
             X[j]=interval(); return; }
   }
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
int Size(const box& X) {return X.Size();}
//----------------------------------------------------------------------
box Inf(box X)
{   box Ans(Size(X));
    for (int k=1; k<=Size(Ans); k++)  Ans[k]=X[k].inf;
    return Ans;
}
//----------------------------------------------------------------------
box Sup(box X)
{   box Ans(Size(X));
    for (int k=1; k<=Size(Ans); k++) Ans[k]=X[k].sup;
    return Ans;
}
//----------------------------------------------------------------------
box Concat(const box& X, const box& Y)
/*  Produit Cartesien ou Concat�nation de deuX pav�s X et y :
       Ans=[X,Y]     =>     Ans=Concat(X,Y); */
{  double dim=X.Size()+Y.Size(); box Ans(dim);
   if ((!X.IsEmpty())&&(!Y.IsEmpty()))
   { for (int i=1; i<=dim; i++)
         { if (i<=Size(X)) Ans[i]=X[i]; else Ans[i]=Y[i-X.Size()]; }
   }
   return Ans;
}
//----------------------------------------------------------------------
box Proj(const box& X, int i, int j)
/* Projection du pav� X dans un espace de dimension dim=(j-i)+1;
   X=[[X1],[X2],..,[Xi],..,[Xj],..[Xn]]
   =>  Proj(X,i,j)=[[Xi],..,[Xj]] et Proj(X,i,i)=[Xi] */
{  int dim=abs(j-i)+1; box Ans(dim);
   if (!X.IsEmpty())
      { int lb=min(i,j);
        for (int k=1; k<=dim; k++) Ans[k]=X[k+lb-1]; }
   return Ans;
}
//----------------------------------------------------------------------
int AxePrincipal(const box& X)
    { int kmax=1;
      double widthmax=Width(X[kmax]);
      for (int k=2; k<=Size(X); k++)
          { if (Width(X[k])>widthmax)
           {kmax=k; widthmax=Width(X[k]);}
      }
     return kmax;
     }
//----------------------------------------------------------------------
int AxePrincipal(box& X, box& Y)
{ int kmax=1;
  double widthmax=Width(X[kmax])*fabs(Y[kmax].inf);
  for (int k=2; k<=Size(X); k++)
  {	if (Width(X[k])*fabs(Y[k].inf)>widthmax)
    { kmax=k; widthmax=Width(X[k])*fabs(Y[k].inf);}
  }
  return kmax;
}
//----------------------------------------------------------------------
int AxePrincipal(box& X, vector<int>& v)
{ int kmax=v[1], SizeV=v.size()-1;
  double widthmax=Width(X[kmax]);
  for (int k=2; k<=SizeV; k++)
      { if (Width(X[v[k]])>widthmax) {kmax=v[k]; widthmax=Width(X[v[k]]);}}
  return kmax;
}
//----------------------------------------------------------------------
void Bisect(box& X, box& X1, box& X2)
{ BisectAlong(X,X1,X2,AxePrincipal(X));	}
//----------------------------------------------------------------------
void Bisect(box& X, box& X1, box& X2, vector<int>& v)
{ BisectAlong(X,X1,X2,AxePrincipal(X,v)); }
//----------------------------------------------------------------------
void Bisect(box& X, box& X1, box& X2, box& V)
{   BisectAlong(X,X1,X2,AxePrincipal(X,V));  }
//----------------------------------------------------------------------
void BisectAlong(box& X, box& X1, box& X2, int i)
{  X1=X2=X; X1[i].sup=X2[i].inf=((X[i].inf)+(1.01*X[i].sup))/2.01; }
//----------------------------------------------------------------------
void BisectHere(box& X, box& X1, box& X2, int i, double here) //Used by the 3-B consistency
{	X1=X2=X;
    X1[i].sup=here;
    X2[i].inf=here;
}
//----------------------------------------------------------------------
void TrisectAlong(box& X, box& X1, box& X2, box& X3, int i)
{  X1=X2=X3=X;
   X1[i].sup=X[i].inf+(Width(X[i])/3);
   X2[i].inf=X1[i].sup;
   X2[i].sup=X2[i].inf+(Width(X[i])/3);
   X3[i].inf=X2[i].sup;
}
//-----------------------------------------------------------------------
void Trisect(box& X, box& X1, box& X2, box& X3)
{ TrisectAlong(X,X1,X2,X3,AxePrincipal(X)); }
//-----------------------------------------------------------------------
void DecoupAlong(box& X, box& X1, box& X2, int i)
{  X1=X2=X;
   X1[i].sup=X[i].inf+(Width(X[i])/3);
   X2[i].inf=X1[i].sup;
}
//------------------------------------------------------------------------
void Decoup(box& X, box& X1, box& X2)
{ DecoupAlong(X,X1,X2,AxePrincipal(X));}
//-----------------------------------------------------------------------
box Rand(const box& X)
{ int sizeX=Size(X); box Ans(sizeX);
  //if (X.IsEmpty()) Ans=EmptyBox(sizeX);
  if (X.IsEmpty()) Ans=EmptyBox(X);
  else { for (int k=1; k<=sizeX; k++) Ans[k]=Rand(X[k]); }
  return Ans;
}
//-----------------------------------------------------------------------
box Center(const box& X)
{ int sizeX=Size(X); box Ans(sizeX);
  if (X.IsEmpty()) Ans=EmptyBox(X);
  else { for (int k=1; k<=sizeX; k++) Ans[k]=Center(X[k]); }
  return Ans;
}
//-----------------------------------------------------------------------
box Center(const box& X, vector<int>& v)
{ int sizev=v.size()-1; box Ans=X;
  for (int k=1; k<=sizev; k++) Ans[v[k]]=Center(X[v[k]]);
  return Ans;
}
//-----------------------------------------------------------------------
bool Disjoint (const box& X, const box& Y)
{   if (X.IsEmpty()||Y.IsEmpty()) return true;
    for (int i=1; i<=Size(X); i++)
        if (Disjoint(X[i],Y[i])) return true;
    return false;
}
//----------------------------------------------------------------------
iboolean In(box X, box Y)
{  if (X.IsEmpty()||Y.IsEmpty()) return ifalse;
   iboolean r=itrue;
   for (int k=1; k<=X.Size(); k++)
       { iboolean in1=In(X[k],Y[k]);
     if (in1==false) return ifalse;
     else if (in1.value==iperhaps) r=iperhaps; }
   return r;
}
//------------------------------------------------------------------------------
double decrease(const box& X, const box& Y)
{    double e=0;
     for (int k=1; k<=X.Size(); k++)
     {  if ((X[k].isEmpty)||(Y[k].isEmpty)) return (-1);
            double e1=0;
            double Xinf=X[k].inf, Xsup=X[k].sup;
            double Yinf=Y[k].inf, Ysup=Y[k].sup;
            if (Xsup>=Ysup) e1=max(e1,Xsup-Ysup);
            if (Xinf<=Yinf) e1=max(e1,Yinf-Xinf);
            if (Xsup<Ysup) e1=max(e1,Ysup-Xsup);
            if (Xinf>Yinf) e1=max(e1,Xinf-Yinf);
            e=max(e,e1);
         }
     return e;
}
//----------------------------------------------------------------------
double decrease(const box& X, const box& Y, vector<int> Tab)
{    double e=0; int size=Tab.size()-1;
     for (int k=1; k<=size; k++)
     {  if ((X[Tab[k]].isEmpty)||(Y[Tab[k]].isEmpty)) return (-1);
            double e1=0;
            double Xinf=X[Tab[k]].inf, Xsup=X[Tab[k]].sup;
            double Yinf=Y[Tab[k]].inf, Ysup=Y[Tab[k]].sup;
            if (Xsup>=Ysup) e1=max(e1,Xsup-Ysup);
            if (Xinf<=Yinf) e1=max(e1,Yinf-Xinf);
            if (Xsup<Ysup) e1=max(e1,Ysup-Xsup);
            if (Xinf>Yinf) e1=max(e1,Xinf-Yinf);
            e=max(e,e1);
         }
     return e;
}
//----------------------------------------------------------------------
double Eloignement(box& X, box& Y)
{  if ((X.IsEmpty())||(Y.IsEmpty())) return oo;
   double e=0;
   for (int k=1; k<=Size(X); k++) {e=max(e,Eloignement(X[k],Y[k]));}
   return e;
}
//------------------------------------------------------------------------------
double Eloignement2(box& X, box& Y)
{// prend le point X1 de X qui est le plus eloigne de [Y] et renvoie la
        // distance de X1 avec [Y]
            Update(X); Update(Y);
            if (Subset(X,Y)) return 0;
            double e=0;
        for (int k=1; k<=X.Size(); k++)
                {double e1=0;
                 double Xinf=X[k].inf;
                 double Xsup=X[k].sup;
                 double Yinf=Y[k].inf;
                 double Ysup=Y[k].sup;
                 if (Xsup>Ysup) e1=max(e1,Xsup-Ysup);
                 if (Xinf<Yinf) e1=max(e1,Yinf-Xinf);
                 e=max(e,e1);
                 }
            return e;
}
//------------------------------------------------------------------------------
double EloignementRelatif2(box& X,box& Y)
{// prend le point X1 de X qui est le plus eloigne de [Y] et renvoie la
        // distance de X1 avec [Y]
             Update(X); Update(Y);
             if (Subset(X,Y)) return 0;
             if (Y.IsEmpty()) return oo;
             double e=0;
         for (int k=1; k<=X.Size(); k++)
                {double e1=0;
                 double Xinf=X[k].inf;
                 double Xsup=X[k].sup;
                 double Yinf=Y[k].inf;
                 double Ysup=Y[k].sup;
                 if (Xsup>Ysup) e1=max(e1,fabs((Xsup-Ysup)/Ysup));
                 if (Xinf<Yinf) e1=max(e1,fabs((Yinf-Xinf)/Yinf));
                 e=max(e,e1);
                 }
             return e;
}
//----------------------------------------------------------------------

box EmptyBox(int n)
{    return (box(n));
}

//----------------------------------------------------------------------

box EmptyBox(const box& X)
{    return (box(Size(X)));
}
//----------------------------------------------------------------------

box Inter(const box& X, const box& Y)
{   box Ans(Size(X));
    if ((X.IsEmpty())||(Y.IsEmpty())) {return Ans;}
    for (int k=1; k<=Size(Ans); k++)
        { Ans[k]=Inter(X[k],Y[k]);
          if (Ans[k].isEmpty) {Update(Ans); return Ans;}
        }
    return Ans;
}
//----------------------------------------------------------------------
box Inter (vector<box>& x)
{   box E=EmptyBox(0);
    if (x.size()==0) return E;
    box r=x[0];
    for (int i=1;i<x.size();i++)
       r=Inter(r,x[i]);
    return r;
}
//----------------------------------------------------------------------
box Union (vector<box>& x)
{   box E=EmptyBox(0);
    if (x.size()==0) return E;
    box r=x[0];
    for (int i=1;i<x.size();i++)
       r=Union(r,x[i]);
    return r;
}
//----------------------------------------------------------------------
box Inflate(box& X, double eps)
{   Update(X);
    box Ans(Size(X));
    for (int k=1; k<=Size(Ans); k++)  {Ans[k]=Inflate(X[k],eps);}
    return Ans;
}
//----------------------------------------------------------------------
bool Prop(box& X, box& Y)  // Normalement X is a subset of y (used in SIVEX)
{ if (X.IsEmpty()) return false;
  if (Y.IsEmpty()) return false;
  for (int k=1; k<=Size(X); k++)
       if ((X[k].inf==Y[k].inf)||(X[k].sup==Y[k].sup)) return (true);
  return false;
}
//----------------------------------------------------------------------
box Union(const box& X, const box& Y)
{   box Ans(max(Size(X),Size(Y)));
    if (X.IsEmpty()) return (Y);
    if (Y.IsEmpty()) return (X);
    for (int k=1; k<=Size(Ans); k++)  Ans[k]=Union(X[k],Y[k]);
    return Ans;
}


//----------------------------------------------------------------------
bool Subset(const box& X, const box& Y)
{   if (X.IsEmpty()) return true;
    if (Y.IsEmpty()) return false;
    bool b=true;
    for (int k=1; k<=Size(X); k++) b = b && Subset(X[k],Y[k]);
    return (b);
}
//----------------------------------------------------------------------
bool SubsetStrict(box& X, box& Y)
{   if (Y.IsEmpty()) return false;
    if (X.IsEmpty()) return true;
    bool b=true;
    for (int k=1; k<=Size(X); k++) b = b && SubsetStrict(X[k],Y[k]);
    return (b);
}
//----------------------------------------------------------------------
bool Subset(box& X, box& Y,double epsilon)
{   if (Subset(X,Y)==false) return false;
    for (int k=1; k<=Size(X); k++)
    if (Subset(X[k],Y[k],epsilon)) return true;
    return (false);
}
//----------------------------------------------------------------------
void Sucre(box& P, box& S)    //Sucre le bout de P qui est dans S
{   int sum=0;
    int j=1;
    for (int i=1; i<=P.Size(); i++)
       {if (Subset(P[i],S[i])) sum++; else j=i;}
    if (sum==(P.Size())-1)
       if (!Subset(S[j],P[j])) Sucre(P[j],S[j]);
};
//----------------------------------------------------------------------
double Volume(box& X)
{   double vol=1;
    for (int i=1;i<=Size(X);i++) vol=vol*Width(X[i]);
    return vol;
};
//----------------------------------------------------------------------
double Width(const box& X) {
return Width(X[AxePrincipal(X)]);}
//----------------------------------------------------------------------
double Width(box& X, vector<int>& v)  {
return Width(X[AxePrincipal(X,v)]);}
//----------------------------------------------------------------------
double Width(box& X, box& Y)
{ int i=AxePrincipal(X,Y);
  return (Width(X[i])*fabs(Y[i].inf));	}
//----------------------------------------------------------------------
double Marge(box X, box Y)
{  if ((X.IsEmpty())||(Y.IsEmpty())) return -oo;
   double ans=Marge(X[1],Y[1]);
   for (int i=2; i<=Size(X); i++)
        ans=min(ans,Marge(X[i],Y[i]));
   return ans;
}
//-----------------------------------------------------------------------
interval ProduitScalaire(box& U, box& V)
{  interval sum=0;
   for (int i=1; i<=Size(U); i++)  sum=sum+U[i]*V[i];
   return (sum);
}
//----------------------------------------------------------------------
interval Determinant(box& U, box& V)
{   interval u1=U[1];
    interval v2=V[2];
    interval v1=V[1];
    interval u2=U[2];
    interval r=u1*v2-v1*u2;
    return u1*v2-v1*u2;
}

 // Calcule l'angle entre 2 vecteur de dimension 2
double Angle (box& V, box& W)
// Attention, il faut des vecteurs et non des box et les vecteurs doivent etre
// de dimension 2
{   if ((Size(V)!=2)||(Size(W)!=2))
    {
       cout<<"error";
    }
    double n2,costeta,sinteta;
    interval nv=Norm(V);
    interval nw=Norm(W);
    interval nvw=nv*nw;
    n2=Todouble(nvw);
    costeta=Todouble(ProduitScalaire(V,W)/n2);
    sinteta=Todouble(Determinant(V,W))/n2;
    return(Arccossin(costeta,sinteta));
}
//----------------------------------------------------------------------
interval Norm(box X)
{ if (X.IsEmpty()) return interval();
  interval r=0;
  for (int i=1; i<=Size(X); i++) r=r+Sqr(X[i]);
  return (Sqrt(r));
}
//----------------------------------------------------------------------
interval NormEuclid(box X, box Y)
{ if (Size(X)!=Size(Y)) return interval();
  if (X.IsEmpty()||Y.IsEmpty()) return interval();
  interval r=0;
  for (int i=1; i<=Size(X); i++) r=r+Sqr(Y[i]-X[i]);
  return (Sqrt(r));
}
//----------------------------------------------------------------------
interval NormInf(box X, box Y)
{ if (Size(X)!=Size(Y)) return interval();
  if (X.IsEmpty()||Y.IsEmpty()) return interval();
  interval ans=Abs(Y[1]-X[1]);
  for (int i=1; i<=Size(X); i++) ans=Max(ans,Abs(Y[i]-X[i]));
  return ans;
}
//----------------------------------------------------------------------
box Zeros(int d)
{   box Ans(d);
    for (int k=1; k<=d; k++) Ans[k]=0;
    return Ans;
}
//----------------------------------------------------------------------
box Empty(int d)
{  box Ans(d);
   for (int k=1; k<=d; k++)  Ans[k]=interval();
   return Ans;
}
//----------------------------------------------------------------------
box Infinity(int a)
{ box Ans(a);
  for (int k=1; k<=a; k++)  Ans[k]=interval(-oo,oo);
  return Ans;
}
//----------------------------------------------------------------------
bool IsBox(box X)
{ if (X.IsEmpty()) return false;
  for (int i=1; i<=Size(X); i++)
      { if (Width(X[i])==0) return false; }
  return true;
}
//----------------------------------------------------------------------
void Cplus(box& Z, box& X, box& Y, int sens)
{        for (int k=1; k<=Size(X); k++)
             Cplus(Z[k],X[k],Y[k],sens);
}
//----------------------------------------------------------------------
void Cmoins(box& Z, box& X, box& Y, int sens)
{        for (int k=1; k<=Size(X); k++)
             Cmoins(Z[k],X[k],Y[k],sens);
}
//----------------------------------------------------------------------
void CProd(box& Y, interval& a, box& X, int sens)
{        for (int k=1; k<=Size(X); k++)
             Cprod(Y[k],a,X[k],sens);
}

//----------------------------------------------------------------------
void C_q_in (box& x, int q, vector<box>& y)
{   vector <interval> yi(y.size());
    for (int i=1;i<=x.Size();i++)
    {
       for (int j=0;j<y.size();j++)
           yi[j]= y[j][i];
       C_q_in (x[i], q, yi) ;
    }
}


//----------------------------------------------------------------------
void Cnorm(interval&R, box& X)
{   int imax=X.Size();
    box Sum2(imax);
    box X2(imax);
    for (int i=1; i<=imax; i++)
        {X2[i]=interval(-oo,oo);
         Csqr(X2[i],X[i],1);
        };
    Sum2[1]=X2[1];
    for (int i=2; i<=imax; i++)
      {Sum2[i]=interval(-oo,oo);
       Cplus(Sum2[i],Sum2[i-1],X2[i],1);
      };
    Csqr(Sum2[imax],R,-1);
    Csqr(Sum2[imax],R,1);
    for (int i=imax; i>=2; i--)
    {Cplus(Sum2[i],Sum2[i-1],X2[i],-1);
    }
    X2[1]=Sum2[1];
    for (int i=imax; i>=1; i--)
    {Csqr(X2[i],X[i],-1);};
}

//----------------------------------------------------------------------

void Cdistance(interval& R, box& X, box& Y)
{  box Z=Y-X;
   Cnorm(R,Z);
   Cmoins(Z,Y,X,-1);
}
//----------------------------------------------------------------------
void CProdScalaire(interval& R, box& X, box& Y)
{   int imax=X.Size();
    box SumXiYi(imax);
    box XiYi(imax);
    for (int i=1; i<=imax; i++)
        {XiYi[i]=X[i]*Y[i];
        };
    SumXiYi[1]=XiYi[1];
    for (int i=2; i<=imax; i++)
      {SumXiYi[i]=interval(-oo,oo);
       Cplus(SumXiYi[i],SumXiYi[i-1],XiYi[i],1);
      };
    R=Inter(R,SumXiYi[imax]);
    SumXiYi[imax]=R;
    for (int i=imax; i>=2; i--)
    {Cplus(SumXiYi[i],SumXiYi[i-1],XiYi[i],-1);
    }
    XiYi[1]=SumXiYi[1];
    for (int i=imax; i>=1; i--)
        {Cprod(XiYi[i],X[i],Y[i],-1);};
;
}


//----------------------------------------------------------------------
void COrtho(box& X, box& Y)
{   interval S(0,0);
    CProdScalaire(S, X, Y);
};
//----------------------------------------------------------------------











