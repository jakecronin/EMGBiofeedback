%Analyze_Precision_Experiments looks at precision testing and bin testing
%from trials with 6 subjects (Sistania, Brian, Phil, Ray, Daniel, and
%Antonio

%Purpose: 
%   1) Show precision in distinguishing between activation
%   percentages for subjects given an activation target
%   
%   2) Check how precision changes as biofeedback becomes more or less
%   sensitive


%% Pull data from each subjet's trials

% Steps:
%   1) get MVC for scaling
%   2) get data from each trial into cell array (subjects X 6)
%   [{name} {filename} {raw} {filt} etc
%   {name}                              ]

% SET SOME PARAMETERS
    windowWidth = 500; %equates to 500ms window (1000hz sampling, 500 samples)

%  Begin with Sistania
    
    raw_filepath = '../LabviewFiles/Data/Sistania_Precision_Sitting/MVC 2RawEMG.csv';
    windowWidth = 500; %samples
    mvc = max(movmean(smooth_data,windowWidth));


%Make figure with all people's boxplots and histograms
%Make figure with combined boxplot and histogram