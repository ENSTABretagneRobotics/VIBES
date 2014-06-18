function newFigure( figure_name )
global current_fig;
global vibesPipeFile;
    if isa(figure_name,'char')
        current_fig=figure_name;
    end
    msg=['{"action":"new","figure":"' current_fig '"}"'];
    fprintf(vibesPipeFile,'%s\n\n',msg);
end

