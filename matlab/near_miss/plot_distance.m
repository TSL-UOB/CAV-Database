%% Load the database capture
load('postgres_distances','-mat')
%% plot the distance of each test case
figure(1); clf;
FontSize = 12;
plot(dist.safe,'k-','LineWidth',1);
hold on;
plot(dist.near,'k--','LineWidth',1);
plot(dist.crash,'k:','LineWidth',1);
plot([0 350],[.5 .5],'R--','LineWidth',1);
set(gca, 'YScale', 'log')
legend('safe', 'near miss', 'crash', 'Location', 'best')
ylabel('Euclidean Distance (m)')
xlabel('Simulation Time (s)')
x = [0.7 0.65];
y = [0.4 0.3];
annotation('textarrow',x,y,'String','Safe Limit')
