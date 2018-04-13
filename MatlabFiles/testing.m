close all
%Sistania trials 1 - 4 all bad

filt_filepath = '../LabviewFiles/Data/Sistania_Precision/Trial05_FilteredData.csv';
filt_data = csvread(filt_filepath,1,0);

raw_filepath = '../LabviewFiles/Data/Sistania_Precision/Trial05_RawData.csv';
raw_data = csvread(raw_filepath,1,0);

%% clean data
filt_data(~any(filt_data,2), : ) = [];  %Delete Empty Rows
filt_data = filt_data(:,2); %Grab only relevant columns

raw_data(~any(raw_data,2), : ) = [];  %Delete Empty Rows
raw_data = raw_data(:,2); %Grab only relevant columns


% normalize data
%filt_data = filt_data - mean(filt_data);
%raw_data = raw_data - mean(raw_data);


[pxx_filt,f_filt] = periodogram(filt_data,[],[],1000);
[pxx_raw,f_raw] = periodogram(raw_data,[],[],1000);


%% Custom filter raw data

%run raw data through lowpass filter
%D = designfilt('highpassiir', 'FilterOrder', 4,'PassbandFrequency', 20, 'PassbandRipple', 1, 'SampleRate', 1000);
%raw_data_filt = filter(D,raw_data);
[b,a]=butter(5,10/1000,'low'); %5th order butterworth lowpass filter
raw_data_filt = filtfilt(b,a,raw_data);
filt_data_filt = filtfilt(b,a,filt_data);

%Also do filter for 60hz
%d = designfilt('bandstopiir','FilterOrder',2,'HalfPowerFrequency1',59,'HalfPowerFrequency2',61, 'DesignMethod','butter','SampleRate',1000);
%raw_data_filt = filter(d,raw_data_filt);
%filt_data_filt = filtfilt(d,filt_data);


% rectify
raw_data_filt = abs(raw_data_filt);

%normalize
%raw_data_filt = raw_data_filt - mean(raw_data_filt);

figure(5); hold on;
subplot(4,1,1); plot(raw_data); title('raw data');
subplot(4,1,2); plot(filt_data); title('filtered data');
subplot(4,1,3); plot(filt_data_filt); title('filtered and refilt data');
subplot(4,1,4); plot(raw_data_filt); title('Custom filtered data');

[pxx_raw_filt,f_raw_filt] = periodogram(raw_data_filt,[],[],1000);

% 
% figure(1)
% hold on
% subplot(4,1,1);
% plot(f_filt,pxx_filt)
% ax = gca;
% ax.XLim = [0 1000];
% title('Labview Filtered Data')
% xlabel('Frequency hz')
% ylabel('Magnitude')
% 
% subplot(4,1,2);
% plot(f_raw,pxx_raw)
% ax = gca;
% ax.XLim = [0 1000];
% title('Raw data')
% xlabel('Frequency hz')
% ylabel('Magnitude')
% 
% 
% subplot(4,1,3);
% plot(f_raw_filt,pxx_raw_filt)
% ax = gca;
% ax.XLim = [0 1000];
% title('Raw data with custom filter')
% xlabel('Frequency hz')
% ylabel('Magnitude')
% 
% 
% figure(2)
% plot(raw_data_filt);









