# vibes.py

import json
import os
import math
import sys
import subprocess


class vibes(object):
    channel = None
    current_fig = 'default'

    @classmethod
    def _write(cls, msg, figure=None, **kwargs):
        if(cls.channel is None):
            cls.beginDrawing()
        else:
            #print(kwargs.pop('figure', cls.current_fig))
            msg['figure'] = kwargs.pop('figure', cls.current_fig)
            # print(msg)
            if 'shape' in msg:
                # print("found shape", kwargs)
                for k, v in kwargs.items():
                    # print(k,v)
                    msg['shape'][k] = v
            msg = json.dumps(msg)
            cls.channel.write(msg + '\n\n')
            cls.channel.flush()
        # print(msg)

    ##########################################################################
    ##				Management of connection to the Vibes server			##
    ##########################################################################

    @classmethod
    def beginDrawing(cls, runVibesInstance=True):

        vibes_json = 'vibes.json'
        # Retrieve user-profile directory from envirnment variable
        user_dir = os.getenv("USERPROFILE")  # Windows
        if not user_dir:
            user_dir = os.getenv("HOME")
            # POSIX
        if user_dir:
            # Environment variable found, connect to a file in user's profile
            # directory
            vibes_json = user_dir + '/.vibes.json'

        try:
            cls.channel = None
            cls.channel = open(vibes_json, 'a')
        except IOError:
            print('cannot open', vibes_json)
            sys.exit(-1)

    @classmethod
    def endDrawing(cls):
        if not (cls.channel is None):
            cls.channel.close()
            cls.channel = None

    @classmethod
    def update(cls):
        if not (cls.channel is None):
            cls.channel.flush()

    ##########################################################################
    ##							Figure Management							##
    ##########################################################################

    @classmethod
    def newFigure(cls, figure=''):
        """Create a new figure named figureName.

        Args:
            figure (string): name of the new figure

        """
        if not figure == '':
            cls.current_fig = figure
        msg ={'action': 'new'}
        cls._write(msg, figure=figure)

    @classmethod
    def setFigureProperties(cls, properties, **kwargs):
        """Set the property key to the provided value for current figure.
        """
        msg = {'action': 'set',
               'properties': properties
              }
        cls._write(msg, **kwargs)

    @classmethod
    def clearFigure(cls, **kwargs):
        """Clears the contents of the figure figureName, or the current figure if argument is given.

        """
        cls._write({'action': 'clear'} , **kwargs)

    @classmethod
    def closeFigure(cls, **kwargs):
        """Close the figure named figureName, or the current figure if no argument is given.
        """
        cls._write({'action': 'close'}, **kwargs)

    @classmethod
    def saveImage(cls, fileName, **kwargs):
        """Save the figure *figureName*, or the current figure if no argument is given as an image.
        Args:
            filName (string): name of the output image
        """
        msg = {'action': 'export',
               'file': '%s' % fileName
              }
        cls._write(msg, **kwargs)

    @classmethod
    def selectFigure(cls, figure=''):
        """Select figureName as the current figure. Drawing operations will then apply to figureName.

        """
        cls.current_fig = figure

    @classmethod
    def setFigureSize(cls, width, height, figure=''):
        """Set the size of the window

        """
        cls.setFigureProperties({"width": width, "height":height})

    @classmethod
    def setFigurePos(cls, x, y, figure=''):
        """Set the position of the window

        """
        cls.setFigureProperties({"x": x, "y":y})
    @classmethod
    def showAxis(cls, is_visible):
      """Display axis if is_visible is True

      """
      cls.setFigureProperties({"showAxis":is_visible})

    ##########################################################################
    ##							View settings								##
    ##########################################################################

    @classmethod
    def axisAuto(cls, **kwargs):
        """Set axes limits to the bounding box of the drawing.
        """
        cls.setFigureProperties({"viewbox": "auto"}, **kwargs)

    @classmethod
    def axisEqual(cls, **kwargs):
        """Same as axisAuto but with the same ratio

        """
        cls.setFigureProperties({"viewbox": "equal"}, **kwargs)

    @classmethod
    def axisLimits(cls, x_lb, x_ub, y_lb, y_ub, **kwargs):
        """Specify the rectangle to be displayed

        Args:
            x_lb,y_lb (float) : lower-left corner
            x_ub,y_ub (float) : upper-right corner

        Example :
            >>> axisLimits(x_lb, x_ub, y_lb, y_ub, figure='')

        """
        cls.setFigureProperties(
            {'viewbox': [x_lb, x_ub, y_lb, y_ub]}, **kwargs)

    @classmethod
    def axisLabels(cls, x_label, y_label, **kwargs):
        """Set axis Labels
        example:
            >>> axisLabels( x_label, y_label, figur='')

        """
        cls.setFigureProperties({'axislabels': [x_label, y_label]}, **kwargs)

    ##########################################################################
    ##                          Group Management                            ##
    ##########################################################################
    @classmethod
    def newGroup(cls, groupName, **kwargs):
        """Create a new group with the specified name.

        """
        msg = {'action': 'draw',
               'shape': {'type': 'group',
                         'name': '%s' % groupName,
                         }
              }
        if 'format' in kwargs:
          msg['shape']['format'] = kwargs.pop('format')

        cls._write(msg,  **kwargs)

    @classmethod
    def clearGroup(cls, groupName, **kwargs):
        """Clear the contents of the group groupName in figure figureName.

        """
        msg = {'action': 'clear',
               'group': '%s' % groupName
              }
        cls._write(msg, **kwargs)

    ##########################################################################
    ##                         Object Management                            ##
    ##########################################################################
    @classmethod
    def removeObject(cls, objectName, **kwargs):
        """Delete the named graphical object objectName from figure figureName.

        """
        msg = {'action': 'delete',
               'figure': '%s' % (figure if figure != None else cls.current_fig),
               'object': '%s' % objectName
              }
        cls._write(msg, **kwargs)

    @classmethod
    def setObjectProperties(cls, objectName, properties, **kwargs):
        """	Set the property key to the provided value for the named object objectName in figure figureName.

        """
        msg = {'action': 'set',
               'figure': '%s' % (figure if figure != None else cls.current_fig),
               'object': '%s' % objectName,
               'properties' : properties
              }
        cls._write(msg, **kwargs)

    ##########################################################################
    ##							Drawing functions							##
    ##########################################################################
    @classmethod
    def drawBox(cls, x_lb, x_ub, y_lb, y_ub, color='r', **kwargs):
        """Draw a box defined by its corners in figure named <figure>

        Args:
            x_lb (double): x lower bound
            x_ub (double): x upper bound
            y_lb (double): y lower bound
            y_ub (double): y upper bound
            figure (string): optional, name of the figure where the box will be drawn
        """
        msg = {'action': 'draw',
               'shape': {'type': 'box',
                        'bounds': [x_lb, x_ub, y_lb, y_ub],
                        'format': color
                        }
               }
        cls._write(msg,**kwargs)

    @classmethod
    def drawBoxesUnion(cls, boxes_bounds, color='r', **kwargs):
        """Computes and draw the union of a list of N-D rectangles, from a
        list of list of bounds

        Args:
            box_bounds (list<list<float>>): list of list of bounds in the form ((x_lb_1, x_ub_1, y_lb_1, ...), (x_lb_2, x_ub_2, y_lb_2, ...), ...)

        """
        msg = {'action': 'draw',
               'shape': {'type': 'boxes union',
                         'bounds': boxes_bounds,
                         'format': color
                         }
               }
        cls._write(msg, **kwargs)

    @classmethod
    def drawBoxDiff(cls, X0, X, color='r', **kwargs):
        msg = {'action': 'draw',
               'shape': {'type': 'boxes diff',
                         'bounds': [X0, X],
                         'format': color
                        }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawLine(cls, points, color='r', **kwargs):
        """Draw a N-D line from the list of coordinates

        Args:
            points (list<list<float>>): [[x_1, y_1, z_1, ...], [x_2, y_2, z_2, ...], ...]
        """
        msg = {'action': 'draw',
               'shape': {'type': 'line',
                         'points': points,
                         'format': color
                        }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawEllipse(cls, cx, cy, a, b, rot, angles=[0, 360], color='r', **kwargs):
        """Draw an ellipse centered at (cx, cy), with semi-major and minor axes a and b,
        and rotated by rot (degrees)

        Partial ellipse can be drawn using the angles argument.

        Args:

            cx,cy (double): center of the ellipse
            a,b (double): semi-major and minor axes
            rot (double): direction of the semi-major axis !!! need to be in degree !!!
            angles (double, double): starting and ending angle of the Ellispe
        Example:
            >>> # full ellipse
            >>> vibes.drawEllipse(0,0, 1,2,45)
            >>> # half ellipse
            >>> vibes.drawEllipse(0,0, 1,2,45, angles=[0,180])

        """
        if len(angles) != 2:
          print("Error: length of angle must be 2")
          return False
        msg = {'action': 'draw',
               'shape': {'type': 'ellipse',
                         'center': [cx, cy],
                         'axis': [a, b],
                         'orientation': rot,
                         'angles': angles,
                         'format': color,
                         }
               }
        cls._write(msg, **kwargs)

    @classmethod
    def drawCircle(cls, cx, cy, R, color='r', **kwargs):
        """Draw a circle centered at (cx, cy), with radius r

        Args:
            cx,cy (double): center of the circle
            r (double): raduis

        """
        cls.drawEllipse(cx, cy, R, R, 0, color=color, **kwargs)

    @classmethod
    def drawVehicle(cls, cx, cy, heading, length, color='r', **kwargs):
        """Draw a vehicle centered at (cx, cy) with heading <heading> and size length

        Args:
            cx,cy (double): position of the vehicle
            heading (double): heading of the vehicle in degree
            lenght (double): length of the vehicle

        """
        msg = {'action': 'draw',
               'shape': {'type': 'vehicle',
                         'center': [cx, cy],
                         'length': length,
                         'orientation': heading,
                         'format': color
                        }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawAUV(cls, cx, cy, oritentation, length, color='r', **kwargs):
        """Draw a AUV centered at (cx, cy) with heading <heading> and size length

        Args:
            cx,cy  (double): position of the AUV
            heading  (double): heading of the vehicle in degree
            lenght (double): length of the vehicle

        """
        msg = {'action': 'draw',
               'shape': {'type': 'vehicle_auv',
                         'center': [cx, cy],
                         'length': length,
                         'orientation': oritentation,
                         'format': color
                         }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawMotorBoat(cls, cx, cy, oritentation, length, color='r', **kwargs):
        """Draw a motor boat centered at (cx, cy) with heading <heading> and size length

        Args:
            cx,cy  (double): position of the vehicle
            heading  (double): heading of the vehicle in degree
            lenght (double): length of the vehicle

        """
        msg = {'action': 'draw',
               'shape': {'type': 'vehicle_motor_boat',
                         'center': [cx, cy],
                         'length': length,
                         'orientation': oritentation,
                         'format': color
                         }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawTank(cls, cx, cy, oritentation, length, color='r', **kwargs):
        """Draw a tank centered at (cx, cy) with heading <heading> and size length

        Args:
            cx,cy  (double): position of the tank
            heading  (double): heading of the vehicle in degree
            lenght (double): length of the vehicle

        """
        msg = {'action': 'draw',
               'shape': {'type': 'vehicle_tank',
                         'center': [cx, cy],
                         'length': length,
                         'orientation': oritentation,
                         'format': color
                         }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawPie(cls, center, rho, theta, color='r', use_radian=False, **kwargs):
        """Draw a Pie centered at <center> with raduis in <rho> and angle in <theta>

        Args:
            center (double, double): center of the Pie
            rho  (double, double): minimal and maximal radius
            theta (double, double) : minimal and maximal angle (in degree)
            use_radian boolean: if True theta is in radian

        """
        msg = {'action': 'draw',
               'shape': {'type': 'pie',
                         'center': [center[0], center[1]],
                         'rho': [rho[0], rho[1]],
                         'theta': [theta[0], theta[1]] if use_radian == False else [math.degrees(theta[0]), math.degrees(theta[1])],
                         'format': color,
                        }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawArrow(cls, A, B, tip_length, color='r', **kwargs):
        """Draw an arrow between starting at point A and ending at point B

        Args:
            A (double, double): starting point (ax, ay)
            B (double, double): ending point (bx, by)
            tip_length (double): length of the tip

        """
        msg = {'action': 'draw',
               'shape': {'type': 'arrow',
                         'points': [A, B],
                         'tip_length': tip_length,
                         'format': color
                         }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawPolygon(cls, points, color='r', **kwargs):
        """Draw a Polygon defined by <points>

        Args:
            points (list of lists): list of points. ex [ [p1x, p1y] ,  [p2x, p2y], ..., [pnx, pny]]

    	  """
        msg = {'action': 'draw',
               'shape': {'type': 'polygon',
                         'bounds': points,
                         'format': color
                         }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawPoint(cls, cx, cy, radius=3, color='r', **kwargs):
        """Draw a Point at position (cy, cy)

        Args:
          cx,cy (double): location of the point

        """
        msg = {'action': 'draw',
                'shape': {'type': 'point',
                          'point': [cx, cy],
                          'Radius' : radius,
                          'format': color
                          }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawPoints(cls,cx,cy, color='r', **kwargs):
        """Draw a set of points at position (cx, cy)

        Args:
            cx,cy (list of double): location of the points

        """
        points = []
        for i in range(len(cx)):
            points.append([cx[i], cy[i]])
        msg = {'action': 'draw',
                'shape': {'type': 'points',
                          'centers': points,
                          'format': color
                          }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawRing(cls, cx, cy, r_min, r_max, color='r', **kwargs):
        """Draw a ring at position (cx, cy) with radius between (r_min, r_max)

        Args:
            cx,cy (double): center if the ring
            r_min, r_max (double): bound of the radius in degree

        """
        msg = {'action': 'draw',
                'shape': {'type': 'ring',
                          'center': [cx, cy],
                          'rho': [r_min, r_max],
                          'format': color
                          }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawText(cls, cx, cy, text, scale, color='r', **kwargs):
        """Draw a text <text> at position (cx, cy)

        Args:
            cx,cy (double): position of the text
            text (string): text to draw

        Note:
            Experimental feature

        """
        msg = {'action': 'draw',
                'shape': {'type': 'text',
                          'text' : text,
                          'position': [cx, cy],
                          'scale' : scale,
                          'format': color
                          }
              }
        cls._write(msg, **kwargs)

    @classmethod
    def drawRaster(cls, filename, xlb, yub, width, height, rot=0, **kwargs):
      """Draw raster from <filename> on the screen with upper left corner at <xlb, yub>
         with pixels size of <xres, yres>.

        Args:
            filename (string): image filename of the input data
            xlb, yub (double): coordinate of the upper left pixel
            width, height (double): raster projected size
            rot (double): optional rotation of the raster in degrees
            color (optional, string): color to used for transparency
      """
      msg = {'action': 'draw',
              'shape': {'type': 'raster',
                        'filename' : filename,
                        'ul_corner':  [xlb, yub],
                        'size' : [width, height],
                        'rot' : rot
                        }
            }
      if "color" in kwargs:
        msg["shape"]["format"] = kwargs['color']
      cls._write(msg, **kwargs)

    @classmethod
    def drawCake(cls, cx, cy, length, color='r', **kwargs):
        """Draw a cake centered at (cx, cy) with heading <heading> and size length

        Args:
            cx,cy  (double): position of the Cake
            heading  (double): heading of the vehicle in degree
            lenght (double): length of the vehicle

        """
        msg = {'action': 'draw',
               'shape': {'type': 'cake',
                         'center': [cx, cy],
                         'length': length,
                         'orientation': oritentation,
                         'format': color
                         }
              }
        cls._write(msg, **kwargs)
