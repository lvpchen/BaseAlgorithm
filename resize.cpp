#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

typedef unsigned char uint8_t;

uint8_t* resizePixels(uint8_t* pixels, int w1, int h1, int w2, int h2)
{
	uint8_t* temp;
	temp = new uint8_t[w2 * h2 *3];
	double x_ratio = w1 / (double)w2;
	double y_ratio = h1 / (double)h2;
	double px, py;
	for (int i = 0; i<h2; i++)
	{
		for (int j = 0; j<w2; j++)
		{
			px = floor(j*x_ratio);
			py = floor(i*y_ratio);
			temp[(i*w2*3) + j] = pixels[(int)((py*w1*3) + px)];
		}
	}
	return temp;
}

int main(int argc, char** argv)
{
    uint8_t dst_w = 320;
    uint8_t dst_h = 240;
    const char* imagepath = "/home/lvpchen/workspace_lvpchen/BaseAlgorithm/images/test.jpg";    
    cv::Mat img_src = cv::imread(imagepath, CV_LOAD_IMAGE_COLOR);
    //cvtColor(img_src,img_src,CV_BGR2GRAY);
    cv::Mat img_dst(dst_h,dst_w,CV_8UC3);
    printf("%d %d\n",img_src.rows,img_src.cols);
    uint8_t *dstPtr = resizePixels(img_src.data,img_src.cols,img_src.rows,dst_w,dst_h);
    img_dst.data = dstPtr;
    imshow("src",img_src);
    imshow("dst",img_dst);
    waitKey(0);
    return 1;
}
