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
