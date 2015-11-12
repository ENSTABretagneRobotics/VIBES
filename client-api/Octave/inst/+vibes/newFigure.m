## Copyright 2015 Oliver Heimlich
##
## This file is part of VIBes' API for Octave.
##
## VIBes' API for Octave is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## VIBes' API for Octave is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with VIBes' API for Octave.
## If not, see <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @documentencoding UTF-8
## @deftypefn  {Function File} {} vibes.newFigure ()
## @deftypefnx {Function File} {} vibes.newFigure (@var{name})
## Create a new figure named @var{name}.
## @end deftypefn
function newFigure (name)

switch (nargin)
    case 0
        __vibes__ ('newFigure');
    case 1
        __vibes__ ('newFigure', name);
    otherwise
        print_usage ();
endswitch

endfunction
