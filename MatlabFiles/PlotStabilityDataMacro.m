function [ f1 ] = PlotStabilityDataMacro( data, xvals )
%UNTITLED9 Summary of this function goes here
%   Detailed explanation goes here
[testnames, means, stddevs, rootmeansquares] = GetStabilityData(data);
f1 = PlotStabilityData(testnames, xvals, means, stddevs, rootmeansquares);
end

