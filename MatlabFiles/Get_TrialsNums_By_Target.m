log_filepath = '../LabviewFiles/Data/Phil Precision Testing/Log.csv';

log_data = csvread(log_filepath,1,0);
log_data = log_data(:,1);
