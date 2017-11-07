function [ testNames ] = GetTestNames( directoryName )
%GetTestNames Returns array of unique file names
%   File names are the characters of filename up to first _ character

directory = dir(directoryName);

for i = 1:length(directory)
    file = directory(i);
    fileName = file.name;
    testName = char(extractBefore(fileName, '_'));
    testNames.(testName) = 'empty';
end
end

