function axisAuto()
global vibesPipeFile;
global current_fig;

msg=struct('action','set','figure',current_fig,'properties',struct('viewbox','auto'));
fprintf(vibesPipeFile,'%s\n\n',vibes.json.dump(msg));
