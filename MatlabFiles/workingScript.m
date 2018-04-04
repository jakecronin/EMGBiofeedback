%% Load the Trials and MVCs

%Get session directory
sessionDir = dir('../LabviewFiles/Data/Default');

%RUN THROUGH ALL FILES LISTED IN DIR
for i = 1:size(sessionDir,1)
    filePath = [sessionDir(i).folder '/' sessionDir(i).name];
    [~, name, ext] = fileparts(filePath);
    if ~strcmp(ext, '.csv')
        disp('skipping non csv');
         %Skip files that don't end in .csv
    elseif strcmp(name,'Log')
        %Load Logfile
        disp('skipping log')
    elseif strcmp(name(1:3),'MVC') %Load MVC Data
        data = csvread(filePath,1,0);
        data(~any(data,2), : ) = [];  %Delete Empty Rows
        data = data(:,1:2); %Grab only relevant columns
        mvcNum = str2num(name(4:5));
        
         %Put into struct
        if name(6) == 'R' %Ignore Display Data
            mvcs(mvcNum).raw = data;
        elseif name(7) == 'F'
            mvcs(mvcNum).filt = data;
        end
        
    elseif strcmp(name(1:5),'Trial') %Load trial data       
        data = csvread(filePath,1,0);
        data(~any(data,2), : ) = [];  %Delete Empty Rows
        data = data(:,1:2); %Grab only relevant columns
        trialNum = str2num(name(6:7));
        
        %Put into struct
        if name(9) == 'D' %Ignore Display Data
            trials(trialNum).disp = data;
        elseif name(9) == 'F'
            trials(trialNum).filt = data;
        elseif name(9) == 'R' % Ignore Filtered Data
            trials(trialNum).raw = data;
        end
    end
end


%% Test Calculated MVCs
%for i = 1:size(mvcs,2)
%   matlabMVCs(i) = max(movmean(mvcs(i).filt(:,2),500));
%end

%% Create Box Plot


trialArrangement = [1 2 3; 4 5 6; 7 8 9; 10 11 12; 13 14 15; 16 17 18];


trials(trialArrangement(1,:));

boxplot(trials(1).filt(:,2),'OutlierSize',100)

dat = trials(3).filt(:,2) - mean(trials(3).filt(:,2));
[pxx,f] = periodogram(dat,[],[],1000);
plot(f,pxx)
ax = gca;
ax.XLim = [0 10];
xlabel('Frequency (cycles/week)')
ylabel('Magnitude')










