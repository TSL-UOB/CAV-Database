% -- select lanelet txt
filename = 'loop_geo_RHD.txt';

% -- import to matlab
lanelet = import_lanelet_txt(filename);

%% -- find the unique lanes
laneIDs = unique(lanelet.lanelet_id);

%% -- filter for lanelet ID
laneletID = 100;
sel = lanelet(laneletID==lanelet.lanelet_id,:);
left_x = sel.x(sel.bound=='leftBound',:);
left_y = sel.y(sel.bound=='leftBound',:);
righ_x = sel.x(sel.bound=='rightBound',:);
righ_y = sel.y(sel.bound=='rightBound',:);

figure(1), clf; hold on;
plot(left_x, left_y, '-r');
plot(righ_x, righ_y, '-b');

%% 

for index=1:numel(laneIDs)

laneletID = laneIDs(index);
sel = lanelet(laneletID==lanelet.lanelet_id,:);
left_x = sel.x(sel.bound=='leftBound',:);
left_y = sel.y(sel.bound=='leftBound',:);
righ_x = sel.x(sel.bound=='rightBound',:);
righ_y = sel.y(sel.bound=='rightBound',:);

figure(2); clf;hold on
plot(left_x, left_y, '-r');
plot(righ_x, righ_y, '-b');
plot_title = sprintf('Boundary for lanelet ID %d', laneletID);
title(plot_title)
legend('left boundary','right boundary','location', 'best' )
pause(2)
end

%% Coordinates for easy debugging
coords = [left_x, left_y;righ_x, righ_y];
