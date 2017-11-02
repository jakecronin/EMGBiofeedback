
d = dir(pwd);
isub = [d(:).isdir]; %# returns logical vector
nameFolds = {d(isub).name}';
nameFolds(ismember(nameFolds,{'.','..'})) = [];

figure
for i = 1:numel(nameFolds)
    name = nameFolds{i};
    [r,f,d] = getFilenames(name);
    data = getDataFromFiles(r, f, d);    
    subplot(numel(nameFolds),1,i);
    plot(data{5}, data{6})
    ylim([0 20]);
    title(name);

    %plotEMGData(name, data{1}, data{2}, data{3}, data{4}, data{5}, data{6});
end

