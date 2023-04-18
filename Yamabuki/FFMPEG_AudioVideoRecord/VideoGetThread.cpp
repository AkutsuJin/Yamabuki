#include "VideoGetThread.h"

VideoGetThread::VideoGetThread(struct RecordConfig *config)
{
    //全局参数配置
    m_RecordConfig=config;
}


//开始运行线程
void VideoGetThread::run()
{
    int ret=-1;
    loop=new QEventLoop;

    //初始摄像头，开启视频采集
    ret=Camear_Init();
    if(ret!=0)
    {
        LogSend(-1,"摄像头初始化错误...\n");
        goto RETURN;
    }

    //开启事件循环
    loop->exec();

RETURN:
    loop->exit();

    //出来释放空间
    delete loop;
    loop=nullptr;

    if(camera)
    {
        camera->stop();
    }

    if(camera)
    {
        delete camera;
        camera=nullptr;
    }

    if(m_pProbe)
    {
        delete  m_pProbe;
        m_pProbe=nullptr;
    }

    LogSend(0,"摄像头采集线程退出...\n");
}


//摄像头初始化
int VideoGetThread::Camear_Init()
{
    /*创建摄像头对象，根据选择的摄像头打开*/
    camera = new QCamera(m_RecordConfig->camera);
    if(camera==nullptr)return -1;

    m_pProbe = new QVideoProbe;
    if(m_pProbe == nullptr)return -1;

    m_pProbe->setSource(camera); // Returns true, hopefully.
    connect(m_pProbe, SIGNAL(videoFrameProbed(QVideoFrame)),this, SLOT(slotOnProbeFrame(QVideoFrame)), Qt::QueuedConnection);

    /*配置摄像头捕获模式为帧捕获模式*/
    //camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->setCaptureMode(QCamera::CaptureVideo);

    /*启动摄像头*/
    camera->start();

    /*设置摄像头的采集帧率和分辨率*/
    QCameraViewfinderSettings settings;
   // settings.setPixelFormat(QVideoFrame::Format_YUYV); //设置像素格式  Android上只支持NV21格式

    //设置摄像头的分辨率
    settings.setResolution(QSize(m_RecordConfig->ImageWidth,m_RecordConfig->ImageHeight));
    camera->setViewfinderSettings(settings);

    //获取当前的配置
    //QCameraViewfinderSettings get_set=camera->viewfinderSettings();
    //qDebug()<<"最大帧率支持:"<<get_set.maximumFrameRate();
    //qDebug()<<"像素格式支持:"<<get_set.pixelFormat();
    //qDebug()<<"当前分辨率:"<<get_set.resolution();

    //获取摄像头支持的分辨率、帧率等参数
#if 0
    int i=0;
    QList<QCameraViewfinderSettings > ViewSets = camera->supportedViewfinderSettings();
    foreach (QCameraViewfinderSettings ViewSet, ViewSets)
    {
        qDebug() << i++ <<" max rate = " << ViewSet.maximumFrameRate() << "min rate = "<< ViewSet.minimumFrameRate() << "resolution "<<ViewSet.resolution()<<ViewSet.pixelFormat();
    }

/* 我的笔记本电脑输出的格式如下
0  max rate =  30 min rate =  30 resolution  QSize(1280, 720) Format_Jpeg
1  max rate =  30 min rate =  30 resolution  QSize(320, 180) Format_Jpeg
2  max rate =  30 min rate =  30 resolution  QSize(320, 240) Format_Jpeg
3  max rate =  30 min rate =  30 resolution  QSize(352, 288) Format_Jpeg
4  max rate =  30 min rate =  30 resolution  QSize(424, 240) Format_Jpeg
5  max rate =  30 min rate =  30 resolution  QSize(640, 360) Format_Jpeg
6  max rate =  30 min rate =  30 resolution  QSize(640, 480) Format_Jpeg
7  max rate =  30 min rate =  30 resolution  QSize(848, 480) Format_Jpeg
8  max rate =  30 min rate =  30 resolution  QSize(960, 540) Format_Jpeg
9  max rate =  10 min rate =  10 resolution  QSize(1280, 720) Format_YUYV
10  max rate =  10 min rate =  10 resolution  QSize(1280, 720) Format_BGR24
11  max rate =  30 min rate =  30 resolution  QSize(320, 180) Format_YUYV
12  max rate =  30 min rate =  30 resolution  QSize(320, 180) Format_BGR24
13  max rate =  30 min rate =  30 resolution  QSize(320, 240) Format_YUYV
14  max rate =  30 min rate =  30 resolution  QSize(320, 240) Format_BGR24
15  max rate =  30 min rate =  30 resolution  QSize(352, 288) Format_YUYV
16  max rate =  30 min rate =  30 resolution  QSize(352, 288) Format_BGR24
17  max rate =  30 min rate =  30 resolution  QSize(424, 240) Format_YUYV
18  max rate =  30 min rate =  30 resolution  QSize(424, 240) Format_BGR24
19  max rate =  30 min rate =  30 resolution  QSize(640, 360) Format_YUYV
20  max rate =  30 min rate =  30 resolution  QSize(640, 360) Format_BGR24
21  max rate =  30 min rate =  30 resolution  QSize(640, 480) Format_YUYV
22  max rate =  30 min rate =  30 resolution  QSize(640, 480) Format_BGR24
23  max rate =  20 min rate =  20 resolution  QSize(848, 480) Format_YUYV
24  max rate =  20 min rate =  20 resolution  QSize(848, 480) Format_BGR24
25  max rate =  15 min rate =  15 resolution  QSize(960, 540) Format_YUYV
26  max rate =  15 min rate =  15 resolution  QSize(960, 540) Format_BGR24
*/
#endif

    return 0;
}


void VideoGetThread::slotOnProbeFrame(const QVideoFrame &frame)
{
   QVideoFrame cloneFrame(frame);
   cloneFrame.map(QAbstractVideoBuffer::ReadOnly);

   if(cloneFrame.pixelFormat()==QVideoFrame::Format_NV21)
   {
        NV21_TO_RGB24(cloneFrame.bits(),m_RecordConfig->rgb_buffer,cloneFrame.width(),cloneFrame.height());
   }
   else if(cloneFrame.pixelFormat()==QVideoFrame::Format_YUYV)
   {
       yuyv_to_rgb(cloneFrame.bits(),m_RecordConfig->rgb_buffer,cloneFrame.width(),cloneFrame.height());
   }
   else if(cloneFrame.pixelFormat()==QVideoFrame::Format_RGB24)
   {
        memcpy(m_RecordConfig->rgb_buffer,cloneFrame.bits(),m_RecordConfig->grb24_size);
   }
   else if(cloneFrame.pixelFormat()==QVideoFrame::Format_BGR24)
   {
        memcpy(m_RecordConfig->rgb_buffer,cloneFrame.bits(),m_RecordConfig->grb24_size);
   }
   else if(cloneFrame.pixelFormat()==QVideoFrame::Format_Jpeg)
   {

   }
   else
   {
       qDebug("当前格式编码为%1,暂时不支持转换.\n");
   }

    //加载图片数据
    QImage image(m_RecordConfig->rgb_buffer,
                       cloneFrame.width(),
                       cloneFrame.height(),
                       QImage::Format_RGB888);
    if(cloneFrame.pixelFormat()==QVideoFrame::Format_BGR24)
    {
        image=image.rgbSwapped(); //BGR格式转RGB
        image=image.mirrored(false, true);
    }
    if(cloneFrame.pixelFormat()==QVideoFrame::Format_Jpeg)
    {
        image.loadFromData((const uchar *)cloneFrame.bits(),cloneFrame.mappedBytes());
    }


    //绘制图片水印
    QDateTime dateTime(QDateTime::currentDateTime());
    //时间效果: 2020-03-05 16:25::04 周一
    QString qStr="";
    qStr+=dateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
    QPainter pp(&image);
    QPen pen = QPen(Qt::white);
    pp.setPen(pen);
    pp.fillRect(QRectF(0,0,300,25),QBrush(QColor(255,0,0,100)));
    pp.drawText(QPointF(0,20),qStr);


    //提取RGB数据
    //下面这一块处理比较耗时间，效率低下. 后期再进行优化
    unsigned char *p=m_RecordConfig->rgb_buffer;
    for(int i=0;i<image.height();i++)
    {
        for(int j=0;j<image.width();j++)
        {
            QRgb rgb=image.pixel(j,i);
            *p++=qRed(rgb);
            *p++=qGreen(rgb);
            *p++=qBlue(rgb);
        }
    }

    //将数据放入全局缓冲区
    m_RecordConfig->video_encode_mutex.lock();
    RGB24_TO_YUV420(m_RecordConfig->rgb_buffer,image.width(),image.height(),m_RecordConfig->video_yuv420p_buff);
    m_RecordConfig->video_encode_mutex.unlock();
    m_RecordConfig->video_WaitConditon.wakeAll();

    //将图像数据给UI界面
    emit VideoDataOutput(image); //发送信号
    cloneFrame.unmap();
}


