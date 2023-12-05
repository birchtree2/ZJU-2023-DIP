#include"bmplib.h"
#include"conv2d.h"
const dMatrix simpleAverage={
    {1/9.,1/9.,1/9.},//9.->double
    {1/9.,1/9.,1/9.},
    {1/9.,1/9.,1/9.}
};
const dMatrix weightAverage={
    {1/16.,1/8.,1/16.},
    {1/8., 1/4.,1/8.},
    {1/16.,1/8.,1/16.}
};
const dMatrix Laplacian={
    {-1,-1,-1},
    {-1,8,-1},
    {-1,-1,-1}
};
BMP smoothing(const BMP& in,int kernelSize){
    dMatrix kernel;
    kernel.resize(kernelSize);
    for(int i=0;i<kernelSize;i++){
        for(int j=0;j<kernelSize;j++){
            kernel[i].push_back(1.0/(kernelSize*kernelSize));
        }
    }
    return conv2d(in,kernel);
}
BMP sharpening(const BMP &in,dMatrix kernel=Laplacian){
    kernel[1][1]+=1;//原图+拉普拉斯算子计算的结果
    return conv2d(in,kernel);
}
int main(){
    BMP in,out;
    string fileName="test2";
    in.read("./img/"+fileName+".bmp");
    out=smoothing(in,13);
    out.write("./img/smoothing_"+fileName+".bmp");
    // out=smoothing(in,weightAverage);
    // out.write("./img/smoothing2_"+fileName+".bmp");
    out=sharpening(in);
    out.write("./img/sharpening_"+fileName+".bmp");

}