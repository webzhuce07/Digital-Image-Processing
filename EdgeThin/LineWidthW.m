function [t] = LineWidthW(list, x, y, n)
%LINEWIDTHW 此处显示有关此函数的摘要
%   此处显示详细说明
Mask3 =[ 1, 0;
        1, -1;
        0, -1;
        -1, -1;
        -1, 0;
        -1, 1;
        0, 1;
        1, 1;
        1, 0];
    
t = 0;
for i = 1 : 9
    if(list(y + Mask3(i , 2), x + Mask3(i, 1)) >= n)
        t = t + 1;
        if( t > 1)
            break;
        end
    else
        t = 0;
    end
    
end

end

