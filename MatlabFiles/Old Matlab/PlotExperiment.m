function [ plots ] = PlotExperiment( data )
%Plot Experiment, figure with plots from each test in experiment 

%create figure
f = figure();
testnames = fieldnames(data);
numtests = length(testnames);

plots = zeros(1,numtests); %initialize plot array
%go through each test and create a subplot for it
for i=1:numtests
    h = subplot(numtests,1,i);
    spacing = 0.03;
    plotheight = (0.98 - (spacing * (numtests+1))) / numtests;
    plotwidth = 0.9;
    set(h, 'Position', [0.05, 1 - i*(plotheight+spacing), plotwidth, plotheight]);
    test = data.(testnames{i});
    plots(i) = PlotTest(test);  %plot trials for this test on one subplot
    set(gca, 'XTick', []);  %remove x axis
    ylim([0,40]);       %scale axis
    l = refline(0,10);  %add target line
    l.Color = 'b';
    
    title(testnames{i});
    
   
end
end

