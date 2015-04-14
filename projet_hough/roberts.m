function [Igx,Igy] = roberts (I)

[m,n]=size(I);
I = double(I);
Igx = zeros(m,n);
Igy = zeros(m,n);

for i=1:m-1,
    for j=1:n-1,
        Igx(i,j) = -I(i,j) + I(i+1,j) - I(i,j+1) + I(i+1,j+1);
        Igy(i,j) = -I(i,j) + I(i,j+1) - I(i+1,j) + I(i+1,j+1);
    end;
end;