function axisAuto()
global vibesPipeFile;
global current_fig;
    msg=struct('figure',current_fig,'action','view','box','auto');
    fprintf(vibesPipeFile,'%s\n\n',vibes.json.dump(msg));
end

