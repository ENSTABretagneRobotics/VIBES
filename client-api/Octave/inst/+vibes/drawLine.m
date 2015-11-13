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
## @deftypefn  {Function File} {} vibes.drawLine (@var{points})
## Plot a line with coordinate points given as rows of a matrix.
## @end deftypefn
##
## @deftypefn  {Function File} {} vibes.drawLine (@dots{}, 'figure', @var{name})
## Do the plotting inside a given VIBes figure
## @end deftypefn
##
## @deftypefn   {Function File} {} vibes.drawLine (@dots{}, @var{format})
## Customize the plotting using VIBes format strings.
## @end deftypefn
function drawLine (varargin)

if (nargin == 0)
    print_usage ();
endif

__vibes__ ('drawLine', varargin{:});

endfunction

%!shared x
%! vibes.beginDrawing
%! x = [ 0 0 ; ...
%!       1 1 ; ...
%!       2 2 ];
%!test vibes.drawLine (x)
%!test vibes.drawLine (x, 'figure', 'foo')
%!test vibes.drawLine (x, 'red')
%!test vibes.drawLine (x, 'figure', 'bar', 'blue')
%!test vibes.drawLine (x, 'green', 'figure', 'baz')
%!shared
%! vibes.endDrawing
