function axisLabels( varargin )
% axisLabels Set axis labels for a VIBes figure.
%   axisLabels('label1','label2',...) set labels of current figure.
%   axisLabels({'label1','label2',...}) set labels of current figure.
%   axisLabels({'label1','label2',...}, Figure) set labels of given Figure.

global vibesPipeFile;
global current_fig;
% Take at least one argument
if (nargin < 1)
    error('axisLabels: Not enough arguments');
end
% Operate on current figure, unless specified
figure = current_fig;

% The only argument is a cellstr: use its contents as labels
if (nargin==1 && iscellstr(varargin{1}))
    labels = reshape(varargin{1},1,length(varargin{1}(:)));
% Two arguments: a list a of label strings an a figure name
elseif (nargin==2 && iscellstr(varargin{1}))
    labels = reshape(varargin{1},1,length(varargin{1}(:)));
    figure = varargin{2};
% Labels strings are directly provided as arguments
else
    labels = varargin;
end

% Create and send message
msg=struct('action','set','figure',figure,'properties',struct('axislabels',{labels}));
fprintf(vibesPipeFile,'%s\n\n',vibes.json.dump(msg));


