## Copyright 2015-2016 Oliver Heimlich
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
## @deftypefn  {Function File} {} vibes.axisLabels (@var{label1}, @var{label2}, @dots{})
## @deftypefnx  {Function File} {} vibes.axisLabels (@{@var{label1}, @var{label2}, @dots{}@})
## Set axis labels for the current VIBes figure.
## @end deftypefn
##
## @deftypefn  {Function File} {} vibes.axisLabels (@{@var{label1}, @var{label2}, @dots{}@}, @var{figure})
## Set axis labels for given VIBes figure.
## @end deftypefn
function axisLabels (varargin)

if (nargin == 0)
    print_usage ();
endif

__vibes__ ('axisLabels', varargin{:});

endfunction

%!shared
%! vibes.beginDrawing
%!test vibes.axisLabels ('x', 'y', 'z')
%!test vibes.axisLabels ({'x', 'y', 'z'})
%!test vibes.axisLabels ({'a', 'b', 'c'}, 'alphabet')
%!shared
%! vibes.endDrawing
