% Read in the stereo pair of images.
clc,close all, clear all
load('matlab.mat');
I1 = imread('scenaL.png');
I2 = imread('scenaR.png');

% Rectify the images.
[J1, J2] = rectifyStereoImages(I1, I2, stereoParams);

% Display the images before rectification.
figure;
h=imshow(stereoAnaglyph(I1, I2), 'InitialMagnification', 50);
title('Before Rectification');
saveas(h,'Before Rectification','png')
% Display the images after rectification.
figure;
h=imshow(stereoAnaglyph(J1, J2), 'InitialMagnification', 50);
title('After Rectification');
saveas(h,'After Rectification','png')
disparityMap = disparity(rgb2gray(J1), rgb2gray(J2));
figure;
h=imshow(disparityMap, [0, 64], 'InitialMagnification', 50);
% colormap('jet');
colorbar;
title('Disparity Map');
saveas(h,'Disparity Map','png')
pointCloud = reconstructScene(disparityMap, stereoParams);
Z = pointCloud(:, :, 3);
mask = repmat(Z > 0 & Z < 1500, [1, 1, 3]);
J1(~mask) = 0;
imshow(J1, 'InitialMagnification', 50);
save('Afterdisp.mat');