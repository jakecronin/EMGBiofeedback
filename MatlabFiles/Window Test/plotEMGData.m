function [ ] = plotEMGData( name, xRaw, raw, xFilt, filt, xDisp, disp)
%plotEMGData Plot the data from getDataFromFiles along with a figure title

figure('NumberTitle', 'off', 'Name', name);
subplot(3,1,1);
plot(xRaw, raw);
title('Raw Data');

subplot(3,1,2);
plot(xFilt, filt);
title('Filtered Data');

subplot(3,1,3);
plot(xDisp, disp, 'r');
ylim([0 20]);
title('Display Data');

end

