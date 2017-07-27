function [ tests ] = GetTests(directoryName )
%GetTrialNames return struct of all trial names for given testName
%   grabs strings after first instance of _
directory = dir(directoryName);
invalidFileNames = 0;
added = 0;
for i = 1:length(directory)
    file = directory(i);
    [filePath, fileName, extension] = fileparts(file.name);
    if (isvarname(fileName) == 0) | (extension ~= '.txt')
        invalidFileNames = invalidFileNames + 1;
        continue
    end
    testName = char(extractBefore(fileName,'_'));
    trialName = char(extractBetween(fileName, [testName,'_'], '_'));
    if isvarname(trialName) == 0
        trialName = strcat('Trial',trialName);
    end
    trialNumber = char(extractBetween(fileName, [trialName, '_'], '.txt'));
    if isvarname(trialNumber) == 0
        trialNumber = strcat('trialNumber', trialNumber);
    end
    if isvarname(testName) && isvarname(trialName) && isvarname(trialNumber)
        added = added+1;
        tests.(testName).(trialName).(trialNumber) = file;
    end
end
end

