function [list0 ] = SobelThinning(list1)
%SOBELTHINNING 此处显示有关此函数的摘要
%   此处显示详细说明
list0 = MySobel(list1, 1, 'M');
rows = size(list1, 1);
cols = size(list1, 2);

for i = 2 :  rows - 3
    for j =  2 : cols - 3
        g = list1(i,j) - list0(i, j); 
        if ((list1(i, j- 1) + list1(i, j+ 1) == 0) || (list1(i -1, j) + list1(i + 1, j) == 0))
            list0(i, j) = list1(i, j);
        else
            if g < 0
                list0(i, j) = 0;
            else
                list0(i, j) = g;
            end
        end
    end
end

end

