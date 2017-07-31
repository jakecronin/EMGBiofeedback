function [ tests ] = PullDataFromDir(directoryName, maxscale)
%PullDataFromdir return struct of all trial names for given testName
%   grabs strings after first instance of _
directory = dir(directoryName);
for i = 1:length(directory)
    file = directory(i);
    [filePath, fileName, extension] = fileparts(file.name);
    if (isvarname(fileName) == 0) | (extension ~= '.txt')
        continue;
    end
    parts = strsplit(fileName, '_');
    testname = char(parts(1));
    trialname = char(parts(2));
    trialnum = char(parts(length(parts)));
    if isvarname(testname)==0
        testname = strcat('test',testname);
    end
    if isvarname(trialname) == 0
        trialname = strcat('trial',trialname);
    end
    if isvarname(trialnum) == 0
        trialnum = strcat('number',trialnum);
    end
    data.('file') = file;
    fullfilename = fullfile(directoryName, file.name);
    data.('values') = (ValuesFromFile(fullfilename) / maxscale) * 100;
    tests.(testname).(trialname).(trialnum) = data;
end
end

