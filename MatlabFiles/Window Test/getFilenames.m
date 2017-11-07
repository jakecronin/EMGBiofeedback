function [ raw, filt, disp, rawmvc,filtmvc] = getFilenames( folder )
%getFilenamesFromDir takes a directory and pulls the filename for the raw
%data file, the filtered data file, and the display data file

listing = dir(folder);
rawEnd = 'RawData.txt';
filtEnd = 'FilteredData.txt';
dispEnd = 'DisplayData.csv';
filtMVCEnd = 'FilteredEMG.txt';
rawMVCEnd = 'RawEMG.txt';

    for i = 1:numel(listing)
        file = listing(i);
        name = file.name;
        if size(name) < 16
            continue;
        end
        if contains(name, rawEnd)
            raw = fullfile(folder, name);
        elseif contains(name, filtEnd)
            filt = fullfile(folder, name);
        elseif contains(name, dispEnd)
            disp = fullfile(folder, name);
        elseif contains(name, rawMVCEnd)
            rawmvc = fullfile(folder, name);
        elseif contains(name, filtMVCEnd)
            filtmvc = fullfile(folder, name);
        end
    end
end