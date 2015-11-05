# vibes.py

import json
import os
import math
import sys


class vibes(object):
    channel = ''
    current_fig = 'default'

    @classmethod
    def write(cls, msg):
        if(cls.channel == ''):
            print('beginDrawing need to be called first')
        else:
            cls.channel.write(msg + '\n\n')
            cls.channel.flush()

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

    ##########################################################################
    ##							Figure Management							##
    ##########################################################################
    
    @classmethod
    def newFigure(cls, figureName):
        if not figureName == '':
            cls.current_fig = figureName
        msg = json.dumps({'action': 'new',
                          'figure': '%s' % (cls.current_fig if figureName == '' else figureName)
                          }, indent=4, separators=(',', ': '))
        cls.write(msg)

    @classmethod
    def setFigureProperties(cls, properties, figureName=''):
        msg = json.dumps({'action': 'set',
                          'figure': '%s' % (cls.current_fig if figureName == '' else figureName),
                          'properties': properties

                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)

    @classmethod
    def clearFigure(cls, figureName=''):
        msg = json.dumps({'action': 'clear',
                          'figure': '%s' % (cls.current_fig if figureName == '' else figureName)
                          })
        cls.write(msg)

    @classmethod
    def closeFigure(cls, figureName=''):
        msg = json.dumps({'action': 'close',
                          'figure': '%s' % (figureName if not figureName == '' else cls.current_fig)
                          })
        cls.write(msg)

    @classmethod
    def saveImage(cls, fileName, figureName=''):
        msg = json.dumps({'action': 'export',
                          'figure': '%s' % (figureName if not figureName == '' else cls.current_fig),
                          'file': '%s' % fileName
                          })
        cls.write(msg)

    @classmethod
    def selectFigure(cls, figureName=''):
        cls.current_fig = figureName

    ##########################################################################
    ##							View settings								##
    ##########################################################################

    @classmethod
    def axisAuto(cls, figureName=''):
        cls.setFigureProperties({"viewbox": "auto"}, figureName)

    @classmethod
    def axisEqual(cls, figureName=''):
        cls.setFigureProperties({"viewbox": "equal"}, figureName)

    @classmethod
    def axisLimits(cls, x_lb, x_ub, y_lb, y_ub, figureName=''):
        cls.setFigureProperties(
            {'viewbox': [x_lb, x_ub, y_lb, y_ub]}, figureName)

    @classmethod
    def axisLabels(cls, x_label, y_label, figureName=''):
        cls.setFigureProperties({'axislabels': [x_label, y_label]}, figureName)

    ##########################################################################
    ##							Drawing functions							##
    ##########################################################################
    @classmethod
    def drawBox(cls, x_lb, x_ub, y_lb, y_ub, color='r', figure=None):
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
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'box',
                                    'bounds': [x_lb, x_ub, y_lb, y_ub],
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)

    @classmethod
    def drawBoxesUnion(cls, boxes_bounds, color='r', figure=None):

        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'boxes union',
                                    'bounds': boxes_bounds,
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)


    @classmethod
    def drawLine(cls, points, color='r', figure=None):
        """
        Draw a N-D line from the list of coordinates 
        Parameters
        ----------
            points : list of list 
                [[x_1, y_1, z_1, ...], [x_2, y_2, z_2, ...], ...]
        """
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'line',
                                    'points': points,
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)

    @classmethod
    def drawEllipse(cls, cx, cy, a, b, rot, angles=[0, 360], color='r', figure=None):
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
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'ellipse',
                                    'center': [cx, cy],
                                    'axis': [a, b],
                                    'orientation': rot,
                                    'angles': angles,
                                    'format': color,
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)

    @classmethod
    def drawCircle(cls, cx, cy, R, color='r', figure=None):
        """
        Draw a circle centered at (cx, cy), with radius r
        Parameters
        ---------
            cx, cy : double
                center of the circle
            r : double
                raduis
        """
        cls.drawEllipse(cx, cy, R, R, 0, color=color, figure=figure)

    @classmethod
    def drawVehicle(cls, cx, cy, heading, length, color='r', figure=None):
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
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'vehicle',
                                    'center': [cx, cy],
                                    'length': length,
                                    'orientation': heading,
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)

    @classmethod
    def drawAUV(cls, cx, cy, heading, length, color='r', figure=None):
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
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'vehicle_auv',
                                    'center': [cx, cy],
                                    'length': length,
                                    'orientation': heading,
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)

    @classmethod
    def drawPie(cls, center, rho, theta, color='r', use_radian=False, figure=None):
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
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'pie',
                                    'center': [center[0], center[1]],
                                    'rho': [rho[0], rho[1]],
                                    'theta': [theta[0], theta[1]] if use_radian == False else [math.degrees(theta[0]), math.degrees(theta[1])],
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)

    @classmethod
    def drawArrow(cls, A, B, tip_length, color='r', figure=None):
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
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'arrow',
                                    'points': [A, B],
                                    'tip_length': tip_length,
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)

    @classmethod
    def drawPolygon(cls, points, color='r', figure=None):
        '''
        Draw a Polygon defined by <points>
        Parameters
        ----------
            points:  list of lists
                list of points. ex [ [p1x, p1y] ,  [p2x, p2y], ..., [pnx, pny]]
            color: string, optional
                color used for drawing
            figure: string, optional
                figure on which the shape will be draw
    	  '''
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'polygon',
                                    'bounds': points,
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)

    @classmethod
    def drawPoint(cls, cx, cy, color='r', figure=None):
        """
        Draw a Point at position (cy, cy)
        """
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'point',
                                    'point': [cx, cy],
                                    'radius' : 3,
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)
    @classmethod
    def drawRing(cls, cx, cy, r_min, r_max, color='r', figure=None):
        """
        Draw a ring at position (cx, cy) with radius between (r_min, r_max)
        Parameters
        ----------
            cx, cy : double
                center if the ring
            r_min, r_max: double
                bound of the radius in degree
        """
        msg = json.dumps({'action': 'draw',
                          'figure': '%s' % (figure if figure != None else cls.current_fig),
                          'shape': {'type': 'ring',
                                    'center': [cx, cy],
                                    'rho': [r_min, r_max],
                                    'format': color
                                    }
                          },  sort_keys=False)  # indent=4, separators=(',', ': '),)
        cls.write(msg)
