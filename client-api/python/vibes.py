#vibes.py

import json
import os


class vibes(object):
	channel = ''
	current_fig = 'default'

	@classmethod
	def write(cls, msg):
		if( cls.channel == ''):
			print('beginDrawing need to be called first')
		else :
			cls.channel.write(msg+'\n\n')
			cls.channel.flush()

	@classmethod
	def beginDrawing(cls):
		vibes_json = 'vibes.json'
		# Retrieve user-profile directory from envirnment variable
		user_dir = os.getenv("USERPROFILE") # Windows
		if not user_dir: 
			user_dir = os.getenv("HOME"); # POSIX
		if user_dir:
			# Environment variable found, connect to a file in user's profile directory
			vibes_json = user_dir+'/.vibes.json'
		
		try:
			cls.channel = open(vibes_json, 'a')
		except IOError:
			print('cannot open', vibes_json)
			sys.exit(-1)

	@classmethod
	def endDrawing(cls):
		cls.channel.close()
	@classmethod
	def newFigure(cls,figureName):
		if not figureName == '':
			cls.current_fig = figureName
		# msg =json.dumps('{\n"action":"new","figure":"%s"\n}\nq\n' %(current_fig if figureName == '' else figureName) )
		msg =json.dumps({ 'action':'new','figure':'%s'%(current_fig if figureName == '' else figureName)}, indent=4, separators=(',', ': '))
		cls.write(msg)

	@classmethod
	def drawBox(cls,x_lb, x_ub, y_lb, y_ub,color='r', figure=None ):
		msg = json.dumps({ 'action':'draw', 
			'figure':'%s'%(figure if  figure != None else cls.current_fig),
			'shape': { 'type' : 'box', 
						'bounds': [x_lb, x_ub, y_lb, y_ub],
						'format': color
					}
			},  sort_keys=False)# indent=4, separators=(',', ': '),)
		cls.write(msg)

	@classmethod
	def drawLine(cls, points, color='r', figure=None):
		msg = json.dumps({ 'action':'draw', 
			'figure':'%s'%(figure if  figure != None else cls.current_fig),
			'shape': { 'type' : 'line', 
						'points' : points,
						'format': color
					}
			},  sort_keys=False)# indent=4, separators=(',', ': '),)
		cls.write(msg)

	@classmethod
	def drawEllipse(cls,cx, cy, a, b, rot, angles = [0, 360], color='r', figure=None):
		msg = json.dumps({ 'action':'draw', 
			'figure':'%s'%(figure if  figure != None else cls.current_fig),
			'shape': { 'type' : 'ellipse',
						'center' : [cx, cy],
						'axis' : [a, b],
						'orientation' : rot,
						'angles' : angles,
						'format' : color,
					}
			},  sort_keys=False)# indent=4, separators=(',', ': '),)
		cls.write(msg)

	@classmethod
	def drawCircle(cls, cx, cy, R, color='r', figure=None):
		cls.drawEllipse(cx, cy, R, R, 0, color=color, figure=figure)


	@classmethod
	def drawAUV(cls, cx, cy, length, oritentation, color='r', figure=None):
		msg = json.dumps({ 'action':'draw', 
			'figure':'%s'%(figure if  figure != None else cls.current_fig),
			'shape': { 'type' : 'vehicle_auv',
						'center' : [cx, cy],
						'length' : length,
						'orientation' : oritentation,
						'format' : color
					}
			},  sort_keys=False)# indent=4, separators=(',', ': '),)
		cls.write(msg)

	@classmethod
	def drawPie(cls, center, rho, theta, color='r', figure=None):
		msg = json.dumps({ 'action':'draw', 
			'figure':'%s'%(figure if  figure != None else cls.current_fig),
			'shape': { 'type' : 'pie',
						'center' : center,
						'rho' : rho,
						'theta' : theta,
						'format' : color
					}
			},  sort_keys=False)# indent=4, separators=(',', ': '),)
		cls.write(msg)		

	@classmethod
	def clearFigure(cls, figureName=''):
		msg = json.dumps({'action':'clear',
		'figure': '%s'%(cls.current_fig if figureName=='' else figureName)
		})
		cls.write(msg)


	@classmethod
	def closeFigure(cls, figureName=''):
		msg=json.dumps({'action':'close',
			'figure':'%s'%(figureName if not figureName=='' else current_fig)
			})
		cls.write(msg)
	@classmethod
	def saveImage(cls, fileName, figureName=''):
		msg=json.dumps({'action':'export',
			'figure':'%s'%(figureName if not figureName=='' else current_fig),
			'file':'%s'%fileName
			})
		cls.write(msg)
  
	@classmethod
	def selectFigure(cls, figureName=''):
		cls.current_fig = figureName;



#
# View settings
#

	# def axisAuto(figureName=''):
		# setFigureProperty(figureName.empty()?current_fig:figureName, "viewbox", "auto");
 # 
  # void axisLimits(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, const std::string &figureName)
  # {
    # setFigureProperty(figureName.empty()?current_fig:figureName, "viewbox", (Vec4d){x_lb,x_ub,y_lb,y_ub});
  # }
# 
  # void axisLabels(const std::string &x_label, const std::string &y_label, const std::string &figureName)
  # {
    # vector<string> labels;
    # labels.push_back(x_label);
    # labels.push_back(y_label);
    # axisLabels(labels, figureName);
  # }
# 
  # void axisLabels(const std::vector<std::string> &labels, const std::string &figureName)
  # {
    # setFigureProperty( figureName.empty()?current_fig:figureName, "axislabels", labels);
  # }
	@classmethod
	def test(cls):
		msg = json.dumps({
		"action": "draw",
		"figure": "test",
		"shape": {
			"type": "box",
			"bounds": [-1, 3, -1, 2],
			"format": "r"
			}
		},sort_keys=False)# indent=4, separators=(',', ': '))
		cls.channel.write(msg+'\n\n')
		cls.channel.flush()