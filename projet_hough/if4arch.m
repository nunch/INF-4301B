close all;
clear all;
% load image and convert it in gray levels
%RGB  = imread('circuit.bmp');
RGB = imread('out.png');
I  = rgb2gray(RGB); % convert to intensity
%---------
% show I
%---------
im_show(I,'Original image');

%-------------------------------
% define application constants
%-------------------------------
gamma = 0.2
th  = 160;
votes = 100;

%---------------------------------------------
% Deriche filter (Gracia Lorca Optimization)
%---------------------------------------------
% smoothing
Is = smoothing_GL(I,gamma);
% Roberts gradient
[Igx,Igy] = roberts(Is);
Ig = sqrt(Igx.^2 + Igy.^2);
%---------
% show Is & Is 
%---------
im_show(Is,'Smoothed image');
im_show(Ig,'Gradient');

%---------------------------------------------
% Contours detection by binarization
%---------------------------------------------
Ib = Ig > th;
%---------
% show Ib
%---------
im_show(Ib, 'Binary contours');

%----------------------------------------------
% Hough transform
%----------------------------------------------
Acc = hough_exh (Ib);

%---------
% show Acc
%---------
im_show(Acc','Houhgh accumulator space');

%----------------------------------------------
% Hough peak detection and line drawing
%----------------------------------------------
% optional peak number reduction
Accmax = imregionalmax(Acc);
Accb = (Accmax.*Acc);
%
hough_peaks_lines(Accb,RGB,votes);
