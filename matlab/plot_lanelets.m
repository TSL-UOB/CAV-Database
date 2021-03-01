%% -- select lanelet txt
filename = 'loop_geo_RHD.txt';

%% -- import to matlab
lanelet = import_lanelet_txt(filename);

%% -- find the unique lanes
laneIDs = unique(lanelet.lanelet_id);

%% -- filter for lanelet ID + plot
laneletID = 112;
sel = lanelet(laneletID==lanelet.lanelet_id,:);
left_x = sel.x(sel.bound=='leftBound',:);
left_y = sel.y(sel.bound=='leftBound',:);
righ_x = sel.x(sel.bound=='rightBound',:);
righ_y = sel.y(sel.bound=='rightBound',:);
run('plot_func');
% Coordinates for easy debugging
coords = [left_x, left_y;flip(righ_x), flip(righ_y); left_x(1), left_y(1)];

%% plot by point
figure(1); clf;hold on
for index=1:numel(coords(:,1))
    x = coords(1:index,1);
    y = coords(1:index,2);
    
    plot(x, y, '-k');
    pause(0.1)
    plot_title = sprintf('Boundary for lanelet ID %d', laneletID);
    title(plot_title)
    legend('outer boundary','location', 'best')
end

%% plot all
for index=1:numel(laneIDs)
laneletID = laneIDs(index);
sel = lanelet(laneletID==lanelet.lanelet_id,:);
left_x = sel.x(sel.bound=='leftBound',:);
left_y = sel.y(sel.bound=='leftBound',:);
righ_x = sel.x(sel.bound=='rightBound',:);
righ_y = sel.y(sel.bound=='rightBound',:);
run('plot_func');
pause(1)
end

