#ifndef CONFIG_H
#define CONFIG_H
#include <QThread>
#include <QCamera>
#include <QVideoProbe>
#include <QWidget>
#include <QCameraInfo>
#include <QAudioDeviceInfo>
#include <QScrollBar>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QPainter>
#include <QDateTime>
#include "Color_conversion.h"
#include <QEventLoop>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QAudioInput>

//声明引用C的头文件
extern "C"
{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <math.h>

    #include <libavutil/avassert.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/opt.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/timestamp.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
    #include "libavfilter/avfilter.h"

    #include "libavutil/avassert.h"
    #include "libavutil/channel_layout.h"
    #include "libavutil/common.h"
    #include "libavutil/opt.h"
}


//录制视频的参数配置
struct RecordConfig
{
    int Mode;          //  0 本地录制   1 推流
    int ImageInptSrc;  //图像来源:  0 摄像头  1 桌面
    int input_DataConf; // 0只要画面  1 画面+音频
    qint8 desktop_page;      //桌面的页面编号
    int video_bit_rate;  //码率
    int video_frame;  //帧率
    int ImageWidth;  //图像宽
    int ImageHeight; //图像高
    QString VideoSavePath; //视频保存路径

    /*视频相关*/
    QMutex  video_encode_mutex;
    QWaitCondition video_WaitConditon;
    QCameraInfo camera;

    /*音频相关*/
    QAudioDeviceInfo audio;
    QMutex  audio_encode_mutex;
    QQueue<QByteArray> AudioDataQueue;

    /*推流地址 比如: rtmp  rtsp等等*/
    QByteArray PushStreamAddr;

    //RGB图像缓冲区,这是采集数据之后转为RGB888的数据，因为还需要将此数据转为YUV420P
    unsigned char *rgb_buffer;
    size_t grb24_size; //RGB数据大小。 宽 x 高 x 3

    //存放转换后的YUV42OP数据
    unsigned char *video_yuv420p_buff;
    unsigned char *video_yuv420p_buff_temp;
    size_t yuv420p_size; //yuv420p数据大小。 宽 x 高 x 3 / 2

};



/*
音频码率=采样率 * 位深度* 通道数
        44100 *   32 *  1   =1411200‬bps
*/
//音频相关参数设置
#define AUDIO_RATE_SET 44100         //音频采样率
#define AUDIO_BIT_RATE_SET  64000  //设置码率  40000
#define AUDIO_CHANNEL_SET   AV_CH_LAYOUT_MONO  //AV_CH_LAYOUT_MONO 单声道   AV_CH_LAYOUT_STEREO 立体声
#define FFMPEG_AudioFormat AV_SAMPLE_FMT_FLTP  //指定音频的格式


#endif // CONFIG_H

