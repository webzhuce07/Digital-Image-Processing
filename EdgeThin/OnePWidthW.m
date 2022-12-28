function [list0 ] = OnePWidthW(list1)
%ONEPWIDTHW 此处显示有关此函数的摘要
%   此处显示详细说明
rows = size(list1, 1);
cols = size(list1, 2);

list0 = zeros(rows, cols);

for i =  2 : rows - 2
    for j = 2 : cols - 2
        if list1(i, j) == 0   %非边界不处理
            continue;
        end
         n = 0;   %计数值初始化
         p = list1(i, j);   %取当前像素值
         
         for l = -1: 1: 1
             for k = -1 : 1 :1
                 if list1(i+l, j + k) >=p
                     n = n +1;
                 end
             end
         end
         
         if(n > 3)  %超过三点者
             t = LineWidthW(list1, j, i, p);  %检测线宽
         
             if t < 2
                 list0(i, j) = p;
             end
           
         elseif(n == 3)
             if list1(i , j - l) > p
                if(list1(i -l, j) >p ) || (list1(i + l, j) > p)
                    n = n -1;
                end
             elseif list1(i, j + l) > p
                 if(list1(i -l, j) >p ) || (list1(i + l, j) > p)
                    n = n -1;
                  end 
             end
             if n == 3 
                 list0(i, j) = p;
             end
             
         else
             list0(i, j) = p;
         
         end
         
    end
end

for i =  2 : rows - 2
    for j = 2 : cols - 2
        if list1(i, j) == 0
            continue;
        end
        
        n = 1;
        for l = -1: 1: 1
             for k = -1 : 1 :1
                 if list1(i+l, j + k) > 0
                     n = n +1;
                 end
             end
        end
        
         if n == 1
             list0(i,j) = 0;
         end
        
    end
end


end

