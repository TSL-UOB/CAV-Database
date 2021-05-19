%% export movie
tic; disp('exporting video...');

% save the video
v = VideoWriter(videoFileName);
v.FrameRate=10;
open(v)
writeVideo(v,F)
close(v)
disp('exporting video...complete'); toc;