#include"bilateral.h"
#include<iostream>
int main(){
    BMP in;
    string fileName="test2";
    in.read("./img/"+fileName+".bmp");
    BMP out=bilateralFilter(in,5,18,0.1);
    out.write("./img/bilateral_"+fileName+".bmp");

}