function [list0] = MySobel(list1, scale, type)
%UNTITLED2 此处显示有关此函数的摘要
%   此处显示详细说明
rows = size(list1, 1);
cols = size(list1, 2);
list0 = zeros(rows, cols);
for i = 2 : rows -1
    for j = 2 : cols -1
        %计算水平差分
        A = abs((list1(i-1, j+1) + 2.0 * list1(i, j+ 1) + list1(i+1, j+ 1))-(list1(i-1, j-1) + 2.0 * list1(i, j-1)+list1(i + 1, j-1)));
        %计算垂直差分
        B = abs((list1(i+ 1, j-1) + 2.0 * list1( i + 1, j) + list1(i+1, j+ 1))-(list1(i-1, j-1) + 2.0 * list1(i - 1, j)+list1(i - 1, j + 1)));
        switch type
            case 'M'
                if A > B
                    C = A;
                else
                    C = B;
                end
              
            case 'H'
                C = A;
             
            case  'V'
                C = B;
             
            otherwise
                C = A + B;
        end 
        
        C = C / scale;
        if( C > 255.0)
            C = 255.0;
        end
        
        list0(i, j) = C;
    end
end

end

