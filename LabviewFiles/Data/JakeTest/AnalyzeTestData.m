close all
%Get data points of interest
%load data into table 
% 5% trials 1-3
% 8% trials 4 - 7
% 10% trials 8 - 10
% 12% trials 11 -13
% 15% trials 14 - 16
% 18% trials 17 - 19
% 20% trials 20 - 22

trialNames = [5 10 15 20];%[5 8 10 12 15 18 20];
%trialsOfInterest = %[1 4 8 11 14 17 20];
channel = 1;
colors = ['rgbmkyc'];

filteredPattern = '*_FilteredData.csv';
displayPattern = '*_DisplayData.csv';

filtfiles = dir(filteredPattern);
dispfiles = dir(displayPattern);
size(trialsOfInterest,2)

pd = gobjects(size(trialNames,1),1); %clear plot array

stdevs = zeros(size(trialNames,2),1);
means = zeros(size(trialNames,2),1);


for i = size(trialNames,2):-1:1

    %load trial data
    trialNum = Log(Log.TrialName == trialNames(i),'TrialNumber');
    trialNum = trialNum.TrialNumber(1);
    filtered = csvread(filtfiles(trialsOfInterest(1,i),1).name,1,0);
    display = csvread(dispfiles(trialsOfInterest(1,i),1).name,1,0);
    
    %extract only time stamps and display channel
    filt = filtered(:,1:2);
    disp = display(:,1:2);
    
    %remove empty rows
    filt = filt(any(filt,2),:);
    disp = disp(any(disp,2),:);
    
    %normalize filtered data to the mvc
    mvc = Log(Log.TrialName == 5 & Log.TrialNumber == 1,:);
    mvc = mvc.MVCValue;
    filt(:,2:end) = (filt(:,2:end) / mvc(1,1)) * 100;
    
    %plot the data
    figure(1)
    pf(i) = plot(filt(:,1), filt(:,2), colors(i));
    hold on;
    pd(i) = plot(disp(:,1), disp(:,2), colors(i),'LineWidth',2);
    
    %Get stdev of filtered data
    stddev(i,1) = std(filt(1000:9000,2));
    means(i,1) = mean(filt(1000:9000,2));
    
    figure(2);hold on;
    peaks = findpeaks(filt(:,2), 'MinPeakWidth', 5);
    plot(peaks);
    
    figure(3); hold on;
    histfit(peaks);
end

figure(1)
%bring lines to front of plot
uistack(pd,'top');

%set title, legend, etc
title('Display values and Filtered EMG Data From Trials with  Various Target Activations');
legend( string(fliplr(trialNames)) + '% MVC Target');

xlabel('Time (s)') % x-axis label
ylabel('Muscle Activity as % of MVC ') % y-axis label

xlim([0 inf]);
ylim([0 100]);



%try building histfit and normal dist plot



