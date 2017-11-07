function [ values ] = ValuesFromFile( fileName )
%Get Data From File as a struct
%Just rfileead in values starting at line 9, and column 2
table = readtable(fileName, 'HeaderLines', 8, 'Format', '%s%s%s%s%s%s%s%s%s'); 
values = table.(2);
values = cell2mat(values);
values = str2num(values);
end

