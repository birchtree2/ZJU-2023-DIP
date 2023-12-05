#include<bits/stdc++.h>
using namespace std;
#pragma pack(push, 1) //按字节顺序
struct BMPHeader{
//文件头file Header:
    uint16_t type;          // 文件类型
    uint32_t fileSize;      // 文件大小
    uint16_t reserved1;     // 保留字段1
    uint16_t reserved2;     // 保留字段2
    uint32_t dataOffset;    // 数据偏移  从文件开始的地方到图像数据的偏移量
//图像信息头 infomation header    
    uint32_t headerSize;    // 头部大小
    int32_t width;          // 图像宽度(像素个数)
    int32_t height;         // 图像高度(像素个数)
    uint16_t planes;        // 颜色平面数
    uint16_t bitsPerPixel;  // 每个像素的位数  一般来讲是24
    uint32_t compression;   // 压缩方式
    uint32_t dataSize;      // 原始图像数据大小
    int32_t horizonRes;     // 水平分辨率 单位:像素/米
    int32_t verticalRes;    // 垂直分辨率
    uint32_t colors;        // 颜色数量
    uint32_t impColors;     // 重要的颜色数量
};
#pragma pack(pop)
struct rgb{
    uint8_t r,g,b;
    rgb(){};
    rgb(uint8_t _r,uint8_t _g,uint8_t _b):r(_r),g(_g),b(_b){}
};
struct yuv{
    double y,u,v;
    yuv(){};
    yuv(double _y,double _u,double _v):y(_y),u(_u),v(_v){}
};
class BMP{
private:
    BMPHeader header;
    vector< vector< rgb > >data;
    yuv rgb2yuv(rgb in);
    rgb yuv2rgb(yuv in);
    int paddingLen();
public:
    BMP(){};
    BMP(BMPHeader _header);
    BMPHeader getHeader();
    int read(string fileName);
    int write(string fileName);
    BMP toGray();
    BMP changeY(double deltaY);
    void debug();
};
void BMP::debug(){
    printf("h=%d w=%d dsz=%d\n",header.height,header.width,data.size());
}
unsigned char solve(double x){
    if(x<0) return 0;
    if(x>255) return 255;
    return x;
}
BMPHeader BMP::getHeader(){return header;}
BMP::BMP(BMPHeader _header):header(_header){}
int BMP::paddingLen(){
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
yuv BMP::rgb2yuv(rgb in){
    double y,u,v;
    y=0.299*in.r+0.587*in.g+0.114*in.b;
    u=-0.147*in.r-0.289*in.g+0.436*in.b;
    v=0.615*in.r-0.515*in.g-0.100*in.b;
    return yuv(y,u,v);
}
rgb BMP::yuv2rgb(yuv in){
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

int main(){
    double deltay;
    string inname;
    BMP in,out;
    while(1){
        printf("Please input [FileName] [DeltaY],input\"quit\"to exit program: ");
        cin>>inname;
        if(inname=="quit") break;
        cin>>deltay;
        if(in.read(inname)){
            out=in.toGray();
            out.write("gray_"+inname);
            out=in.changeY(deltay);
            out.write("new_"+inname);
        }
    }
}
    

