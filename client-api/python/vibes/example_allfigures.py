from vibes import vibes

def shapeGen():
	x, y = yield
	(x,y) = yield vibes.drawArrow([x-1, y-1], [x+1, y+1], 0.6, 'k[k]')
	(x,y) = yield vibes.drawAUV(x,y,45,2, 'k[y]')
	(x,y) = yield vibes.drawBox(x-1,x+1,y-2,y+2, "[b]")
	(x,y) = yield vibes.drawBoxesUnion([[x-1,x,y-2,y], [x-0.5,x+1,y-0.5,y+2]], "[r]")
	(x,y) = yield vibes.drawCircle(x,y,1, "g[m]")
	(x,y) = yield vibes.drawEllipse(x,y,2,1,45, color="r[darkCyan]")
	(x,y) = yield vibes.drawLine([[x-0.5, y-1], [x+1, y+0.5]], "k")
	(x,y) = yield vibes.drawPie( (x-2, y-2), (1,2.5), [20,70] , "y[cyan]")
	(x,y) = yield vibes.drawPolygon([[x-1,y-1], [x, y+1], [ x+1, y-1]], "k[orange]")
	(x,y) = yield vibes.drawRing(x,y, 1,2, '[red]')
	# (x,y) = yield vibes.drawPie( (x, y), (1,2), [0,360] , "y[cyan]")
	(x,y) = yield vibes.drawVehicle( x, y, 20, 1, "[darkBlue]")
	(x,y) = yield vibes.drawPoint( x,y, 1, "[k]")
	(x,y) = yield vibes.drawText( x,y, "Text", 1)

vibes.beginDrawing()
vibes.newFigure("example")
vibes.setFigureProperties( { "x": 100,"y": 100,"width": 500,"height": 500} )

dd =shapeGen()
dd.send(None)
# dd.next()
for x in range(0,14,4):
	for y in range(0,14,4):
		try:
			dd.send((y,-x))
		except StopIteration:
			# print("stop iter")
			break

vibes.axisEqual()
