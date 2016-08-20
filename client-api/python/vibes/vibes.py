# vibes.py

import json
import os
import math
import sys


class vibes(object):
    channel = ''
    current_fig = 'default'

    @classmethod
    def write(cls, msg, figure=None, **kwargs):
        if(cls.channel == ''):
            print('beginDrawing need to be called first')
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
    def beginDrawing(cls):
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
            cls.channel = open(vibes_json, 'a')
        except IOError:
            print('cannot open', vibes_json)
            sys.exit(-1)

    @classmethod
    def endDrawing(cls):
        cls.channel.close()

    @classmethod
    def update(cls):
      cls.channel.flush()

    ##########################################################################
    ##							Figure Management							##
    ##########################################################################

    @classmethod
    def newFigure(cls, figure=''):
        if not figure == '':
            cls.current_fig = figure
        msg ={'action': 'new'}
        cls.write(msg, figure=figure)

    @classmethod
    def setFigureProperties(cls, properties, **kwargs):
        msg = {'action': 'set',
               'properties': properties
              }
        cls.write(msg, **kwargs)

    @classmethod
    def clearFigure(cls, **kwargs):
        cls.write({'action': 'clear'} , **kwargs)

    @classmethod
    def closeFigure(cls, **kwargs):

        cls.write({'action': 'close'}, **kwargs)

    @classmethod
    def saveImage(cls, fileName, **kwargs):
        msg = {'action': 'export',
               'file': '%s' % fileName
              }
        cls.write(msg, **kwargs)

    @classmethod
    def selectFigure(cls, figure=''):
        cls.current_fig = figure

    ##########################################################################
    ##							View settings								##
    ##########################################################################

    @classmethod
    def axisAuto(cls, **kwargs):
        cls.setFigureProperties({"viewbox": "auto"}, **kwargs)

    @classmethod
    def axisEqual(cls, **kwargs):
        cls.setFigureProperties({"viewbox": "equal"}, **kwargs)

    @classmethod
    def axisLimits(cls, x_lb, x_ub, y_lb, y_ub, **kwargs):
        """
        axisLimits : set axisLimits
        example :
          axisLimits(x_lb, x_ub, y_lb, y_ub, figure='')
        """
        cls.setFigureProperties(
            {'viewbox': [x_lb, x_ub, y_lb, y_ub]}, **kwargs)

    @classmethod
    def axisLabels(cls, x_label, y_label, **kwargs):
        """
        axisLabels
        example : axisLabels( x_label, y_label, figur='')
        """
        cls.setFigureProperties({'axislabels': [x_label, y_label]}, **kwargs)

    ##########################################################################
    ##                          Group Management                            ##
    ##########################################################################
    @classmethod
    def newGroup(cls, groupName, **kwargs):
        msg = {'action': 'draw',
               'shape': {'type': 'group',
                         'name': '%s' % groupName,
                         }
              }
        if 'format' in kwargs:
          msg['shape']['format'] = kwargs.pop('format')

        cls.write(msg,  **kwargs)

    @classmethod
    def clearGroup(cls, groupName, **kwargs):
        msg = {'action': 'clear',
               'group': '%s' % groupName
              }
        cls.write(msg, **kwargs)

    ##########################################################################
    ##                         Object Management                            ##
    ##########################################################################
    @classmethod
    def removeObject(cls, objectName, **kwargs):
        msg = {'action': 'delete',
               'figure': '%s' % (figure if figure != None else cls.current_fig),
               'object': '%s' % objectName
              }
        cls.write(msg, **kwargs)

    @classmethod
    def setObjectProperties(cls, objectName, properties, **kwargs):
        msg = {'action': 'set',
               'figure': '%s' % (figure if figure != None else cls.current_fig),
               'object': '%s' % objectName,
               'properties' : properties
              }
        cls.write(msg, **kwargs)

    ##########################################################################
    ##							Drawing functions							##
    ##########################################################################
    @classmethod
    def drawBox(cls, x_lb, x_ub, y_lb, y_ub, color='r', **kwargs):
        '''
        drawBox :
        	Draw a box defined by its corners in figure named <figure>
        parameters
        ----------
        	x_lb : double
        		x lower bound
        	x_ub : double
        		x upper bound
        	y_lb : double
        		y lower bound
        	y_ub : double
        		y upper bound
        	figure : string, optional
        		name of the figure where the box will be draw
        '''
        msg = {'action': 'draw',
               'shape': {'type': 'box',
                        'bounds': [x_lb, x_ub, y_lb, y_ub],
                        'format': color
                        }
               }
        cls.write(msg,**kwargs)

    @classmethod
    def drawBoxesUnion(cls, boxes_bounds, color='r', **kwargs):

        msg = {'action': 'draw',
               'shape': {'type': 'boxes union',
                         'bounds': boxes_bounds,
                         'format': color
                         }
               }
        cls.write(msg, **kwargs)

    @classmethod
    def drawBoxDiff(cls, X0, X, color='r', **kwargs):
        msg = {'action': 'draw',
               'shape': {'type': 'boxes diff',
                         'bounds': [X0, X],
                         'format': color
                        }
              }
        cls.write(msg, **kwargs)

    @classmethod
    def drawLine(cls, points, color='r', **kwargs):
        """
        Draw a N-D line from the list of coordinates
        Parameters
        ----------
            points : list of list
                [[x_1, y_1, z_1, ...], [x_2, y_2, z_2, ...], ...]
        """
        msg = {'action': 'draw',
               'shape': {'type': 'line',
                         'points': points,
                         'format': color
                        }
              }
        cls.write(msg, **kwargs)

    @classmethod
    def drawEllipse(cls, cx, cy, a, b, rot, angles=[0, 360], color='r', **kwargs):
        """
        Draw an ellipse centered at (cx, cy), with semi-major and minor axes a and b,
        and rotated by \a rot degrees
        Parameters
        ----------
            cx,cy : double
                center of the ellipse
            a, b : double
                semi-major and minar axes
            rot : double !!! need to be in degree !!!
                direction of the semi-major axis
            angles: [ double, double]
                starting and ending angle of the Ellispe
        """
        msg = {'action': 'draw',
               'shape': {'type': 'ellipse',
                         'center': [cx, cy],
                         'axis': [a, b],
                         'orientation': rot,
                         'angles': angles,
                         'format': color,
                         }
               }
        cls.write(msg, **kwargs)

    @classmethod
    def drawCircle(cls, cx, cy, R, color='r', **kwargs):
        """
        Draw a circle centered at (cx, cy), with radius r
        Parameters
        ---------
            cx, cy : double
                center of the circle
            r : double
                raduis
        """
        cls.drawEllipse(cx, cy, R, R, 0, color=color, **kwargs)

    @classmethod
    def drawVehicle(cls, cx, cy, length, oritentation, color='r', **kwargs):
        """
        Draw a vehicle centered at (cx, cy) with heading <heading> and size length
        Parameters
        ----------
            cx, cy : double
                position of the vehicle
            heading : double !!! need to be in degree !!!
                heading of the vehicle
            lenght: double
                length of the vehicle
        """
        msg = {'action': 'draw',
               'shape': {'type': 'vehicle',
                         'center': [cx, cy],
                         'length': length,
                         'orientation': oritentation,
                         'format': color
                        }
              }
        cls.write(msg, **kwargs)

    @classmethod
    def drawAUV(cls, cx, cy, oritentation, length, color='r', **kwargs):
        """
        Draw a AUV centered at (cx, cy) with heading <heading> and size length
        Parameters
        ----------
            cx, cy : double
                position of the AUV
            heading : double !!! need to be in degree !!!
                heading of the vehicle
            lenght: double
                length of the vehicle
        """
        msg = {'action': 'draw',
               'shape': {'type': 'vehicle_auv',
                         'center': [cx, cy],
                         'length': length,
                         'orientation': oritentation,
                         'format': color
                         }
              }
        cls.write(msg, **kwargs)

    @classmethod
    def drawPie(cls, center, rho, theta, color='r', use_radian=False, **kwargs):
        """
        Draw a Pie centered at <center> with raduis in <rho> and angle in <theta>
        Parameters
        ----------
            center: [double, double]
                center of the Pie
            rho : [double, double]
                minimal and maximal radius
            theta: [double, double] (in degree)
                minimal and maximal angle
            use_radian : boolean
                if True theta is in radian
        """
        msg = {'action': 'draw',
               'shape': {'type': 'pie',
                         'center': [center[0], center[1]],
                         'rho': [rho[0], rho[1]],
                         'theta': [theta[0], theta[1]] if use_radian == False else [math.degrees(theta[0]), math.degrees(theta[1])],
                         'format': color,
                        }
              }
        cls.write(msg, **kwargs)

    @classmethod
    def drawArrow(cls, A, B, tip_length, color='r', **kwargs):
        """
        Draw an Arrow between A and B
        Parameters
        ----------
            A : [double, double]
                starting point (ax, ay)
            B : [double, double]
                ending point (bx, by)
            tip_length:
                length of the tip
        """
        msg = {'action': 'draw',
               'shape': {'type': 'arrow',
                         'points': [A, B],
                         'tip_length': tip_length,
                         'format': color
                         }
              }
        cls.write(msg, **kwargs)

    @classmethod
    def drawPolygon(cls, points, color='r', **kwargs):
        """
        Draw a Polygon defined by <points>
        Parameters
        ----------
            points:  list of lists
                list of points. ex [ [p1x, p1y] ,  [p2x, p2y], ..., [pnx, pny]]
            color: string, optional
                color used for drawing
            figure: string, optional
                figure on which the shape will be draw
    	  """
        msg = {'action': 'draw',
               'shape': {'type': 'polygon',
                         'bounds': points,
                         'format': color
                         }
              }
        cls.write(msg, **kwargs)

    @classmethod
    def drawPoint(cls, cx, cy, color='r', **kwargs):
        """
        Draw a Point at position (cy, cy)
        """
        msg = {'action': 'draw',
                'shape': {'type': 'point',
                          'point': [cx, cy],
                          'radius' : 3,
                          'format': color
                          }
              }
        cls.write(msg, **kwargs)
    @classmethod
    def drawRing(cls, cx, cy, r_min, r_max, color='r', **kwargs):
        """
        Draw a ring at position (cx, cy) with radius between (r_min, r_max)
        Parameters
        ----------
            cx, cy : double
                center if the ring
            r_min, r_max: double
                bound of the radius in degree
        """
        msg = {'action': 'draw',
                'shape': {'type': 'ring',
                          'center': [cx, cy],
                          'rho': [r_min, r_max],
                          'format': color
                          }
              }
        cls.write(msg, **kwargs)

    @classmethod
    def drawText(cls, cx, cy, text, scale, color='r', **kwargs):
        """
        Draw a text <text> at position (cx, cy)
        Parameters
        ----------
            cx, cy : double
                position of the text
            text: string
                text to draw
        """
        msg = {'action': 'draw',
                'shape': {'type': 'text',
                          'text' : text,
                          'position': [cx, cy],
                          'scale' : scale,
                          'format': color
                          }
              }
        cls.write(msg, **kwargs)
