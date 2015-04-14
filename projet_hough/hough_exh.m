function Acc = hough_exh(Ib)

[m,n]=size (Ib);
m2 = round(m/2);
n2 = round(n/2);
maxrho = (ceil(sqrt(m^2 + n^2)));
% Accumulator initialization
Acc = zeros (180,maxrho);
% Compute Hough transform for every Ib(x,y)>0
for i=2:m-1,
    for j=2:n-1,
        if Ib(i,j) > 0,
            for angle = 1:180,
               rho = (j)*cosd(angle) + (i)*sind(angle);
               rho_idx = round(rho/2 + maxrho/2);
               %if rho_idx ~= 0,
               if rho_idx > 0
               Acc(angle,rho_idx) = Acc(angle,rho_idx) + 1;
               else if rho_idx ~=0
                     Acc(angle,rho_idx+maxrho) = Acc(angle,rho_idx+maxrho) + 1;
                 end;
               end;
            end;
        end;
    end;
end;