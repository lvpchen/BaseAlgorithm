#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H

class GaussianBlur
{
public:
    GaussianBlur();

    void gaussBlur(QImage &img, int radius);

private:
    void gaussBlur1(int* pix, int w, int h, int radius);
    void gaussBlur2(int* pix, int w, int h, int radius);
    void boxBlurH(int* srcPix, int* destPix, int w, int h, int radius);
    void boxBlurV(int* srcPix, int* destPix, int w, int h, int radius);
    void boxBlur(int* srcPix, int* destPix, int w, int h, int r);
    void boxesForGauss(float sigma, int* size, int n);

};

#endif // GAUSSIANBLUR_H
