%%Get trial sub-folders
d = dir(pwd);
isub = [d(:).isdir]; %# returns logical vector
nameFolds = {d(isub).name}';
nameFolds(ismember(nameFolds,{'.','..'})) = [];

%Sort Folders by MS
s = sprintf('%s ', nameFolds{:});
foldVals = sscanf(s, '%gMS');
[sortedVals, sortIndex] = sort(foldVals);
sortedNames = nameFolds(sortIndex);

%%
%Pull data from each trial into structs{xRaw, yRaw, xFilt, yFilt, xDisp,
%yDisp
clear trials
for i = 1:numel(nameFolds)
    name = sortedNames{i};
    [r,f,d, rmvc, fmvc] = getFilenames(name);
    trial = getDataFromFiles(r, f, d, rmvc, fmvc);
    trials(i) = trial;
end

%%
%Calculate standard deviation and average for each trial filt data
clear stdDevs;
clear avgs;
for i = 1:numel(nameFolds)
    f = trials(i).yFilt;
    fTrim = f(1000:numel(f)-1000);
    stdDevs(i) = std(fTrim);
    avgs(i) = mean(fTrim);
end
   
%%
%plot raw, filt, and disp on figure
figure
for i = 1:numel(nameFolds)
    %
    trial = trials(i);
    yRaw = (trial.yRaw / max(trial.yMVC)) * 100;
    yFilt = (trial.yFilt / max(trial.yMVC)) * 100;
    subplot(numel(nameFolds)/2,2,i);
    %plot(data{1}, r, 'b')
    plot(trials(i).xFilt, yFilt, 'r')
    hold on
    ylim([0,60]);
    plot(trials(i).xDisp, trials(i).yDisp, 'g')
    plot([0 trials(i).xDisp(end)], [10 10], 'k'); %plot line through 10%
    hold off
    title(name);
     %plotEMGData(name, data{1}, data{2}, data{3}, data{4}, data{5}, data{6});
end

%%
%Plot Display Avg Vs Moving Avg
figure
plot(sortedVals, stdDevs, '-o');
title('Standard Deviation (red) and Mean (blue) by Window Size');
hold on;
plot(sortedVals, avgs, '-*');
hold off;

%%
%Plot display values against matlab's moving mean
figure
for i = 1:numel(nameFolds)
    matlabMean = movmean(trials(i).yFilt, sortedVals(i,1)/2);
    matlabMean = matlabMean / movmean(trials(i).yMVC, 500) * 100; %Scale matlab mean
    subplot(numel(nameFolds)/2, 2, i);
    title("Display values vs Matlab's moving average");
    plot(matlabMean, 'b');
    hold on;
    plot(trials(i).xDisp, trials(i).yDisp, 'r');
    hold off;
end
