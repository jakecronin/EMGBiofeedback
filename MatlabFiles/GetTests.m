function [ tests ] = GetTests(directoryName )
%GetTrialNames return struct of all trial names for given testName
%   grabs strings after first instance of _
directory = dir(directoryName);
for i = 1:length(directory)
    file = directory(i);
    fileName = char(extractBetween(file.name, '"', '"'));
    if isvarname(fileName) == 0
        continue
    end
    testName = char(extractBefore(fileName,'_'));
    trialName = char(extractBetween(fileName, [testName,'_'], '_'));
    trialNumber = char(['trial ', extractBetween(fileName, [trialName, '_'], '.txt')]);
    if isvarname(testName) && isvarname(trialName) && isvarname(trialNumber)
    tests.(testname).(trialName).(trialNumber) = file;
    end
end
end

