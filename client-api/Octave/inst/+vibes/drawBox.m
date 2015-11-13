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
## @deftypefn  {Function File} {} vibes.drawBox (@var{box})
## Plot a single box with pairs of lower bounds and upper bounds.
##
## The matrix @var{box} must be of size either @option{[2 @var{n}]} or
## @option{[@var{n} 2]}, where @var{n} is the number of axes.
##
## Alternatively @var{box} may be an interval vector.
## @end deftypefn
##
## @deftypefn  {Function File} {} vibes.drawBox (@var{x_lb}, @var{x_ub}, @var{y_lb}, @var{y_ub})
## @deftypefnx {Function File} {} vibes.drawBox (@var{x_lb}, @var{x_ub}, @var{y_lb}, @var{y_ub}, @var{z_lb}, @var{z_ub})
## Plot a single box.
## @end deftypefn
##
## @deftypefn  {Function File} {} vibes.drawBox (@dots{}, 'figure', @var{name})
## Do the plotting inside a given VIBes figure
## @end deftypefn
##
## @deftypefn  {Function File} {} vibes.drawBox (@dots{}, @var{format})
## Customize the plotting using VIBes format strings.
## @end deftypefn
function drawBox (varargin)

if (nargin == 0)
    print_usage ();
endif

if (any (isa (varargin{1}, {'infsup', 'intval'})))
    varargin{1} = horzcat (vec (inf (varargin{1})), ...
                           vec (sup (varargin{1})));
    ## Stack multiple intervals into a common box
    for i = 2 : nargin
        if (any (isa (varargin{2}, {'infsup', 'intval'})))
            varargin = {vertcat(varargin{1}, ...
                                horzcat (vec (inf (varargin{2})), ...
                                         vec (sup (varargin{2})))), ...
                        varargin{3:end}};
        else
            break
        endif
    endfor
endif

__vibes__ ('drawBox', varargin{:});

endfunction

%!shared x,y,z
%! vibes.beginDrawing
%! x = [ 0  2 ; ...
%!       7 22 ];
%! y = [ 1 2 3 4 ; ...
%!       2 3 4 5 ];
%! z = [ 1 2 ; ...
%!       3 4 ; ...
%!       5 6 ; ...
%!       7 8 ];
%!test vibes.drawBox (x)
%!test vibes.drawBox (y)
%!test vibes.drawBox (z)
%!test vibes.drawBox (-1, 0, 1, 2)
%!test vibes.drawBox (-1, 0, 1, 2, 4, 5)
%!shared
%! vibes.endDrawing

%!shared ix,iy,iz,dx,dy,dz,rx,ry,rz
%! pkg load interval
%! vibes.beginDrawing
%! x = [ 0  2 ; ...
%!       7 22 ];
%! y = [ 1 2 3 4 ; ...
%!       2 3 4 5 ];
%! z = [ 1 2 ; ...
%!       3 4 ; ...
%!       5 6 ; ...
%!       7 8 ];
%! ix = infsup (x)
%! iy = infsup (y)
%! iz = infsup (z)
%! dx = infsupdec (x)
%! dy = infsupdec (y)
%! dz = infsupdec (z)
%! rx = infsup (x, x + 100)
%! ry = infsup (y, y + 100)
%! rz = infsup (z, z + 100)
%!test vibes.drawBox (ix)
%!test vibes.drawBox (iy)
%!test vibes.drawBox (iz)
%!test vibes.drawBox (dx)
%!test vibes.drawBox (dy)
%!test vibes.drawBox (dz)
%!test vibes.drawBox (rx)
%!test vibes.drawBox (ry)
%!test vibes.drawBox (rz)
%!shared
%! vibes.endDrawing
