#ifndef VIDEOGETTHREAD_H
#define VIDEOGETTHREAD_H
#include "Config.h"

class VideoGetThread: public QThread
{
    Q_OBJECT
public:
     VideoGetThread(RecordConfig *config);
public slots:
    void slotOnProbeFrame(const QVideoFrame &frame);
protected:
    void run();
signals:
    void LogSend(int err,QString text);
    void VideoDataOutput(QImage); //输出信号
private:
    int Camear_Init();
    QCamera *camera;
    QVideoProbe *m_pProbe;
    QEventLoop *loop;
    struct RecordConfig *m_RecordConfig;
};


#endif // VIDEOGETTHREAD_H

