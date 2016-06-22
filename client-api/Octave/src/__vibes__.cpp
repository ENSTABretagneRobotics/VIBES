// Copyright 2015-2016 Oliver Heimlich
//
// This file is part of VIBes' API for Octave.
//
// VIBes' API for Octave is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// VIBes' API for Octave is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with VIBes' API for Octave.
// If not, see <http://www.gnu.org/licenses/>.

#include <octave/oct.h>
#include <exception>

// Ideally, the functions in this file would be split into several oct-files:
//    +vibes/beginDrawing.oct
//    +vibes/endDrawing.oct
//    +vibes/...
//
// We would have to compile vibes.cpp as a shared library libvibes.so/.dll
// in order to get shared static variables in vibes.cpp working. Then,
// dynamically loading of the shared library would become difficult. To keep
// the loading mechanism simple, we use a single oct-file.
//
// It is not possible to define several external functions inside the oct-file,
// if they shall be called with the namespace mechanism (vibes.FUNCTION),
// because this is not supported by the autoload function. Thus, we create a
// single dispatcher function, which is called from several m-files:
//     +vibes/beginDrawing.m
//     +vibes/endDrawing.m
//     +vibes/...
//
// This also simplifies installation of the package, because src/+folders are
// not supported by the pkg function and we would have to work around this in
// the Makefile during package installation.
#include "vibes.h"
#include "vibes.cpp"

void __assertDrawing__ (
  const bool currentState = true,
  const bool newState = true);

void beginDrawing (const octave_value_list args)
{
  switch (args.length ())
    {
      case 0:
        __assertDrawing__ (false, true);
        vibes::beginDrawing ();
        break;
      case 1:
        if (args(0).is_string ())
          {
            __assertDrawing__ (false, true);
            vibes::beginDrawing (args(0).string_value ());
          }
        else
          error ("Connect expects a string as an argument");
        break;
      default:
        error ("beginDrawing: Wrong number of arguments");
        return;
    }
}

void endDrawing (const octave_value_list args)
{
  if (args.length () > 0)
    {
      error ("endDrawing: Wrong number of arguments");
      return;
    }
  __assertDrawing__ (true, false);
  vibes::endDrawing ();
}

// Internal function to make sure that the VIBes file handle is correctly
// initialized before using it.  Otherwise vibes.cpp will produce segfaults
// because of missing checks in the code.
static bool lastDrawingState = false;
void __assertDrawing__ (
  const bool currentState,
  const bool newState)
{
  // Did the user forget to call appropriate commands?
  if (currentState != lastDrawingState)
    if (lastDrawingState)
      {
        warning ("Please use vibes.endDrawing before vibes.beginDrawing");
        vibes::endDrawing ();
      }
    else
      {
        warning ("Please use vibes.beginDrawing before other VIBes commands");
        vibes::beginDrawing ();
      }
  
  lastDrawingState = newState;
}

void axisAuto (const octave_value_list args)
{
  if (args.length () > 0)
    {
      error ("axisAuto: Wrong number of arguments");
      return;
    }
  __assertDrawing__ ();
  vibes::axisAuto ();
}

void axisLabels (const octave_value_list args)
{
  if (args.length () == 0)
    {
      error ("axisLabels: Not enough arguments");
      return;
    }
  
  __assertDrawing__ ();
  std::vector<std::string> labels;
  std::string figureName;
  if (args(0).is_cellstr ())
    {
      // The first argument is a cellstr: use its contents as labels
      const Array<std::string> cellstr = args(0).cellstr_value ();
      const octave_idx_type n = cellstr.numel ();
      labels.reserve (n);
      for (octave_idx_type i = 0; i < n; i++)
        {
          labels.push_back(cellstr.elem(i));
        }
      
      if (args.length () >= 2)
        figureName = args(1).string_value ();
    }
  else
    {
      // Labels strings are directly provided as arguments
      const octave_idx_type n = args.length ();
      labels.reserve (n);
      for (octave_idx_type i = 0; i < n; i++)
        {
          labels.push_back(args(i).string_value ());
        }
    }
  vibes::axisLabels (labels, figureName);
}

void newFigure (const octave_value_list args)
{
  switch (args.length ())
    {
      case 0:
        __assertDrawing__ ();
        vibes::newFigure ();
        break;
      case 1:
        if (args(0).is_string ())
          {
            __assertDrawing__ ();
            vibes::newFigure (args(0).string_value ());
          }
        else
          error ("Figure expects a string as an argument");
        break;
      default:
        error ("newFigure: Wrong number of arguments");
        return;
    }
}

void drawBox (const octave_value_list args)
{
  if (args.length () == 0)
    {
      error ("drawBox: Not enough arguments");
      return;
    }
  
  __assertDrawing__ ();
  if (args(0).is_string ())
    {
      error ("drawBox: First argument must contain numeric boundaries");
      return;
    }
  
  std::vector<double> coordinates;
  octave_idx_type options_idx;
  if ((args(0).rows () == 2 || args(0).columns () == 2) &&
      (args.length () == 1 || args(1).is_string ()))
    {
      // Single box, coordinates given in columns or rows
      Matrix box = args(0).matrix_value ();
      // Give precedence to coordinates in rows
      if (box.columns () == 2)
        box = box.transpose ();
        
      const octave_idx_type n = box.numel ();
      coordinates.reserve (n);
      // Add elements in the order x_lb, x_ub, y_lb, y_ub, z_lb, z_ub, ...
      for (octave_idx_type i = 0; i < n; i ++)
        {
          coordinates.push_back (box.elem (i));
        }
      options_idx = 1;
    }
  else
    {
      // Single boxes, boundaries given as seperate parameters
      for (options_idx = 0; options_idx < args.length () &&
                            ! args(options_idx).is_string (); options_idx ++)
        {
          coordinates.push_back (args(options_idx).scalar_value ());
        }
    }
  
  vibes::Params params;
  std::string format;
  for (; options_idx < args.length (); options_idx ++)
    {
      if (! args(options_idx).is_string ())
        {
          error ("drawBox: Parameter expected to be a string");
          return;
        }
      if (args(options_idx).string_value () == "figure")
        {
          options_idx ++;
          if (options_idx < args.length () &&
              args(options_idx).is_string ())
            {
              std::string figure = args(options_idx).string_value ();
              params["figure"] = figure;
            }
          else
            {
              error ("drawBox: Missing argument after 'figure'");
              return;
            }
        }
      else
        format += args(options_idx).string_value ();
    }
  
  vibes::drawBox(coordinates, format, params);
}

void drawBoxes (const octave_value_list args)
{
  if (args.length () == 0)
    {
      error ("drawBoxes: Not enough arguments");
      return;
    }
  
  __assertDrawing__ ();
  if (args(0).is_string ())
    {
      error ("drawBoxes: First argument must contain numeric boundaries");
      return;
    }

  const Matrix lb = args(0).matrix_value ();
  const Matrix ub = args(1).matrix_value ();
  
  std::vector< std::vector<double> > boxes;
  const octave_idx_type n = lb.columns (); // TODO broadcasting
  const octave_idx_type m = lb.rows ();
  boxes.reserve (n);
  for (octave_idx_type i = 0; i < n; i ++)
    {
      std::vector<double> coordinates;
      coordinates.reserve (2 * m);
      for (octave_idx_type j = 0; j < m; j ++)
        {
          coordinates.push_back (lb.elem (j, i));
          coordinates.push_back (ub.elem (j, i));
        }
      boxes.push_back (coordinates);
    }
  
  vibes::drawBoxes(boxes);
}

void drawLine (const octave_value_list args)
{
  if (args.length () == 0)
    {
      error ("drawLine: Not enough arguments");
      return;
    }
  
  __assertDrawing__ ();
  if (args(0).is_string ())
    {
      error ("drawLine: First argument must contain numeric boundaries");
      return;
    }
  
  std::vector< std::vector<double> > coordinates;
  std::vector<double> point;
  const Matrix line = args(0).matrix_value ();
  const octave_idx_type n = line.rows ();
  const octave_idx_type m = line.columns ();
  coordinates.reserve (n);
  point.reserve (m);
  for (octave_idx_type i = 0; i < n; i ++)
    {
      point.clear ();
      for (octave_idx_type coordinate = 0; coordinate < m; coordinate ++)
        {
          point.push_back (line.elem (i, coordinate));
        }
      coordinates.push_back (point);
    }
  
  vibes::Params params;
  std::string format;
  for (octave_idx_type arg = 1; arg < args.length (); arg ++)
    {
      if (! args(arg).is_string ())
        {
          error ("drawLine: Parameter expected to be a string");
          return;
        }
      if (args(arg).string_value () == "figure")
        {
          arg ++;
          if (arg < args.length () &&
              args(arg).is_string ())
            {
              std::string figure = args(arg).string_value ();
              params["figure"] = figure;
            }
          else
            {
              error ("drawLine: Missing argument after 'figure'");
              return;
            }
        }
      else
        format += args(arg).string_value ();
    }
  
  vibes::drawLine(coordinates, format, params);
}

DEFUN_DLD (__vibes__, args, nargout,
  "-*- texinfo -*-\n"
  "@deftypefn  {Loadable Function} {} __vibes__ (@var{function}, @dots{})\n"
  "This internal function is a wrapper around VIBes' client API and "
  "will be called by the public functions in the @code{+vibes} package.\n"
  "\n"
  "Please use @code{help vibes.@var{function}} for the documentation.\n"
  "@end deftypefn"
  )
{
  const int nargin = args.length ();
  if (nargin < 1 || ! args(0).is_string ())
    {
      print_usage ();
      return octave_value_list ();
    }
  
  // This function is just a dispatcher for the other functions
  const std::string f_name = args(0).string_value ();
  const octave_value_list f_args = args.slice (1, args.length () - 1);
  try
    {
      if      (f_name == "drawBox")
        drawBox (f_args);
      else if (f_name == "drawBoxes")
        drawBoxes (f_args);
      else if (f_name == "drawLine")
        drawLine (f_args);
      else if (f_name == "axisAuto")
        axisAuto (f_args);
      else if (f_name == "axisLabels")
        axisLabels (f_args);
      else if (f_name == "beginDrawing")
        beginDrawing (f_args);
      else if (f_name == "endDrawing")
        endDrawing (f_args);
      else if (f_name == "newFigure")
        newFigure (f_args);
      else
        error ("This particular VIBes function is not supported");
    }
  catch (std::exception& e)
    {
      error (e.what ());
    }
  catch (...)
    {
      error ("Exception occurred inside vibes client api");
    }

  return octave_value_list ();
}
