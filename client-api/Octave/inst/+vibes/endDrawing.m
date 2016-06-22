## Copyright 2015-2016 Oliver Heimlich
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, see <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @documentencoding UTF-8
## @deftypefn  {Function File} {} vibes.endDrawing ()
## Disconnect from the VIBes viewer.  This function should be called after any
## other VIBes functions to close the session.
##
## @example
## @group
## vibes.beginDrawing
## vibes.drawBox (0, 1, 0, 1)
## vibes.endDrawing
## @end group
## @end example
## @end deftypefn
function endDrawing ()

if (nargin != 0)
    print_usage ();
endif

__vibes__ ('endDrawing');

endfunction

%!test
%! vibes.beginDrawing
%! vibes.endDrawing
%!warning vibes.endDrawing
