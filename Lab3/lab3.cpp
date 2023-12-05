#include"bmplib.h"
#include<cmath>
#include<cstring>
BMP Enhance(const BMP &in){
    BMP out(in);
    double Lmax=0;
    for(int i=0;i<in.getHeight();i++){
        for(int j=0;j<in.getWidth();j++){
            //find the maximum value of lumin Y
            Lmax=max(Lmax,in.rgb2yuv(in.getRGB(i,j)).y);
        }
    }
    for(int i=0;i<in.getHeight();i++){
        for(int j=0;j<in.getWidth();j++){
            yuv lu=in.rgb2yuv(in.getRGB(i,j));
            lu.y=log(lu.y+1)/log(Lmax+1)*255;//logarithm
            out.setRGB(i,j,in.yuv2rgb(lu));//convert back to RGB
        }
    }
    return out;
}

BMP Equalize(const BMP &in){
    const int MGRAY=255;
    static double his[MGRAY+1][3],tr[MGRAY+5][3];
    //histogram for r,g,b respectively
    BMP out(in);
    memset(his,0,sizeof(his));
    memset(tr,0,sizeof(tr));
    for(int k=0;k<3;k++){
        for(int i=0;i<in.getHeight();i++){
            for(int j=0;j<in.getWidth();j++){
                if(k==0) his[in.getRGB(i,j).r][0]+=1;
                else if(k==1) his[in.getRGB(i,j).g][1]+=1;
                else if(k==2)his[in.getRGB(i,j).b][2]+=1;
            }
        }
        double totalNum=0;
        for(int i=0;i<=MGRAY;i++) totalNum+=his[i][k];
        for(int i=0;i<=MGRAY;i++){ //map to [0,1]
            his[i][k]=his[i][k]/totalNum;
            tr[i][k]=(i==0?0:tr[i-1][k])+his[i][k];//T(r)=sum(p[k]) 0<=k<=r
            printf("%.2f ",tr[i][k]);
        }
        printf("\n");
    }
    for(int i=0;i<in.getHeight();i++){
        for(int j=0;j<in.getWidth();j++){
            rgb old=in.getRGB(i,j);
            uint8_t rnew=tr[old.r][0]*MGRAY;//from [0,1] to [0,255]
            uint8_t gnew=tr[old.g][1]*MGRAY;//to the closest integer
            uint8_t bnew=tr[old.b][2]*MGRAY;
            // printf("(%d,%d,%d)\n",rnew,gnew,bnew);
            out.setRGB(i,j,rgb(rnew,gnew,bnew));
        }
    }
    return out;
}
BMP Equalize2(const BMP &in){
    const int MGRAY=255;
    static double his[MGRAY+1],tr[MGRAY+5];
    //histogram for r,g,b respectively
    BMP out(in);
    memset(his,0,sizeof(his));
    memset(tr,0,sizeof(tr));
    for(int i=0;i<in.getHeight();i++){
        for(int j=0;j<in.getWidth();j++){
            yuv t=in.rgb2yuv(in.getRGB(i,j));
            his[int(t.y)]+=1;
        }
    }
    double totalNum=0;
    for(int i=0;i<=MGRAY;i++) totalNum+=his[i];
    for(int i=0;i<=MGRAY;i++){ //map to [0,1]
        his[i]=his[i]/totalNum;
        tr[i]=(i==0?0:tr[i-1])+his[i];//T(r)=sum(p[k]) 0<=k<=r
    }
    for(int i=0;i<in.getHeight();i++){
        for(int j=0;j<in.getWidth();j++){
            yuv t=in.rgb2yuv(in.getRGB(i,j));
            t.y=tr[int(t.y)]*255;//transform y->T(y)
            out.setRGB(i,j,in.yuv2rgb(t));
        }
    }
    return out;
}
int main(){
    BMP in,out;
    string fileName;
    cin>>fileName;
    fileName=fileName.substr(0,fileName.length()-4);//remove .bmp
    in.read("./img/"+fileName+".bmp");
    out=Enhance(in);
    out.write("./img/_enhanced"+fileName+".bmp");
    out=Equalize2(in);
    out.write("./img/_equalized2"+fileName+".bmp");
}