%% Get MVC
mvc = 100; %just an init value
raw_filepath = '../LabviewFiles/Data/Sistania_Precision_Sitting/MVC 2RawEMG.csv';
processRawData;
windowWidth = 500; %samples
mvc = max(movmean(smooth_data,windowWidth));


smooth = cell(6,1);
trialNumbers = [26 28 29 27 24 25];%5, 8, 10, 12, 15, 20
numTrials = size(trialNumbers,2);

combineTrials = false;
if combineTrials
    for i = 1:6
        combined = [];
        for j = 1:3
            trialNum = trialNumbers(i,j);
            if trialNum > 9
                raw_filepath = ['../LabviewFiles/Data/Sistania_Precision/Trial' num2str(trialNum) '_RawData.csv'];
            else
                raw_filepath = ['../LabviewFiles/Data/Sistania_Precision/Trial0' num2str(trialNum) '_RawData.csv'];
            end
            processRawData;
            combined = [combined smooth_data];  %append smoothed data for given activation
        end
        smooth(i,1) = {smooth_data};
    end
else
    for i = 1:numTrials %5,6,7,8,9 trials
        trialNum = trialNumbers(1,i);
        raw_filepath = ['../LabviewFiles/Data/Sistania_Precision_Sitting/Trial' num2str(trialNum) '_RawData.csv'];
        processRawData;
        smooth(i,1) = {smooth_data};
    end
end


close all;
%Do histograms
figure(1);
hold on;

%   Get means for later use
for i = 1:numTrials
    m(i) = mean(smooth{i,1});
end

for i = numTrials:-1:1
    histfit(smooth{i,1}, 20);
end

for i = numTrials:-1:1
    plot(m(i), 0, '*r');
    text(m(i),-80,num2str(m(i)),'VerticalAlignment','top','Rotation', -45, 'Color', 'r');
end
title('Activation Distribution for Activation Targets of 5%, 8%, 10%, 12%, 15%, and 20%');



figure(2)
hold on
activations = ["5%", "8%", "10%", "12%", "15%", "20%"];

%Chop off excess data to normalize array sizes
min = size(smooth{1,1},1);
for i=2:size(smooth,1)
    if size(smooth{i,1},1) < min
        min = size(smooth{i,1},1);
    end
end


boxplot([smooth{1,1}(1:min,1) smooth{2,1}(1:min,1) smooth{3,1}(1:min,1) smooth{4,1}(1:min,1) smooth{5,1}(1:min,1) smooth{6,1}(1:min,1)], activations);
title('EMG Activation Distribution Across Targets 5%, 8%, 10%, 12%, 15%, and 20% Combining Like Trials, lowpass-smooth');
ylabel('Percent of MVC')
xlabel('Target Activation')
for i = 1:6
    text(i,m(i),num2str(m(i)),'HorizontalAlignment','left');
end

