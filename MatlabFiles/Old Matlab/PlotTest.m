function [ p ] = PlotTest( test )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
trialnames = fieldnames(test);
numtrials = length(trialnames);
allvalues = [];
last = 0;
hold on;
for j=1:numtrials
    trial = test.(trialnames{j});
    values = trial.values;
    allvalues = cat(1, allvalues, values);
    p = plot(values);
    if length(values) > last
        last = length(values);
    end
end
avg = mean(allvalues);
avgline = refline(0,avg);
avgline.Color = 'r';

stddev = std2(allvalues);

rootmeansquare = rms(allvalues);
rmsline = refline(0, rootmeansquare);
rmsline.Color = 'b';

sampleRate = 2000;
sampleSize = 67;   %sampleRate / 30;
movingmean = movmean(allvalues, sampleSize);
meanline = refline(0, movingmean);
meanline.Color = 'g';

movingrms = step(movRMS, allvalues);
movrmsline = refline(0, movingrms);
movrmsline.Color = 'i';
%devline1 = refline(0,avg-stddev);
%devline2 = refline(0,avg+stddev);
%devline1.Color = 'y';
%devline2.Color = 'y';

x = [0, last, last, 0];
y = [avg+stddev, avg+stddev, avg-stddev, avg-stddev];

patch(x,y,'c');
alpha(0.2);

hold off;
end

