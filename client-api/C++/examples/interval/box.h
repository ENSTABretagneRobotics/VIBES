#ifndef __BOX__
#define __BOX__

#include "interval.h"
#include "iboolean.h"

#include <iosfwd> // Forward declarations for iostream
#include <vector> //!< \todo Forward declare template classes vector<int> and vector<box>

class QDebug;

class box
{
    interval* data;
    int dim;

public:

    //---------------------CONSTRUCTEURS--------------------------------------------
    box ();
    box (int);
    box (interval,int);
    box (interval x);
    box (interval x,interval y);
    box (interval x,interval y, interval z);
    box (const box&);
    //---------------------DESTRUCTEUR----------------------------------------------
    ~box ();
    //---------------------OPERATEURS-----------------------------------------------
    interval& operator[] (int) const ;
    box& operator=(const box&);

    void Resize (int);
    int Size(void) const {return dim;}

    box& Intersect(const box& Y);
    bool IsEmpty (void) const;
    double Width(void) const;
    double SumWidth(void);
};


//----------------------OPERATEURS----------------------------------------------
box      operator&(const box&, const box&);
box      operator|(const box&, const box&);
box	operator+(const box&,const box&);
box	operator-(const box&);
box	operator-(const box&,const box&);
box	operator*(const interval&,const box&);
box      operator*(const double,const box&);
box	operator*(const box&,const interval&);

std::ostream& operator<<(std::ostream&, const box&);
QDebug operator<<(QDebug, const box&);

//-------------------FONCTIONS NON MEMBRES---------------------------------------

double       Angle             (box& ,box&); // Il faut des vecteurs de dim 2
int          Size              (const box&);
int	    AxePrincipal      (const box&);
int	    AxePrincipal      (box&, std::vector<int>&);
int	    AxePrincipal      (box&, box&);
void         Update            (box&);
void	    Bisect	      (box&, box&, box&);
void	    Bisect	      (box&, box&, box&, std::vector<int>&);
void	    Decoup	      (box&, box&,box&);
void         Trisect           (box&, box&, box&, box&);
void	    Bisect	      (box&, box&,box&,box&);
void	    BisectAlong	      (box&, box&,box&,int);
void	    DecoupAlong	      (box&, box&,box&,int);
void         TrisectAlong      (box&,box&,box&,box&,int);
void         BisectHere        (box& ,box&,box&,int,double);
box          Rand              (const box& X);
box	    Center	      (const box&);
box          Center            (const box&, std::vector<int>&);
box          Concat            (const box&, const box&);
//friend void	    CheckRange	      (box&,box&);
interval     Determinant       (box&, box&);
bool	    Disjoint	      (const box&,const box&);
double       decrease          (const box&, const box&);
double       decrease          (const box&, const box&, std::vector<int>);
box          EmptyBox          (const box&);
box          EmptyBox          (int);
double       Eloignement       (box&,box&);
double       Eloignement2      (box&,box&);
double       EloignementRelatif2(box&,box&);
double       Marge             (box,box);
iboolean	    In		      (box,box);
box	    Inf 	      (box);
box          Infinity          (int);
box	    Inflate 	      (box&,double);
box	    Inter 	      (const box&,const box&);
box          Inter             (std::vector<box>&);
bool         IsBox             (box);
interval     Norm              (box);
interval     NormEuclid        (box, box);
interval     NormInf           (box, box);
interval     ProduitScalaire   (box&,box&);
box          Proj              (const box&, int, int);
bool         Prop              (box&,box&);
bool	    Subset	      (const box &, const box &);
bool         SubsetStrict      (box&,box&);
bool	    Subset	      (box&,box&,double);
void         Sucre             (box&,box&);
box	    Sup 	      (box);
box          Union             (std::vector<box>&);
box	    Union	      (const box&,const box&);
double       Volume            (box&);
double	    Width	      (const box&);
double	    Width	      (box&,std::vector<int>&);
double	    Width	      (box&,box&);
box          Zeros             (int);

//-------------------CONTRACTEURS---------------------------------------

void Cplus  (box&, box&, box&, int sens);
void Cmoins (box&, box&, box&, int sens);
void C_q_in (box&, int, std::vector<box>&);
void Cnorm(interval&R, box& X);
void Cdistance(interval& R, box& X, box& Y);
void CProdScalaire(interval& R, box& X, box& Y);
void COrtho(box& X, box& Y);
void CProd(box& Y, interval& a, box& X, int sens);

#endif
