#include"bilateral.h"
#include<iostream>
int main(){
    BMP in;
    string fileName="2236";
    in.read("./img/"+fileName+".bmp");
    int space=80,color=80;
    BMP out=bilateralFilter(in,15,space,color);
    out.write("./img/bilateral_s"+to_string(space)+"_c"+to_string(color)+fileName+".bmp");

}