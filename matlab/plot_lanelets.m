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
    xlim([min(coords(:,1)) max(coords(:,1))]);
    ylim([min(coords(:,2)) max(coords(:,2))]);
    pause(0.05)
    plot_title = sprintf('Boundary for lanelet ID %d', laneletID);
    title(plot_title)    
    F(index) = getframe(gcf); %animate  
    if index==numel(coords(:,1)) %persist last frame
        F(index+1) = F(index);
        F(index+2) = F(index);
        F(index+3) = F(index);
    end
end

videoTag = strcat('Lanelet_ani_',num2str(laneletID));
videoFileName = sprintf('%s_%s',videoTag,datestr(now,'HH_MM_SS'));
exportMovie

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

%% GPS String
lat_long_convert %get the conversion ratios
GPS = zeros(numel(coords(:,1)),2);
GPS_str = '';
%assuming XY data is in meters
for index=1:numel(coords(:,1))
    x = coords(index,1);
    y = coords(index,2);
    GPS(index,1) = x/m_per_deg_lon + minlon;
    GPS(index,2) = y/m_per_deg_lat + minlat;
    if index < numel(coords(:,1))
        GPS_str = strcat(GPS_str,sprintf(' %15.12f %15.12f,',GPS(index,1),GPS(index,2)));
    else
        GPS_str = strcat(GPS_str,sprintf(' %15.12f %15.12f' ,GPS(index,1),GPS(index,2)));
    end
end

%% Generate SQL string for POSTGIS
top = "SELECT ST_GeomFromText('POLYGON((";
tail = "))')";
SQL_query = strcat(top, GPS_str, tail);

