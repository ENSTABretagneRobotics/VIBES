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
## @deftypefn  {Function File} {} vibes.drawBoxes (@var{lb}, @var{ub})
## @deftypefnx {Function File} {} vibes.drawBoxes (@var{x_lb}, @var{x_ub}, @var{y_lb}, @var{y_ub})
## @deftypefnx {Function File} {} vibes.drawBoxes (@var{x_lb}, @var{x_ub}, @var{y_lb}, @var{y_ub}, @var{z_lb}, @var{z_ub})
## Plot several boxes with pairs of lower bounds and upper bounds.
##
## The matrices @var{lb} and @var{ub} must be of compatible size
## @var{n}×@var{m}, where @var{n} is the number of dimensions of the boxes and
## @var{m} is the number of boxes to plot.
## @end deftypefn
##
## @deftypefn  {Function File} {} vibes.drawBox (@var{boxes})
## When @var{boxes} is an interval matrix, each column represents a box.
## @end deftypefn
##
## @deftypefn  {Function File} {} vibes.drawBox (@dots{}, 'figure', @var{name})
## Do the plotting inside a given VIBes figure
## @end deftypefn
##
## @deftypefn  {Function File} {} vibes.drawBox (@dots{}, @var{format})
## Customize the plotting using VIBes format strings.
## @end deftypefn
function drawBoxes (varargin)

if (nargin == 0)
    print_usage ();
endif

if (any (isa (varargin{1}, {'infsup', 'intval'})))
    varargin = {inf(varargin{1}), sup(varargin{1})};
endif

__vibes__ ('drawBoxes', varargin{:});

endfunction
