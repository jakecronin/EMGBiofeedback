function [ mvcs ] = getMVCValue( filename )
%Get MVC Value

fid = fopen(filename);
data = textscan(fid, '%s %s %f %f %f %f %f %f %f %f', 'headerlines', 6);

mvcs = zeros(8,1);

for i = 1:8
    vals = data{i+2};
    %M = movmean(vals, 500); %0.5 second moving average
    %mvcs(i) = max(M);
    mvcs(i) = max(vals);
end

end
