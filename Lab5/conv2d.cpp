#include "conv2d.h"

BMP conv2d(const BMP &in, const dMatrix &kernel) {
    BMP out(in);
    int height=in.getHeight(),width=in.getWidth();
    int khalf=kernel.size()/2;//size() need to be even
    for(int i=khalf;i<height-khalf;i++){
        for(int j=khalf;j<width-khalf;j++){
            double sumr=0,sumg=0,sumb=0;
            for(int k=-khalf;k<=khalf;k++){
                for(int u=-khalf;u<=khalf;u++){
                    rgb pixel=in.getRGB(i+k,j+u);
                    double weight=kernel[k+khalf][u+khalf];
                    sumr+=pixel.r*weight;
                    sumg+=pixel.g*weight;
                    sumb+=pixel.b*weight;
                }
            }//solve convert double to uint8_t
            out.setRGB(i,j,rgb(BMP::solve(sumr),BMP::solve(sumg),BMP::solve(sumb)));
        }
    }
    return out;
}