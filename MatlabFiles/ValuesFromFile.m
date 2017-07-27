function [ output_args ] = ValuesFromFile( fileName )
%Get Data From File as a struct
%Just read in values starting at line 9, and column 2
readtable(fileName, 'HeaderLines', 8) 


end

