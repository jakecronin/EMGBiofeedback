
d = dir(pwd);
isub = [d(:).isdir]; %# returns logical vector
nameFolds = {d(isub).name}';
nameFolds(ismember(nameFolds,{'.','..'})) = [];

for i = 1:numel(nameFolds)
    name = nameFolds{1};
    [r,f,d] = getFilenames(name);
    data = getDataFromFiles(r, f, d);
    plotEMGData(name, data{1}, data{2}, data{3}, data{4}, data{5}, data{6});
    
end