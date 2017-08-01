function [ testnames, means, stddevs, rootmeansquares ] = GetStabilityData( data )
%GetStabilityData Summary of this function goes here
%   Detailed explanation goes here
testnames = fieldnames(data);
numtests = length(testnames);

means = [];
stddevs = [];
rootmeansquares = [];
for i=1:numtests
    test = data.(testnames{i});
    trialnames = fieldnames(test);
    numtrials = length(trialnames);
    allvalues = [];
    for j=1:numtrials
         trial = test.(trialnames{j});
         values = trial.values;
         allvalues = cat(1, allvalues, values);
    end
    rootmeansquares = cat(1, rootmeansquares, rms(allvalues));
    means = cat(1, means, mean(allvalues));
    stddevs = cat(1, stddevs, std2(allvalues));
end

