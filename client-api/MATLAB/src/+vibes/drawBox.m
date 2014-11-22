function drawBox(varargin)
    global vibesPipeFile;
    global current_fig;
    v=-1; % Used to tell wether the function got a single box or a list of boxes
    params={};
    bounds=[];
    fig_name=current_fig;
    if size(varargin{1},2) == 2,
        v=2;
        bounds=varargin{1}';
        bounds=bounds(:)';
    elseif size(varargin{1},1) == 2,
        v=2;
        bounds=varargin{1};
        bounds=bounds(:)';
    elseif size(varargin{1},1) == 1 && size(varargin{2},1) == 1 && size(varargin{3},1) == 1 && size(varargin{4},1) == 1,
        v=5;
        bounds=varargin(1:4);
    end
    
    format=0;
    
    if mod(size(varargin(v:end),2),2) ~= 0, % got an odd number of parameters
        format = varargin(end);
        varargin(end)=[];
    end
    
    params=varargin(v:end);
    chars=find(cellfun('isclass',params,'char'));
    idx_fig=strmatch('figure',params(chars));
    
    if size(idx_fig,2) ~= 0
        fig_name=params(idx_fig+1);
    end
    
    params(idx_fig:idx_fig+1)=[];
    params(:)
    msg=struct('action','draw','figure',fig_name,{'shape',params(:)});
    
    %msg=['{"action":"draw","figure":"' current_fig '","shape":{"type":"box","bounds":}}"'];
    %fprintf(vibesPipeFile,'%s\n\n',msg);
end