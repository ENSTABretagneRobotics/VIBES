#exmaple.py

from vibes import vibes

print(vibes.channel, vibes.current_fig)

vibes.beginDrawing()
vibes.newFigure("test")
vibes.drawBox(0,1,4,6,color='[#FF12FFA0]', figure='test')
vibes.drawBox(0.5,2,4,7,color='[#00FFFF66]', figure='test')
vibes.drawBox(-1,0,-4,-6,color='r', figure='test')
vibes.drawLine([[0,0], [4,4]])
vibes.drawEllipse(3, 5, 1, 3, 45)
vibes.drawEllipse(5, 5, 3, 3, 0, angles=[30,60], color='g')

vibes.drawCircle(0, 0, 5)
vibes.drawAUV(0,0, 2, 3, color='r[yellow]')

vibes.drawPie([0,0], [3,4], [45, 90])

vibes.drawPie([0,0], [5,9], [-120, -40], "[b]")

vibes.drawPie([0,0], [1,2], [160, 220], 'g[y]')

vibes.drawPie([5,2], [1,2], [160, 220], 'g[y]')



vibes.newFigure("test2")
# vibes.drawBox(-1,0,-4,-6,color='[r]', figure='test2')

# vibes.selectFigure("test")
# vibes.clearFigure()
# vibes.selectFigure("test2")
# vibes.saveImage('./test.svg', "test")
# vibes.closeFigure("test2")

vibes.test()
vibes.endDrawing()

