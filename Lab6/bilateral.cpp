#include"bmplib.h"
#include<cmath>
#include<iostream>
#include "bilateral.h"
BMP bilateralFilter(const BMP& in,int kernelSize,double spaceSigma,double colorSigma){
    BMP out(in);
    int height=in.getHeight(),width=in.getWidth();
    int radius=kernelSize/2;
    int maxColor=256;
    vector<double>colorWeights(maxColor*3);
    vector<double>spaceWeights(kernelSize*kernelSize*2);
    for(int i=0;i<maxColor*3;i++){//因为是把r,g,b大小的差值相加,所以距离最大为3&2
        colorWeights[i]=exp(-i*i/(0.5*colorSigma*colorSigma));//exp(-x/2σ^2)
    }
    for(int i=-radius;i<=radius;i++){
        for(int j=-radius;j<=radius;j++){
            int d=i*i+j*j;//square of distance
            //[i+radius,j+radius]
            int offset=(i+radius)*(2*radius+1)+j+radius;
            spaceWeights[offset]=exp(-d/(0.5*spaceSigma*spaceSigma));
        }
    }
    for(int x=radius;x<height-radius;x++){
        for(int y=radius;y<width-radius;y++){
            rgb pixel=in.getRGB(x,y);//center pixel
            double sumr=0,sumg=0,sumb=0,sumw=0;
            for(int i=-radius;i<=radius;i++){
                for(int j=-radius;j<=radius;j++){
                    rgb q=in.getRGB(x+i,y+j);
                    int spaceDist=i*i+j*j;
                    int offset=(i+radius)*(2*radius+1)+j+radius;
                    //spacial distance between (i,j) and (x+i,y+j)
                    int colorDist=abs((int)q.r-(int)pixel.r)+abs((int)q.b-(int)pixel.b)+abs((int)q.g-(int)pixel.g);
                    //color distance of pixel(i,j) and pixel(x+i,y+j)
                    double weight=spaceWeights[offset]*colorWeights[colorDist];
                    sumr+=q.r*weight;
                    sumg+=q.g*weight;
                    sumb+=q.b*weight;
                    sumw+=weight;
                }
            }
            sumr/=sumw;sumg/=sumw;sumb/=sumw;//最后记得做归一化
            out.setRGB(x,y,rgb(BMP::solve(sumr),BMP::solve(sumg),BMP::solve(sumb)));
        }
    }
    return out;
}