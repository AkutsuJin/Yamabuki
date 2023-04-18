#ifndef DESKTOPIMAGEGETTHREAD_H
#define DESKTOPIMAGEGETTHREAD_H

#include "Config.h"

class DesktopImageGetThread: public QThread
{
    Q_OBJECT
public:
     DesktopImageGetThread(RecordConfig *config);
     int m_run; //运行状态 0停止 1继续
protected:
    void run();
signals:
    void LogSend(int err,QString text);
    void VideoDataOutput(QImage); //输出信号
private:
    struct RecordConfig *m_RecordConfig;
    QScreen *screen;
};


#endif // DESKTOPIMAGEGETTHREAD_H

