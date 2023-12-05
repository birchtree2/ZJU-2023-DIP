#include<stddef.h>
#include<vector>
#include<iostream>
#include<stdio.h>
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
    unsigned char solve(double x);
    BMP dilateOrErode(bool isDilate);
public:
    BMP(){};
    BMP(BMPHeader _header);
    BMP(const BMP& other);//拷贝构造函数
    BMPHeader getHeader();
    int read(string fileName);
    int write(string fileName);
    int getHeight();
    int getWidth();
//灰度部分
    BMP toGray();
    BMP changeY(double deltaY);
//二值化部分
    uint8_t getGray(int x,int y);
    uint8_t setGray(int x,int y,int val);
    BMP toBinary();
    BMP erode();
    BMP dilate();
    BMP opening();
    BMP closing();
    void debug();
};