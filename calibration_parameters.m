clc, clear,close all
numImagePairs = 40;
imageFiles1 = cell(numImagePairs, 1);
imageFiles2 = cell(numImagePairs, 1);
imageDir = fullfile('C:', 'Users', 'Tomáš','Desktop', ...
    'Nový prieèinok','snimky', 'snimky');
for i = 1:numImagePairs
    imageFiles1{i} = fullfile(imageDir, sprintf('image_left_%01d.png', i));
    imageFiles2{i} = fullfile(imageDir, sprintf('image_right_%01d.png', i));
end

images1 = cast([], 'uint8');
images2 = cast([], 'uint8');
for i = 1:numel(imageFiles1)
    im = imread(imageFiles1{i});
    images1(:, :, :, i) = im;

    im = imread(imageFiles2{i});
    images2(:, :, :, i) = im;
end

[imagePoints, boardSize] = detectCheckerboardPoints(images1, images2);

% Display one masked image with the correctly detected checkerboard
figure;
imshow(images1(:,:,:,1), 'InitialMagnification', 50);
hold on;
plot(imagePoints(:, 1, 1, 1), imagePoints(:, 2, 1, 1), '*-g');
title('Successful Checkerboard Detection');

% Generate world coordinates of the checkerboard points.
squareSize = 25; % millimeters
worldPoints = generateCheckerboardPoints(boardSize, squareSize);

% Compute the stereo camera parameters.
stereoParams = estimateCameraParameters(imagePoints, worldPoints);

% Evaluate calibration accuracy.
figure;
showReprojectionErrors(stereoParams);