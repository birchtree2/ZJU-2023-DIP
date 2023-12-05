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
unsigned char solve(double x){
    if(x<0) return 0;
    if(x>255) return 255;
    return x;
}
int main(){
    double deltay;
    string inname;
    int lu;
    while(1){
        printf("[FileName] [Delta]");
        cin>>inname;
        if(inname=="quit") break;
        cin>>deltay;
        FILE* bmpin=fopen(inname.c_str(),"rb");
        string grayname="gray_"+inname;
        string newname="new_"+inname;
        if(!bmpin){
            printf("Failed to open File\n");
            return 0;
        }
        BMPHeader header;
        fread(&header,sizeof(header),1,bmpin);
        if(header.type!=0x4D42){
            printf("Not BMP file\n");
            return 0;
        }
        if(header.bitsPerPixel!=24){
            printf("%d\n",header.bitsPerPixel);
            printf("Not 24-bit BMP\n");
            return 0;
        }
        FILE* bmpgray=fopen(grayname.c_str(),"wb");
        fwrite(&header,sizeof(header),1,bmpgray);//写入文件头
        FILE* bmpnew=fopen(newname.c_str(),"wb");
        fwrite(&header,sizeof(header),1,bmpnew);

        int rowSize = ceil(header.width * header.bitsPerPixel / 8.0);//每行的字节数
        int padding=(4-rowSize%4)%4;//补到4的倍数需要的的字节数
        unsigned char zero=0;
        printf("total:%d B ,padding=%d\n",header.height*header.width,padding);
        for(int i=0;i<header.height;i++){//按行读入
            for(int j=0;j<header.width;j++){
                unsigned char b,g,r;//b,g,r的顺序
                fread(&b,1,1,bmpin);
                fread(&g,1,1,bmpin);
                fread(&r,1,1,bmpin);
                double y,u,v;
                y=0.299*r+0.587*g+0.114*b;
                u=-0.147*r-0.289*g+0.436*b;
                v=0.615*r-0.515*g-0.100*b;
                unsigned char _y=solve(y);//convert to 1byte
                fwrite(&_y,1,1,bmpgray); //灰度图像,r=g=b=y
                fwrite(&_y,1,1,bmpgray); 
                fwrite(&_y,1,1,bmpgray);

                y+=deltay;
                // printf("%d %d %d - ",r,g,b);
                r=solve(y+1.139*v);
                g=solve(y-0.395*u-0.581*v);
                b=solve(y+2.032*u);
                // printf("%d %d %d \n ",r,g,b);
                // solve(r);solve(g);solve(b
                fwrite(&b,1,1,bmpnew);
                fwrite(&g,1,1,bmpnew);
                fwrite(&r,1,1,bmpnew);
            }
            fseek(bmpin, padding, SEEK_CUR);//跳过填充的字节
            for(int j=0;j<padding;j++) fwrite(&zero,1,1,bmpgray);//补0
            for(int j=0;j<padding;j++) fwrite(&zero,1,1,bmpnew);
        }
        fclose(bmpin);
        fclose(bmpgray);
        fclose(bmpnew);
    }
    
}
