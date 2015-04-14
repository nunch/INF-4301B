function hough_peaks_lines (Acc, RGB,votes);

[m,n] = size(Acc);
[mi,ni,k] = size(RGB);
%---------
% show Hough lines
%---------

im_show (RGB,'Detected lines');
hold on
Il = zeros(mi,ni);
maxrho = ceil(sqrt(mi^2 + ni^2));
for i=1:m-1,
    for j=1:n-1,
        if Acc(i,j)>votes,
            rho = j*2 - maxrho;
            angle = i;
            if angle>0
                  m = - cosd(angle)/sind(angle);
                  c = rho/sind(angle);
                  line ([1 ni],[m*1+c m*ni+c]);
            end;
        end;
    end;
end;
