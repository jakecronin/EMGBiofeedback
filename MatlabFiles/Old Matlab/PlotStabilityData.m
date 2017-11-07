function [ f2 ] = PlotStabilityData( trialnames, xvals, means, stddevs, rootmeansquares )
%PlotStabilityData
%   Detailed explanation goes here
f2 = figure();

scatter(xvals, means);
hold on;
scatter(xvals, stddevs);
scatter(xvals, (stddevs - means));
scatter(xvals, rootmeansquares);
legend('Means', 'Std Devs', 'StdDevs - Means', 'root mean squares');
title('Average Mean and Standard Deviation for Each Paramenter');
ylabel('Percent MVC Activation');
xlabel('Moving Window Size (ms)')
end

