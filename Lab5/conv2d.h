#ifndef __CONV2D_H__
#define __CONV2D_H__
#include"bmplib.h"
typedef vector<vector<double>> dMatrix;
BMP conv2d(const BMP&in, const dMatrix& kernel);
#endif