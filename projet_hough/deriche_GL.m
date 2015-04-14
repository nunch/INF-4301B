% Garcia Lorka Optimization of Deriche filter 
% I - input image (gray level)
% gamma - smoothing parameter
% Ia - output image
function [Ia] = deriche_GL(I,gamma)

g = gamma;
g1 = (1-g)^2;
g2 = 2*g;
gg = g*g;

% Image size
[m,n] = size(I);

Ic = double(I);
Ia = double(I);
% Horizontal smoothing
for i=1:m, % for every line :
    % Filtre causal
    Ic(i,1) = g1*I(i,1);
    Ic(i,2) = g1*I(i,2)+ (g2 * Ic(i,1));
    for j=3:n,
       Ic(i,j)= (g1*I(i,j)) + (g2 * Ic(i,j-1)) + (gg*Ic(i,j-2));
    end;
    % Filtre anticausal
        Ia(i,n) = g1*Ic(i,n);
        Ia(i,n-1) = g1*Ic(i,n-1) + g2*Ia(i,n);
    for j=n-2:-1:1
        Ia(i,j) = g1*Ic(i,j) + g2*Ia(i,j+1) + gg*Ia(i,j+2);
    end;
end;

% Vertical smoothing
I = Ia';
[m,n] = size (I);
Ic = I;
Ia = I;
% Horizontal smoothing
for i=1:m, % for every line :
    % Filtre causal
    Ic(i,1) = g1*I(i,1);
    Ic(i,2) = g1*I(i,2)+ (g2 * Ic(i,1));
    for j=3:n,
       Ic(i,j)= (g1*I(i,j)) + (g2 * Ic(i,j-1)) + (gg*Ic(i,j-2));
    end;
    % Filtre anticausal
        Ia(i,n) = g1*Ic(i,n);
        Ia(i,n-1) = g1*Ic(i,n-1) + g2*Ia(i,n);
    for j=n-2:-1:1
        Ia(i,j) = g1*Ic(i,j) + g2*Ia(i,j+1) + gg*Ia(i,j+2);
    end;
end;

Ia = Ia';