#include "DesktopImageGetThread.h"

DesktopImageGetThread::DesktopImageGetThread(RecordConfig *config)
{
    m_RecordConfig=config;

    //Windows系统截图
    screen = QGuiApplication::primaryScreen();

}


//线程开始运行
void DesktopImageGetThread::run()
{
    //根据帧率计算采集频率
    int sleep_ms=1000/m_RecordConfig->video_frame;

    while(m_run)
    {
        //获取当前屏幕的图像,默认获取全屏
        QPixmap pixmap=screen->grabWindow(m_RecordConfig->desktop_page);
        QImage image=pixmap.toImage();
        image=image.scaled(m_RecordConfig->ImageWidth,m_RecordConfig->ImageHeight,Qt::KeepAspectRatio, Qt::SmoothTransformation);

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
        m_RecordConfig->video_encode_mutex.lock();
        RGB24_TO_YUV420(m_RecordConfig->rgb_buffer,image.width(),image.height(),m_RecordConfig->video_yuv420p_buff);
        m_RecordConfig->video_encode_mutex.unlock();
        m_RecordConfig->video_WaitConditon.wakeAll();

        //发送信号，显示在UI界面
        emit VideoDataOutput(image);

        //延时等待,这里需要改进，需要加上前面消耗的时间
        msleep(sleep_ms);
    }

    LogSend(0,"桌面画面采集线程已退出...\n");
}


