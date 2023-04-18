#ifndef WIDGET_H
#define WIDGET_H
#include "Config.h"
#include "VideoGetThread.h"
#include "VideoFrameDisplay.h"
#include "ffmpeg_EncodeThread.h"
#include "DesktopImageGetThread.h"
#include "AudioGetThread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QList<QAudioDeviceInfo> audio_dev_list;
    QList<QCameraInfo> video_dev_list;

private slots:
    void slot_LogDisplay(int err,QString text);

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_UpdateDevInfo_clicked();

    void on_pushButton_selectPath_clicked();

    void on_comboBox_video_input_activated(int index);

protected:
    void closeEvent(QCloseEvent *event); //窗口关闭事
private:
    Ui::Widget *ui;

    //视频采集线程
    VideoGetThread *m_VideoGetThread;
    //桌面画面采集线程
    DesktopImageGetThread *m_DesktopImageGetThread;
    //音频采集线程
    AudioGetThread *m_AudioGetThread;

    //ffmpeg编码线程
    ffmpeg_EncodeThread *m_ffmpeg_EncodeThread;

    //录制视频的全局参数配置
    struct RecordConfig *m_RecordConfig;
};
#endif // WIDGET_H

