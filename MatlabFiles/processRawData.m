%processRawData puts data from raw_filepath into processed_data cell array

%Needs raw_filepath and mvc
%Stores data in processed_data cell
%   {name}, {raw_data}, {filt_data}, {rect_data}, {smooth_data}

%Reads data from raw_filepath
%   Filteres data with butterworth bandpass
%   Rectifies data
%   Scales to mvc
%   Moothing with moving mean
%Saves 
close all;

%bandpass paramenters
fcutlow = 20;
fcuthigh = 499;
fs = 1000;
order = 4;

%smoothing parameters
movmean_width = 500; %500 samples at 1000hz = 500ms 

plot_data = false;

processed_data = cell(0);


%% Load Data
%raw_filepath = '../LabviewFiles/Data/Sistania_Precision/Trial06_RawData.csv';
raw_data = csvread(raw_filepath,1,0);



%% Process Data

%   CLEAN
    raw_data(~any(raw_data,2), : ) = [];  %Delete Empty Rows
    raw_data = raw_data(:,2); %Grab only relevant columns
    raw_data = raw_data(1000:end-1000,1); %Chop off first and last second of data

%     if size(raw_data,1) >= 8000
%         raw_data = raw_data(1000:8000,1);
%     elseif size(raw_data,1) > 7000
%         raw_data = raw_data(1:7001,1);
%     end

%   BANDPASS 20 - 500 hz 4th order
    %F = designfilt('bandpassiir','FilterOrder',2,'HalfPowerFrequency1',59,'HalfPowerFrequency2',61, 'DesignMethod','butter','SampleRate',1000);
    [b,a] = butter(order,[fcutlow,fcuthigh]/(fs/2), 'bandpass');
    filt_data = filter(b,a,raw_data);
    %filt_data = filter1('bp',raw_data,'fc',[20 500], 'fs',1000);

%   RECTIFY
    rect_data = abs(filt_data);

%   SCALE TO MVC (MUST HAVE AN MVC)
    rect_data = rect_data / mvc * 100 ;

%   LOWPASS FOR SMOOTHING at 5th order 10 hz
    %[b,a]=butter(5,10/1000,'low'); %5th order butterworth lowpass filter
    %smooth_data = filtfilt(b,a,rect_data);
    smooth_data = movmean(rect_data, movmean_width);
    %smooth_data = rect_data;
    
%   SAVE DATA
    processed_data = [{raw_filepath} {raw_data} {filt_data} {rect_data} {smooth_data}];


%% Plot Data

if plot_data
    r = 5;
    figure(1); hold on;
    subplot(r,1,1); plot(raw_data); title('raw data');
    subplot(r,1,2); plot(filt_data); title('filt data');
    subplot(r,1,3); plot(rect_data); title('rect data');
    subplot(r,1,4); plot(smooth_data); title('smooth data');
    subplot(r,1,5); histfit(smooth_data); title('histogram');
end



