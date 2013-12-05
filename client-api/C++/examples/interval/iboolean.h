#ifndef __IBOOLEAN__
#define __IBOOLEAN__

#include <iosfwd>
class QDebug;

// IBOOLEAN is an interval Boolean. Used for a trivalued logic
// if x=[0,0]=ifalse  : certainly false
// if x=[0,1]=iperhaps: don't know
// if x=[1,1]=itrue  : certainly true
// Otherwise x=empty
enum IBOOLEAN {itrue, ifalse, iperhaps, empty};

class iboolean
{
public:
    IBOOLEAN value;
public:
    iboolean ();
    iboolean (bool);
    iboolean (IBOOLEAN);
    iboolean (const iboolean&);
};

iboolean operator&& (iboolean, iboolean);
iboolean operator|| (iboolean, iboolean);
iboolean operator!  (iboolean);
bool		 operator== (iboolean, iboolean);
bool		 operator!= (iboolean, iboolean);

std::ostream& operator<< (std::ostream& os, const iboolean&);
QDebug operator<< (QDebug, const iboolean&);

iboolean Inter(iboolean,iboolean);
iboolean Not(iboolean x);
iboolean Union(iboolean, iboolean);
iboolean And(iboolean,iboolean);
iboolean Or(iboolean,iboolean);
iboolean leq(iboolean x, iboolean y);
iboolean geq(iboolean x, iboolean y);
iboolean Restrict(iboolean x, iboolean y);      // return x and !y
iboolean Xor(iboolean x, iboolean y);

#endif
