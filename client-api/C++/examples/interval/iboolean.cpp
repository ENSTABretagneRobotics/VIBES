#include "iboolean.h"

#include <iostream>
//#include <QDebug>

iboolean::iboolean ()
{value = iperhaps;}

iboolean::iboolean (bool b)
{if (b) value=itrue; else value=ifalse;}

iboolean::iboolean (IBOOLEAN tt) {value = tt;}

iboolean::iboolean (const iboolean& t)
{*this = t;}

iboolean operator&& (iboolean x, iboolean y)                          // AND
{    return And(x,y);  }

iboolean operator|| (iboolean x, iboolean y)                        // OR
{    return Or(x,y); }

bool operator== (iboolean x, iboolean y)
{	return (x.value==y.value);}

bool operator!= (iboolean x, iboolean y)
{	return (x.value!=y.value);}


iboolean operator! (iboolean x)
{	if (x.value == itrue) return iboolean(ifalse);
    if (x.value == ifalse) return iboolean(itrue);
    return x;
}


iboolean Not(iboolean x)
{	if (x.value == iperhaps)   return iperhaps;
    if (x.value == itrue)      return ifalse;
    if (x.value == ifalse)     return itrue;
    return empty;
}


iboolean Inter(iboolean x, iboolean y)
{	if (x.value == y.value)   return x.value;
    if (x.value == iperhaps)   return y.value;
    if (y.value == iperhaps)   return x.value;
    return iboolean(empty);
}


iboolean Union(iboolean x, iboolean y)
{	if (x.value == empty)   return y.value;
    if (y.value == empty)   return x.value;
    if (x.value == y.value)   return x.value;
    return iboolean(iperhaps);
}

iboolean Xor(iboolean x, iboolean y)
{	if (x.value == empty)   return empty;
    if (y.value == empty)   return empty;
    if (x.value == iperhaps)   return iperhaps;
    if (y.value == iperhaps)   return iperhaps;
    if (x.value == y.value)   return ifalse;
    return iboolean(itrue);
}



iboolean And(iboolean x, iboolean y)
{	if ((x.value==empty)||(y.value==empty))   return iboolean(empty);
    if ((x.value==ifalse)||(y.value==ifalse)) return iboolean(ifalse);
    if ((x.value==iperhaps)||(y.value==iperhaps)) return iboolean(iperhaps);
    return iboolean(itrue);
}


iboolean geq(iboolean x, iboolean y)      // Project the constraint x>=y with respect to x
{	iboolean r;
        r=iboolean(iperhaps);
        if (y.value==empty)  r=iboolean(empty);
    if (y.value==itrue) r=iboolean(itrue);
        return Inter(x,r);
}



iboolean leq(iboolean x, iboolean y)      // Project the constraint x<=y with respect to x
{	iboolean r;
        r=iboolean(iperhaps);
        if (y.value==empty)  r=iboolean(empty);
    if (y.value==ifalse) r=iboolean(ifalse);
        return Inter(x,r);
/*                               a     &&    (implique b)
1*0=\empty                       1     &&      0
1*1=1                            1     &&     [0,1]
1*[0,1]=1                        1     &&     [0,1]
0*0=0                            0     &&      0
0*1=0                            0     &&      [0,1]
0*[0,1]=0                        0     &&      [0,1]
[0,1]*0=0                        [0,1] &&       0
[0,1]*1=[0,1]                    [0,1] &&      [0,1]
[0,1]*[0,1]=[0,1]                [0,1] &&     [0,1]
*/
}

iboolean Restrict(iboolean x, iboolean y)      // return x and !y
{ return And(x,!y);}





iboolean Or(iboolean x, iboolean y)
{	if ((x.value==empty)||(y.value==empty))   return iboolean(empty);
    if ( (x.value==itrue) || (y.value==itrue) ) return iboolean(itrue);
    if ( (x.value==iperhaps) || (y.value==iperhaps) ) return iboolean(iperhaps);
    return iboolean(ifalse);
}


//------------------------------------------------------------------------------
std::ostream& operator<< (std::ostream& os, const iboolean& a)
{
if      (a.value==itrue)    os<<"   itrue";
if      (a.value==ifalse)   os<<"  ifalse";
if      (a.value==iperhaps) os<<"iperhaps";
return os;
}

//QDebug operator<< (QDebug os, const iboolean&a)
//{
//    if      (a.value==itrue)    os<<"   itrue";
//    if      (a.value==ifalse)   os<<"  ifalse";
//    if      (a.value==iperhaps) os<<"iperhaps";
//    return os;
//}



