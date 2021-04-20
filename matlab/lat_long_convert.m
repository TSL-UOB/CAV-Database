% Convert X & Y to lat long
minlon = -2.499825; 
minlat = 51.489030;
latMid = 51.582838; 

% For the overtaking test case
% From AG openDrive file
minlat = 51.48458098 + 0.0015 + 0.00002;
minlon = -2.516507064 + 0.0043 - 0.00025;
latMid = 51.58458098;
% % From osm file
% % <bounds minlat="51.4784000" minlon="-2.5263000" maxlat="51.4942000" maxlon="-2.4991000"/>
% minlat=51.4784000;
% minlon=-2.5263000;
% maxlat=51.4942000;
% maxlon=-2.4991000;
% latMid= (maxlat - minlat)/2;

m_per_deg_lat = 111132.92 - 559.82*cos(2*latMid*pi/180) + ...
    1.175*cos(4*latMid*pi/180) - 0.0023*cos(6*latMid*pi/180);
m_per_deg_lon = 111412.84*cos(latMid*pi/180) - 93.5*cos(3*latMid*pi/180)...
    + 0.118*cos(5*latMid*pi/180);