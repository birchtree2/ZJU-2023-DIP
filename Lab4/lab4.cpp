#include"bmplib.h"
#include<cmath>
#include<cstring>
// BMP rowInterpolation(const BMP &in){
//     BMP out(in);
//     rgb lastx(255,255,255);
//     for(int i=0;i<in.getHeight();i++){
//         rgb last;
//         if(in.getGray(i,0))
//         for(int j=0;j<in.getWidth();j++){
//             rgb now=out.getRGB(i,j);
//             if(now.r==255&&now.g==255&&now.b==255){//是空白的
//                 out.setRGB(i,j,last);//
//             }else last=now;
//         }
//     }
//     return out;
// }
BMP Translation(const BMP &in,int dheight,int dwidth){
    BMP out(in.getHeight()+dheight,in.getWidth()+dwidth);
    for(int i=0;i<in.getHeight();i++){
        for(int j=0;j<in.getWidth();j++){
            out.setRGB(i+dheight,j+dwidth,in.getRGB(i,j));
        }
    }
    return out;
}
BMP Mirror(const BMP &in,bool Horizontal,bool Vertical){
    BMP out(in);
    for(int i=0;i<in.getHeight();i++){
        for(int j=0;j<in.getWidth();j++){
            out.setRGB(i,j,in.getRGB(
                Vertical?(in.getHeight()-1-i):i,
                Horizontal?(in.getWidth()-1-j):j
            ));
        }
    }
    return out;
}
BMP Rotate(const BMP &in,double theta/*in rad*/){
    //x,y的变换函数
    int h=in.getHeight(),w=in.getWidth();
    int H=0,W=0;
    BMP out(
        fabs(w*cos(theta))+fabs(h*sin(theta)),
        fabs(h*cos(theta))+fabs(w*sin(theta))
    );
    //x=(x0-w/2)*cos(theta)+()+w/2
    H=out.getHeight(),W=out.getWidth();
    for(int y=0;y<H;y++){
        for(int x=0;x<W;x++){
            double x0=(x-W*0.5)*cos(theta)+(y-H*0.5)*sin(theta)+w*0.5;//逆变换
            double y0=-(x-W*0.5)*sin(theta)+(y-H*0.5)*cos(theta)+h*0.5;
            int x0i=round(x0),y0i=round(y0);//nearest neighbour
            if(x0i>=0&&x0i<w&&y0i>=0&&y0i<h){
                out.setRGB(y,x,in.getRGB(y0i,x0i));
            }
        }
    }
    return out;
}
BMP Shear(const BMP &in,double dx,double dy){
    // auto fx=[dx,dy](double x,double y)->double{return x+dx*y;};
    // auto fy=[dx,dy](double x,double y)->double{return y+dy*x;};
    //注意width对应X，height对应Y
    int h=in.getHeight(),w=in.getWidth();
    BMP out(h+dy*w,w+dx*h);
    for(int y=0;y<in.getHeight();y++){
        for(int x=0;x<in.getWidth();x++){
            out.setRGB(y+dy*x,x+dx*y,in.getRGB(y,x));
        }
    }
    return out;
}
BMP Scale(const BMP &in,double cx,double cy){
    BMP out(in.getHeight()*cy,in.getWidth()*cx);
    for(int y=0;y<out.getHeight();y++){
        for(int x=0;x<out.getWidth();x++){
            out.setRGB(y,x,in.getRGB(y/cy,x/cx));//nearest neighbour interporlation
        }
    }
    return out;
    // return rowInterpolation(out);
}
int main(){
    const double pi=acos(-1);
    BMP in,out;
    string fileName="test3";
    // cin>>fileName;
    // fileName=fileName.substr(0,fileName.length()-4);//remove .bmp
    in.read("./img/"+fileName+".bmp");
    out=Translation(in,50,50);
    out.write("./img/translation_"+fileName+".bmp");
    out=Mirror(in,true,false);
    out.write("./img/mirrorX_"+fileName+".bmp");
    out=Mirror(in,false,true);
    out.write("./img/mirrorY_"+fileName+".bmp");
    out=Shear(in,1,0);
    out.write("./img/shearX1.5_"+fileName+".bmp");
    out=Scale(in,2,1);
    out.write("./img/scale_"+fileName+".bmp");
    out=Rotate(in,3*pi/4);
    out.write("./img/rotate45_"+fileName+".bmp");
    // out=Equalize2(in);
    // out.write("./img/_equalized2"+fileName+".bmp");
}