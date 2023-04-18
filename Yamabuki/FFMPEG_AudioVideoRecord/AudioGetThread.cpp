#include "AudioGetThread.h"


//配置
AudioGetThread::AudioGetThread(RecordConfig *config)
{
    m_RecordConfig=config;
}


int AudioGetThread::Audio_Init()
{
    QString text;
    QAudioFormat auido_input_format;

    //设置录音的格式
    auido_input_format.setSampleRate(44100); //设置采样率以对赫兹采样。 以秒为单位，每秒采集多少声音数据的频率.
    auido_input_format.setChannelCount(1);   //将通道数设置为通道。
    auido_input_format.setSampleSize(16);     /*将样本大小设置为指定的sampleSize（以位为单位）通常为8或16，但是某些系统可能支持更大的样本量。*/
    auido_input_format.setCodec("audio/pcm"); //设置编码格式
    auido_input_format.setByteOrder(QAudioFormat::LittleEndian); //样本是小端字节顺序
    auido_input_format.setSampleType(QAudioFormat::SignedInt); //样本类型
    //选择设备作为输入源
    QAudioDeviceInfo info = m_RecordConfig->audio;
    emit LogSend(0,tr("当前的录音设备的名字:%1\n").arg(info.deviceName()));

    //判断输入的格式是否支持，如果不支持就使用系统支持的默认格式
    if(!info.isFormatSupported(auido_input_format))
    {
      emit LogSend(0,"返回与系统支持的提供的设置最接近的QAudioFormat\n");
      auido_input_format=info.nearestFormat(auido_input_format);
      /*
       * 返回与系统支持的提供的设置最接近的QAudioFormat。
         这些设置由所使用的平台/音频插件提供。
         它们还取决于所使用的QAudio :: Mode。
      */
    }
    //当前设备支持的编码
    emit LogSend(0,"当前设备支持的编码格式:\n");
    QStringList list=info.supportedCodecs();
    for(int i=0;i<list.size();i++)
    {
        text=list.at(i)+"\n";
        emit LogSend(0,text);
    }

    int sampleRate=auido_input_format.sampleRate();

    emit LogSend(0,tr("当前录音的采样率=%1\n").arg(sampleRate));

    emit LogSend(0,tr("当前录音的通道数=%1\n").arg(auido_input_format.channelCount()));

    emit LogSend(0,tr("当前录音的样本大小=%1\n").arg(auido_input_format.sampleSize()));

    emit LogSend(0,tr("当前录音的编码格式=%1\n").arg(auido_input_format.codec()));

    if(sampleRate<=0)
    {
        return -1;
    }

    audio_in = new QAudioInput(auido_input_format);
    connect(audio_in,SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged_input(QAudio::State)),Qt::QueuedConnection);
    audio_streamIn=audio_in->start(); //开始音频采集
    //关联音频读数据信号
    connect(audio_streamIn,SIGNAL(readyRead()),this,SLOT(audio_ReadyRead()),Qt::QueuedConnection);
    return 0;
}


//有音频信号可以读
void AudioGetThread::audio_ReadyRead()
{
    m_RecordConfig->audio_encode_mutex.lock();
    //生产者
    QByteArray byte=audio_streamIn->readAll();
    if(byte.size()>0)m_RecordConfig->AudioDataQueue.enqueue(byte);
    //qDebug()<<"input="<<videoaudioencode.audio_data_queue.size();
    m_RecordConfig->audio_encode_mutex.unlock();

}


//录音状态
void AudioGetThread::handleStateChanged_input(QAudio::State newState)
{
 switch (newState) {
     case QAudio::StoppedState:
         if (audio_in->error() != QAudio::NoError)
         {
             // Error handling
             qDebug()<<"录音出现错误.\n";
         }
         else
         {
             // Finished recording
             qDebug()<<"完成录音\n";
             audio_in->stop();
             audio_streamIn->close();
             delete audio_in;
             delete loop;
         }
         break;
     case QAudio::ActiveState:
         // Started recording - read from IO device
            qDebug()<<"开始从IO设备读取PCM声音数据.\n";
         break;
     default:
         // ... other cases as appropriate
         break;
 }
}


//开始运行
void AudioGetThread::run()
{
    //1. 音频设备初始化
    int ret=Audio_Init();

    if(ret<0)
    {
        LogSend(0,"音频采集线程已退出1...\r\n");
        return;
    }

    m_RecordConfig->AudioDataQueue.clear();

    loop=new QEventLoop;

    //开始执行事件循环
    loop->exec();
    loop->exit();

    //取消关联音频读数据信号
    audio_in->stop();
    LogSend(0,"音频采集线程已退出2...\r\n");
}

