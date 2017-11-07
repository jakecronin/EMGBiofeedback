function [ data ] = getDataFromFiles( rawFilename, filtFilename, dispFilename, rmvcFilename, fmvcFilename)
%getDataFromFiles takes strings of 3 filenames for raw, filtered, and
%display data. It returns a cell with the corresponding x,y data for each
%file
%return type:
% 3x1 cell{ name, x, y }

%Constants
targetChannel = 1;
headerLines = 6;

%1) GET RAW DATA
rawFid = fopen(rawFilename);
rawData = textscan(rawFid, '%s %s %f %f %f %f %f %f %f %f', 'headerlines', headerLines);
rawEMGValues = rawData{targetChannel + 2};
xRaw = todatenum(cdfepoch(rawData{2}));
xRaw = (xRaw - xRaw(1,1)) * 100000000;  %scale x values
rmvcFid = fopen(fmvcFilename); %scale y values
rmvcaData = textscan(rmvcFid, '%s %s %f %f %f %f %f %f %f %f', 'headerlines', headerLines);

%2) Get Filtered Data
filtFid = fopen(filtFilename);
filtData = textscan(filtFid, '%s %s %f %f %f %f %f %f %f %f', 'headerlines', headerLines);
filtEMGValues = filtData{targetChannel + 2};
xFiltered = datenum(filtData{2});
xFiltered = (xFiltered - xFiltered(1,1)) * 100000000; % scale x values

%3)Get Display Data
dispFid = fopen(dispFilename);
displayData = textscan(dispFid, '%f%s%s%s', 'delimiter', ',');
dispValues = displayData{1};      
%   scale disp data
%mvcFid = fopen(mvcFilename);
%mvcData = textscan(mvcFid, '%s %s %f %f %f %f %f %f %f %f', 'headerlines', headerLines);
%mvcValues = mvcData{targetChannel+2};
%mvcMax = max(mvcValues);
%dispValues = dispValues * mvcMax / 100;
%   get display x values
%frewind(filterFid);
%headerLine = fgets(filterFid);
%key = 'Frames Per Second: ';
%fpsIndex = strfind(headerLine, key);
%fps = sscanf(headerLine(fpsIndex(1) + length(key):end), '%g', 1);
xStart = xFiltered(1);
xEnd = xFiltered(end);
range = xEnd - xStart;
xDisp = [xStart:(range / numel(dispValues)):xEnd - 1];

disp(filtFilename);
filtStdDev = std(filtEMGValues);
disp(filtStdDev);
data = {xRaw, rawEMGValues, xFiltered, filtEMGValues, xDisp, dispValues};

end

