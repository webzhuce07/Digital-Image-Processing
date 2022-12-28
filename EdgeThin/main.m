close all;

src = imread("test.jpg");
src = rgb2gray(src);
list1 = MySobel(src, 1, 'M');


imshow(uint8(list1));

list0 = SobelThinning(list1);

listu = SobelThinning(list0);

listv = SobelThinning(listu);

listu = (list0 + listu + listv) ./3.0;

result = OnePWidthW(listu);
imwrite(result,"r.png");

figure;
imshow(uint8(result));