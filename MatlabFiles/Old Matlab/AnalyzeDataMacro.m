dirname = 'Modified Data';
max = 7.613;        %needs to be recorded and read
data = PullDataFromDir(dirname, max);
testnames = fieldnames(data);

for i = 1:length(testnames)
    experiment = data.(testnames{i});
    PlotExperiment(experiment);
    xvalues = [100, 250, 500, 1000, 2000];
    PlotStabilityDataMacro(experiment, xvalues);
end
