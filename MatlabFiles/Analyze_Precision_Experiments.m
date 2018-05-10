%Analyze_Precision_Experiments looks at precision testing and bin testing
%from trials with 6 subjects (Sistania, Brian, Phil, Ray, Marchand, and
%Antonio

%Purpose: 
%   1) Show precision in distinguishing between activation
%   percentages for subjects given an activation target
%   
%   2) Check how precision changes as biofeedback becomes more or less
%   sensitive


%% Pull data from each subjet's trials

% Steps:
%   1) get MVC for scaling
%   2) get data from each trial into cell array (subjects X 6)
%   [{name} {filename} {raw} {filt} etc
%   {name}                              ]

% SET SOME PARAMETERS
    plot_combined = true;
    plot_individuals = true;
    hist_precision = 20;
    
    remove_bad_subjects = true;
    
    
 %% SISTANIA
    sistania_data = cell(0);
    raw_filepath = '../LabviewFiles/Data/Sistania_Precision/MVC 1RawEMG.csv';
    windowWidth = 500; mvc = 100; %samples
    processRawData;
    mvc = max(smooth_data);
    mvc = 2.185779; %Hard code mvc from logfile. Original MVC file lost
    trial_numbers = [5 16 20; 6 12 18; 7 15 17; 8 11 21; 9 14 19; 10 13 22]; %5 8 10 12 15 20% activation
    %Run through trials for each target
    for subject = 1:size(trial_numbers,1)
        for j = 1:size(trial_numbers,2)
            trialNum = trial_numbers(subject,j);
            if trialNum > 9
                raw_filepath = ['../LabviewFiles/Data/Sistania_Precision/Trial' num2str(trialNum) '_RawData.csv'];
            else
                raw_filepath = ['../LabviewFiles/Data/Sistania_Precision/Trial0' num2str(trialNum) '_RawData.csv'];
            end
            processRawData;
            sistania_data(subject,j) = {processed_data};
        end
    end

%% BRIAN 
    brian_data = cell(0);
    raw_filepath = '../LabviewFiles/Data/Default/MVC 5RawEMG.csv';
    windowWidth = 500; mvc = 100; %samples
    processRawData;
    mvc = max(smooth_data);
    trial_numbers = [1 2 3; 4 5 6; 7 8 9; 10 11 12; 13 14 15; 16 17 18];%5, 8, 10, 12, 15, 20
    %Run through trials for each target
    for subject = 1:size(trial_numbers,1)
        for j = 1:size(trial_numbers,2)
            trialNum = trial_numbers(subject,j);
            if trialNum > 9
                raw_filepath = ['../LabviewFiles/Data/Default/Trial' num2str(trialNum) '_RawData.csv'];
            else
                raw_filepath = ['../LabviewFiles/Data/Default/Trial0' num2str(trialNum) '_RawData.csv'];
            end
            processRawData;
            brian_data(subject,j) = {processed_data};
        end
    end
    
%% Phil
    phil_data = cell(0);
    raw_filepath = '../LabviewFiles/Data/Phil Precision Testing/MVC 2RawEMG.csv';
    windowWidth = 500; mvc = 100;
    processRawData;
    mvc = max(smooth_data);
    trial_numbers = [2 12 18; 4 9 14; 6 10 16; 7 11 17; 3 8 15; 5 13 19];
    %Run through trials for each target
    for subject = 1:size(trial_numbers,1)
        for j = 1:size(trial_numbers,2)
            trialNum = trial_numbers(subject,j);
            if trialNum > 9
                raw_filepath = ['../LabviewFiles/Data/Phil Precision Testing/Trial' num2str(trialNum) '_RawData.csv'];
            else
                raw_filepath = ['../LabviewFiles/Data/Phil Precision Testing/Trial0' num2str(trialNum) '_RawData.csv'];
            end
            processRawData;
            phil_data(subject,j) = {processed_data};
        end
    end
    
%% Ray
    ray_data = cell(0);
    raw_filepath = '../LabviewFiles/Data/Ray Precision Testing/MVC 6RawEMG.csv';
    windowWidth = 500; mvc = 100;
    processRawData;
    mvc = max(smooth_data);
    trial_numbers = [18 20 28; 16 24 27; 19 25 26; 14 21 0; 17 22 29; 15 23 30 ];
    %Run through trials for each target
    for subject = 1:size(trial_numbers,1)
        for j = 1:size(trial_numbers,2)
            trialNum = trial_numbers(subject,j);
            if trialNum == 0
                continue;
            elseif trialNum > 9
                raw_filepath = ['../LabviewFiles/Data/Ray Precision Testing/Trial' num2str(trialNum) '_RawData.csv'];
            else
                raw_filepath = ['../LabviewFiles/Data/Ray Precision Testing/Trial0' num2str(trialNum) '_RawData.csv'];
            end
            processRawData;
            ray_data(subject,j) = {processed_data};
        end
    end
    
%% Antonio
    antonio_data = cell(0);
    raw_filepath = '../LabviewFiles/Data/Antonio Bin Testing/MVC 6RawEMG.csv';
    windowWidth = 500; mvc = 100;
    processRawData;
    mvc = max(smooth_data);
    trial_numbers = [3 12 16; 5 11 20; 7 13 18; 6 14 15; 4 9 19; 8 10 17];
    %Run through trials for each target
    for subject = 1:size(trial_numbers,1)
        for j = 1:size(trial_numbers,2)
            trialNum = trial_numbers(subject,j);
            if trialNum == 0
                continue;
            elseif trialNum > 9
                raw_filepath = ['../LabviewFiles/Data/Antonio Bin Testing/Trial' num2str(trialNum) '_RawData.csv'];
            else
                raw_filepath = ['../LabviewFiles/Data/Antonio Bin Testing/Trial0' num2str(trialNum) '_RawData.csv'];
            end
            processRawData;
            antonio_data(subject,j) = {processed_data};
        end
    end

 %% Marchand
    marchand_data = cell(0);
    raw_filepath = '../LabviewFiles/Data/Marchand Bin Testing/MVC 2RawEMG.csv';
    windowWidth = 500; mvc = 100;
    processRawData;
    mvc = max(smooth_data);
    trial_numbers = [3 8 17; 1 12 16; 6 11 14; 4 9 15; 5 10 18; 2 7 13];
    %Run through trials for each target
    for subject = 1:size(trial_numbers,1) %
        for j = 1:size(trial_numbers,2)
            trialNum = trial_numbers(subject,j);
            if trialNum == 0
                continue;
            elseif trialNum > 9
                raw_filepath = ['../LabviewFiles/Data/Marchand Bin Testing/Trial' num2str(trialNum) '_RawData.csv'];
            else
                raw_filepath = ['../LabviewFiles/Data/Marchand Bin Testing/Trial0' num2str(trialNum) '_RawData.csv'];
            end
            processRawData;
            marchand_data(subject,j) = {processed_data};
        end
    end
    
    
    
%% Consolidate Data

% put all subject data into cell
all_subjects = [{sistania_data} {brian_data} {phil_data} {antonio_data} {ray_data} {marchand_data}];

for subject = 1:size(all_subjects,2)
    subject_data = all_subjects{1,subject}; %activations X repeats cell array
    for row = 1:size(subject_data,1) %one target activation at a time
        combined = subject_data{row,1}(1,2:end); %represents one trial. combine with repeats of same target activation
        for trial = 2:size(subject_data,2)
            this_trial = subject_data{row,trial}; % a 1X5 cell array
            for data_type = 2:size(this_trial,2) %skip the first cell (the name)
                combined{1,data_type-1} = [combined{1,data_type-1}; this_trial{1,data_type}];
            end
        end
        target_data_by_subject(row,subject) = {combined};
    end
end

%consolidate data from all subjects
target_data(:,1) = target_data_by_subject(:,1); %activations X 1 cell array
for row = 1:size(target_data_by_subject,2)
    for subject = 2:size(target_data_by_subject,2)
        if remove_bad_subjects && (subject == 4 || subject == 6)
            continue;
        end
        cell_of_interest = target_data_by_subject{row,subject};
        for data_type = 1:size(cell_of_interest,2)
            target_data{row,1}{1,data_type}= [target_data{row,1}{1,data_type};...
            cell_of_interest{1,data_type}];
        end
        
    end
end

%% Plot a boxplot figure with each subjects results

for subject = 1:size(target_data_by_subject,2)
    min = size(target_data_by_subject{1,subject}{1,4},1);
    for target = 1:size(target_data_by_subject,1) %get min bottleneck
        if size(target_data_by_subject{target,subject}{1,4},1) < min
            min = size(target_data_by_subject{target,subject}{1,4},1);
        end
    end
    %construct array
    clear x;
    for target = 1:size(target_data_by_subject,1)
        x(:,target) = target_data_by_subject{target,subject}{1,4}(1:min,1);
    end
    all_subject_data(subject) = {x};
end

figure(); hold on;
names = ["Sistania", "Brian","Phil", "Antonio", "Ray", "Daniel"];
activations = ["5%", "8%", "10%", "12%", "15%", "20%"];
for i = 1:size(all_subject_data,2)
    subplot(2,6,i)
    boxplot(all_subject_data{1,i},activations);
    axis([0 7 0 30]);
    title(names(1,i));
    m = mean(all_subject_data{1,i});
    for j = 1:size(activations,2)
        text(j,m(j),num2str(m(j)),'HorizontalAlignment','left');
    end
    
    %Add Histograms
    subplot(2,6,i+size(all_subject_data,2)); hold on;
    for target = 1:size(all_subject_data{1,i},2)
        histfit(all_subject_data{1,i}(:,target), hist_precision);
        plot(m(target), 0, '*r');
        axis([0 30 0 10000])
        text(m(target),-80,num2str(m(target)),'VerticalAlignment','top','Rotation', -45, 'Color', 'r');
    end
end





%% Build boxplot with combined results

if plot_combined
    % Build array of data for each target activation
    %   1) get limiting trial duration
    min = size(target_data{1,1}{1,4},1);
    for t = 1:size(target_data,1)
        if size(target_data{t,1}{1,4},1) < min
            min = size(target_data{t,1}{1,4},1);
        end
    end

    %   2) Build array of trimmed data
    clear data_array;
    for t = 1:size(target_data,1)
        data_array(:,t) = target_data{t,1}{1,4}(1:min,1);
    end

    %   3) Make Boxplot
    figure();
    hold on
    activations = ["5%", "8%", "10%", "12%", "15%", "20%"];
    boxplot(data_array, activations);
    title('EMG Activation Precision, 4 Subjects, Three 10s Trials Per Target');
    ylabel('Percent of MVC')
    xlabel('Target Activation')

    m = mean(data_array);
    for i = 1:size(activations,2)
        text(i,m(i),num2str(m(i)),'HorizontalAlignment','left');
    end 
end

%% Get Precision and Accuracy


accuracy = m - [5 8 10 12 15 20]
precision = std(data_array)
