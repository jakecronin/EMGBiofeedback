clear all;
a = csvread('DoubleRemoveWhitesFinishedDataFormatting/Trial02_FilteredData.csv');
cleanData = a(any(a,2),:); %removes rows of all zero or NAN data
x = cleanData(:,1);
y = cleanData(:,2);

subplot(2,1,1);
plot(x)
title("Make sure timestamps are continuous")
subplot(2,1,2);
plot(x,y);
title("Data")