function drawLine( points, color )
    global vibesPipeFile;
    global current_fig;
    msg = struct('action','draw','figure',current_fig,'shape',struct('type','line','points',points,'format',color));
    fprintf(vibesPipeFile,'%s\n\n',vibes.json.dump(msg));
end