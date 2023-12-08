#pragma once
#include"bmplib.h"
#include<cmath>
BMP bilateralFilter(const BMP &in, int kernelSize, double spaceSigma,
                    double colorSigma);

