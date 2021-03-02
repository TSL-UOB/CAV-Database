% Convert X & Y to lat long
minlon = -2.499825; 
minlat = 51.489030;
latMid = 51.582838; 

m_per_deg_lat = 111132.92 - 559.82*cos(2*latMid*pi/180) + ...
    1.175*cos(4*latMid*pi/180) - 0.0023*cos(6*latMid*pi/180);
m_per_deg_lon = 111412.84*cos(latMid*pi/180) - 93.5*cos(3*latMid*pi/180)...
    + 0.118*cos(5*latMid*pi/180);
