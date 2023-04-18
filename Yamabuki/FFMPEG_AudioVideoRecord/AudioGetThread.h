#ifndef AUDIOGETTHREAD_H
#define AUDIOGETTHREAD_H
#include "Config.h"
class AudioGetThread: public QThread
{
    Q_OBJECT
public:
     AudioGetThread(RecordConfig *config);
public slots:
     void handleStateChanged_input(QAudio::State newState);
     void audio_ReadyRead();
protected:
    void run();
signals:
    void LogSend(int err,QString text);
private:
    int Audio_Init();

    QEventLoop *loop;
    struct RecordConfig *m_RecordConfig;
    QAudioInput *audio_in;
    QIODevice* audio_streamIn;
};

#endif // AUDIOGETTHREAD_H

