close all;

src = imread("1.png");
list1 = MySobel(src, 1, 'M');

list0 = SobelThinning(list1);

listu = SobelThinning(list0);

listv = SobelThinning(listu);

listu = (list0 + listu + listv) ./3.0;

result = OnePWidthW(listu);