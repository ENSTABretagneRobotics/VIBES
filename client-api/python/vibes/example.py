#exmaple.py

from vibes import vibes

print(vibes.channel, vibes.current_fig)

vibes.beginDrawing()
vibes.newFigure("test")
vibes.drawBox(0,1,4,6,color='[#FF12FFA0]', figure='test')
vibes.drawText(12, 12, 'My Text', 0.1, 'b[r]')
vibes.axisAuto()
vibes.drawBox(0.5,2,4,7,color='[#00FFFF66]', figure='test')
vibes.drawBox(-1,0,-4,-6,color='r', figure='test')
vibes.drawLine([[0,0], [4,4]])
vibes.drawEllipse(3, 5, 1, 3, 45)
vibes.drawEllipse(5, 5, 3, 3, 0, angles=[30,60], color='g')

vibes.drawCircle(0, 0, 5)
vibes.drawAUV(0,0, 2, 3, color='r[yellow]')


vibes.drawPie([0,0], [3,4], [45, 90])

vibes.newGroup("Pie", figure="test", format='[cyan]')
vibes.drawPie([0,0], [5,9], [-120, -40], group="Pie")
# vibes.drawPie([0,0], [5,9], [-120, -40], "[b]")

vibes.drawPie([5,2], [1,2], [160, 220]) #, 'g[y]')

# vibes.clearGroup("Pie", figure="test")

vibes.drawPie([0,0], [1,2], [160, 220]) #, 'g[y]')


# exit()
vibes.newFigure("test2")
vibes.setFigureProperties({'x':0, 'y':0, 'width':600, 'height':600})

# vibes.axisLimits(10,20,10,20)
vibes.axisLabels("titi", "toto")

vibes.showAxis(False)
# vibes.drawBox(-1,0,-4,-6,color='[r]', figure='test2')



# vibes.selectFigure("test")
# vibes.clearFigure()
# vibes.selectFigure("test2")
# vibes.saveImage('./test.svg', "test")
# vibes.closeFigure("test2")

vibes.drawBoxDiff([11, 13, 11, 24], [12,12.5, 11, 13], 'b[r]')

vibes.drawArrow([9, 9], [9, 10], 0.3, '[r]')
vibes.drawArrow([9, 9], [10, 9], 0.3, '[r]')

vibes.drawPolygon([[1,1], [1,4], [5,2], [5, 1]])
vibes.setFigureProperties({'viewbox':'equal'})

vibes.newFigure("raster")
vibes.drawArrow((0,0), (1,1), 0.1)


vibes.endDrawing()
