%% Get MVC
mvc = 100;
raw_filepath = '../LabviewFiles/Data/Sistania_Precision/MVC 1RawEMG.csv';
processRawData;
windowWidth = 500; %samples
mvc = max(movmean(smooth_data,windowWidth));


smooth = cell(6,1);

combineTrials = true;
if combineTrials
    trialNumbers = [5 16 20; 6 12 18; 7 15 17; 8 11 21; 9 14 19; 10 13 22];%5, 8, 10, 12, 15, 20
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
    for i = 1:5 %5,6,7,8,9 trials
        raw_filepath = ['../LabviewFiles/Data/Sistania_Precision/Trial0' num2str(i+4) '_RawData.csv'];
        processRawData;
        smooth(i,1) = {smooth_data};
    end
    raw_filepath = '../LabviewFiles/Data/Sistania_Precision/Trial10_RawData.csv';
    processRawData;
    smooth(6,1) = {smooth_data};
end


close all;
%Do histograms
figure(1);
hold on;

%   Get means for later use
for i = 1:6
    m(i) = mean(smooth{i,1});
end

for i = 6:-1:1
    histfit(smooth{i,1}, 5);
end

for i = 6:-1:1
    plot(m(i), 0, '*r');
    text(m(i),-80,num2str(m(i)),'VerticalAlignment','top','Rotation', -45, 'Color', 'r');
end


figure(2)
hold on
activations = ["5%", "8%", "10%", "12%", "15%", "20%"];
boxplot([smooth{1,1} smooth{2,1} smooth{3,1} smooth{4,1} smooth{5,1} smooth{6,1}], activations);
title('EMG Activation Distribution Across Targets 5%, 8%, 10%, 12%, 15%, and 20% Combining Like Trials');
for i = 1:6
    text(i,m(i),num2str(m(i)),'HorizontalAlignment','left');
end


