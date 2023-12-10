#include"bilateral.h"
#include<iostream>
int main(){
    BMP in;
    string fileName="test";
    in.read("./img/"+fileName+".bmp");
    int space=80,color=80;
    while(1){
        cin>>space>>color;
        BMP out=bilateralFilter(in,15,space,color);
        out.write("./img/bilateral_s"+to_string(space)+"_c"+to_string(color)+
        "_"+fileName+".bmp");//add param in filename for comparison
    }
    

}