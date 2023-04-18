#ifndef COLOR_CONVERSION_H
#define COLOR_CONVERSION_H
#include "Config.h"

//全局函数
void YUV420P_to_RGB24(unsigned char *data, unsigned char *rgb, int width, int height);
void NV21_TO_RGB24(unsigned char *data, unsigned char *rgb, int width, int height);
void YUV420P_TO_RGB24(unsigned char *yuv420p, unsigned char *rgb24, int width, int height);
void yuyv_to_rgb(unsigned char *yuv_buffer,unsigned char *rgb_buffer,int iWidth,int iHeight);
int yuyv_to_yuv420p(const unsigned char *in, unsigned char *out, unsigned int width, unsigned int height);
void NV21_YUV420P(const unsigned char* image_src, unsigned char* image_dst,int image_width, int image_height);
bool RGB24_TO_YUV420(unsigned char *RgbBuf,int w,int h,unsigned char *yuvBuf);

#endif // COLOR_CONVERSION_H
