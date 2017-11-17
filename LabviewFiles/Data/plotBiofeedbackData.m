clear all;
a = csvread('Trial01_FilteredData.csv');
filtData = a(any(a,2),:);
x = filtData(:,1);
y = filtData(:,2);

plot(x,y);