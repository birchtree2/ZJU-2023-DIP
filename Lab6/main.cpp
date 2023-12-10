#include"bilateral.h"
#include<iostream>
int main(){
    BMP in;
    string fileName="2236";
    in.read("./img/"+fileName+".bmp");
    BMP out=bilateralFilter(in,15,80,80);
    out.write("./img/bilateral_"+fileName+".bmp");

}