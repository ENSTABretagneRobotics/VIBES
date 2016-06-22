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
## @deftypefn  {Function File} {} vibes.beginDrawing ()
## @deftypefnx {Function File} {} vibes.beginDrawing (@var{filename})
## Connect to the VIBes viewer to send data for plotting.  This
## function must be called before using any other VIBes functions.
##
## Connection to VIBes is established via a file.  Plotting commands for VIBes
## will be sent to the file and, asynchronously, interpreted by the VIBes
## viewer.  Make sure that the VIBes viewer is started to see results of the
## plotting.
##
## @example
## @group
## vibes.beginDrawing
## vibes.drawBox (0, 1, 0, 1)
## vibes.endDrawing
## @end group
## @end example
##
## The target file is @code{.vibes.json} inside the user's home directory by
## default and may be changed with the optional @var{filename} parameter.
##
## @example
## @group
## vibes.beginDrawing simulation1.json
## vibes.drawBox (0, 1, 0, 1)
## vibes.endDrawing
## @end group
## @end example
## @seealso{vibes.endDrawing}
## @end deftypefn
function beginDrawing (filename)

switch (nargin)
    case 0
        __vibes__ ('beginDrawing');
    case 1
        __vibes__ ('beginDrawing', filename);
    otherwise
        print_usage ();
endswitch

endfunction

%!test
%! vibes.beginDrawing
%! vibes.endDrawing
%!warning vibes.beginDrawing; vibes.beginDrawing; vibes.endDrawing
%!test
%! vibes.beginDrawing ('.vibes-custom.json')
%! assert (exist ('.vibes-custom.json', 'file'))
%! vibes.endDrawing
%! delete ('.vibes-custom.json')
