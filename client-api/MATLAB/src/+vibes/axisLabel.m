function axisLabel( labels )
global vibesPipeFile;
global current_fig;
    msg=struct('action','set','figure',current_fig,'properties',struct('axislabels',labels));
    fprintf(vibesPipeFile,'%s\n\n',vibes.json.dump(msg));
end

