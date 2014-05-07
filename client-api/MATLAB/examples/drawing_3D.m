%%% To use this example, the ../src/+vibes package must be in your MATLAB PATH variable

vibes.beginDrawing();           % Initialize VIBes connection
vibes.newFigure('sin and cos'); % Create a new VIBes figure

x = 0:0.1:10;
sinx = sin(x);
cosx = cos(x);

points = [x', sinx', cosx'];

vibes.drawLine(points, 'blue'); % Draw the 3D curve
vibes.axisAuto();
vibes.axisLabel(['x    '; 'cos x'; 'sin x']);

vibes.endDrawing();             % Closes VIBes connection