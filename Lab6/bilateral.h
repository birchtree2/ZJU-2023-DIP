#pragma once
#include"bmplib.h"
BMP bilateralFilter(const BMP &in, int kernelSize, double spaceSigma,
                    double colorSigma);
