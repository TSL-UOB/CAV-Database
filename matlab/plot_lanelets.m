% -- select lanelet txt
filename = 'loop_geo_RHD.txt';

% -- import to matlab
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
coords = [left_x, left_y;righ_x, righ_y];
run('plot_func');

%% plot all 
for index=1:numel(laneIDs)

laneletID = laneIDs(index);
sel = lanelet(laneletID==lanelet.lanelet_id,:);
left_x = sel.x(sel.bound=='leftBound',:);
left_y = sel.y(sel.bound=='leftBound',:);
righ_x = sel.x(sel.bound=='rightBound',:);
righ_y = sel.y(sel.bound=='rightBound',:);
run('plot_func');
pause(2)
end

%% Coordinates for easy debugging
coords = [left_x, left_y;righ_x, righ_y];



