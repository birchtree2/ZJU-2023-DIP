#include"bmplib.h"
BMP BMP::toBinary(){
    BMP out(*this);
    printf("ok\n");
    uint8_t T=0;
    vector<double>cnt;//记录灰度i的出现频率
    cnt.resize(256);
    for(int i=0;i<getHeight();i++){
        for(int j=0;j<getWidth();j++){
            cnt[data[i][j].r]++;
        }
    }
    double imgSize=getWidth()*getHeight();
    double avg0=0,w1=0,sum=0;
    for(int i=0;i<256;i++){
        cnt[i]/=imgSize;
        avg0+=i*cnt[i];//全局的平均灰度值
    }
    double maxVar=0;
    for(int i=0;i<256;i++){
        w1+=cnt[i];//前景的概率之和
        sum+=i*cnt[i];
        if(w1==0) continue;
        double avg1=sum/w1;//前景的平均灰度值
        double var=(avg1-avg0)*(avg1-avg0)*w1/(1-w1);
        if(var>maxVar){
            maxVar=var;
            T=i;
        }
    }
    // printf("T=%d\n",T);
    for(int i=0;i<getHeight();i++){
        for(int j=0;j<getWidth();j++){
            if(data[i][j].r<T) out.data[i][j]=rgb(0,0,0);
            else out.data[i][j]=rgb(255,255,255);
        }
    }
    return out;
}

uint8_t BMP::getGray(int x,int y){
    x=min(max(x,0),getHeight()-1);
    y=min(max(y,0),getWidth()-1);
    return data[x][y].r;
}
uint8_t BMP::setGray(int x,int y,int val){
    data[x][y]=rgb(val,val,val);
}
BMP BMP::dilateOrErode(bool isDilate){
    const int Size=3;
    const bool kernel[3][3]={{0,1,0},{1,1,1},{0,1,0}};
    const int centerX=1,centerY=1;
    //structural element
    BMP out(*this);
    for(int i=0;i<getHeight();i++){
        for(int j=0;j<getWidth();j++){
            bool ans=isDilate?0:1;
            for(int k=-1;k<=1;k++){
                for(int u=-1;u<=1;u++){
                    if(kernel[centerX+k][centerY+u]){
                        if(isDilate) ans=ans || !getGray(i+k,j+u);//黑色的灰度值是0,但在形态学里是1
                        else ans=ans && !getGray(i+k,j+u);//白色的灰度值是255，但在形态学里是0
                    }
                }
            }
            out.setGray(i,j,ans?0:255);
        }
    }
    return out;
}
BMP BMP::dilate(){
    return dilateOrErode(1);
}
BMP BMP::erode(){
    return dilateOrErode(0);
}
BMP BMP::opening(){
    return erode().dilate();
}
BMP BMP::closing(){
    return dilate().erode();
}
int main(){
    BMP in,out;string fileName;
    cin>>fileName;
    fileName=fileName.substr(0,fileName.length()-4);//去掉后缀名
    in.read("./img/"+fileName+".bmp");
    in=in.toGray();
    in=in.toBinary();
    in.write("./img/"+fileName+"_binary.bmp");
    out=in.dilate();
    out.write("./img/"+fileName+"_dilate.bmp");
    out=in.erode();
    out.write("./img/"+fileName+"_erode.bmp");
    out=in.opening();
    out.write("./img/"+fileName+"_opening.bmp");
    out=in.closing();
    out.write("./img/"+fileName+"_closing.bmp");
}