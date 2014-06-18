function beginDrawing( filename )
    vibes.json.startup; % Load the .jar associated vith the json lib
    global vibesPipeFile;
    
    if nargin > 0
        if isa(filename,'char')
            vibesPipeFile=fopen(filename,'w');
        else
            disp('Connect expects a string as an argument');
        end
    else
        home_path=getenv('USERPROFILE'); % WINDOWS
        if isempty(home_path) % UNIX
            home_path=getenv('HOME');
        end
        if isempty(home_path)
            vibesPipeFile=fopen('vibes.json','w');
        else
            vibesPipeFile=fopen([home_path '/.vibes.json'],'w');
        end
    end
end

