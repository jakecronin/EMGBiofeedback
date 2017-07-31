function [ f2 ] = PlotStabilityData( trialnames, yvals, means, stddevs )
%PlotStabilityData
%   Detailed explanation goes here
f2 = figure();

scatter(yvals, means);
hold on;
scatter(yvals, stddevs);
scatter(yvals, (stddevs - means));
legend('Means', 'Std Devs', 'StdDevs - Means');
title('Average Mean and Standard Deviation for Each Paramenter');
ylabel('Percent MVC Activation');
xlabel('Moving Window Size (ms)');
end

