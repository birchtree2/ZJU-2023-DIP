#include"bmplib.h"
#include<cmath>
using namespace std;
void BMP::debug(){
    printf("h=%d w=%d dsz=%d\n",header.height,header.width,data.size());
}
unsigned char BMP::solve(double x){
    if(x<0) return 0;
    if(x>255) return 255;
    return round(x);
}
BMPHeader BMP::getHeader() const{return header;}
BMP::BMP(BMPHeader _header):header(_header){}
int BMP::getHeight() const{ return header.height;}
int BMP::getWidth() const{ return header.width;}
BMP::BMP(const BMP& other){//todo
    header=other.header;
    data.resize(header.height);
    for(int i=0;i<header.height;i++){
        data[i].resize(header.width);
        for(int j=0;j<header.width;j++) data[i][j]=other.data[i][j]; 
    }
}
BMP::BMP(int newheight,int newwidth){
    header.type = 0x4D42;//BM
    header.bitsPerPixel = 24;
    header.width = newwidth;//set to new height
    header.height = newheight;
    header.fileSize = newheight * (newwidth * 3 + paddingLen()) + sizeof(BMPHeader);
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.dataOffset = sizeof(BMPHeader);
    // 头部大小设置为40
    header.headerSize = 40;
    header.planes = 1;
    // 压缩方式设置为无压缩
    header.compression = 0;
    // 原始图像数据大小设置为0
    header.dataSize = 0;
    // 水平和垂直分辨率设置为默认值
    header.horizonRes = 0;
    header.verticalRes = 0;
    // 颜色数量和重要的颜色数量设置为0
    header.colors = 0;
    header.impColors = 0;
    // 初始化图像数据为新的高度和宽度
    // data.resize(newheight, vector<rgb>(newwidth));
    data.resize(header.height);
    for(int i=0;i<header.height;i++){
        data[i].resize(header.width);
        for(int j=0;j<header.width;j++) data[i][j]=rgb(255,255,255); 
    }
}

int BMP::paddingLen()const{
    int rowSize=header.bitsPerPixel/8*header.width;//每行的字节数
    return (4-rowSize%4)%4;//补到4的倍数需要的的字节数
}
int BMP::read(string fileName){
    FILE* fp=fopen(fileName.c_str(),"rb");//打开输入文件
    if(!fp){
        cerr<<"Can't open file "+fileName<<endl;
        return 0;
    }
    fread(&header,sizeof(header),1,fp);//读入文件头
    if(header.type!=0x4D42){
        cerr<<"Not BMP file"+fileName<<endl;
        return 0;
    } 
    fseek(fp,header.dataOffset,SEEK_SET);
    int padding=paddingLen();//计算补齐长度
    unsigned char zero=0;
    data.resize(header.height);
    for(int i=0;i<header.height;i++){//按行读入
        data[i].resize(header.width);
        for(int j=0;j<header.width;j++){
            uint8_t b,g,r;//b,g,r的顺序
            fread(&b,1,1,fp);
            fread(&g,1,1,fp);
            fread(&r,1,1,fp);
            data[i][j]=rgb(r,g,b);
        }
        fseek(fp,padding,SEEK_CUR);//跳过多的
    }
    fclose(fp);
    return 1;
}
int BMP::write(string fileName){
    FILE* fp=fopen(fileName.c_str(),"wb");
    if(!fp) return 1;
    fwrite(&header,sizeof(header),1,fp);
    fseek(fp,header.dataOffset,SEEK_SET);//跳到图像数据区
    int padding=paddingLen();
    uint8_t zero=0;
    for(int i=0;i<header.height;i++){//按行读入
        for(int j=0;j<header.width;j++){
            fwrite(&data[i][j].b,1,1,fp);
            fwrite(&data[i][j].g,1,1,fp);
            fwrite(&data[i][j].r,1,1,fp);
        }
        for(int j=0;j<padding;j++) fwrite(&zero,1,1,fp);
    }
    printf("Successfully write %s!\n",fileName.c_str());
    fclose(fp);
    return 0;
}
yuv BMP::rgb2yuv(rgb in) const{
    double y,u,v;
    y=0.299*in.r+0.587*in.g+0.114*in.b;
    u=-0.147*in.r-0.289*in.g+0.436*in.b;
    v=0.615*in.r-0.515*in.g-0.100*in.b;
    return yuv(y,u,v);
}
rgb BMP::yuv2rgb(yuv in) const{
    uint8_t r,g,b;
    r=solve(in.y+1.139*in.v);
    g=solve(in.y-0.395*in.u-0.581*in.v);
    b=solve(in.y+2.032*in.u);
    return rgb(r,g,b);
}
BMP BMP::toGray(){
    BMP out;
    out.header=header;
    out.data.resize(header.height);
    for(int i=0;i<header.height;i++){
        out.data[i].resize(header.width);
        for(int j=0;j<header.width;j++){
            yuv val=rgb2yuv(data[i][j]);//rgb转换为yuv
            out.data[i][j].b=solve(val.y);//把r,g,b都设为y
            out.data[i][j].g=solve(val.y);
            out.data[i][j].r=solve(val.y);
        }
    }
    return out;
}
BMP BMP::changeY(double deltaY){
    BMP out;
    out.header=header;
    out.data.resize(header.height);
    for(int i=0;i<header.height;i++){
        out.data[i].resize(header.width);
        for(int j=0;j<header.width;j++){
            yuv val=rgb2yuv(data[i][j]);//rgb转换为yuv
            val.y+=deltaY;
            out.data[i][j]=yuv2rgb(val);
        }
    }
    return out;
}
uint8_t BMP::getGray(int x,int y)const{
    x=min(max(x,0),getHeight()-1);
    y=min(max(y,0),getWidth()-1);
    return data[x][y].r;
}
uint8_t BMP::setGray(int x,int y,int val){
    data[x][y]=rgb(val,val,val);
}
rgb BMP::getRGB(int x,int y) const{
    x=min(max(x,0),getHeight()-1);
    y=min(max(y,0),getWidth()-1);
    return data[x][y];
}
rgb BMP::setRGB(int x,int y,rgb val){
    data[x][y]=val;
}
