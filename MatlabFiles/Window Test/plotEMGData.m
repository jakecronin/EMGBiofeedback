function [ ] = plotEMGData( name, xRaw, raw, xFilt, filt, xDisp, disp)
%plotEMGData Plot the data from getDataFromFiles along with a figure title

figure
title(name);
subplot(3,1,1);
plot(xRaw, raw);

subplot(3,1,2);
plot(xFilt, filt);

subplot(3,1,3);
plot(xDisp, disp, 'r');
ylim([0 20]);

end

