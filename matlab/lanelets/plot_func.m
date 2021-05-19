%% plot_lanelet
figure(1); clf;hold on
plot(left_x, left_y, '-r');
plot(righ_x, righ_y, '-b');
plot_title = sprintf('Boundary for lanelet ID %d', laneletID);
title(plot_title)
legend('left boundary','right boundary','location', 'best' )