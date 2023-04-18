#ifndef FFMPEG_ENCODETHREAD_H
#define FFMPEG_ENCODETHREAD_H
#include "Config.h"

//单个输出AVStream的包装器
typedef struct OutputStream
{
    AVStream *st;
    AVCodecContext *enc;

    /* 下一帧的点数*/
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
}OutputStream;



class ffmpeg_EncodeThread: public QThread
{
    Q_OBJECT
public:
    ffmpeg_EncodeThread(RecordConfig *config);
    bool m_run; //运行状态 false表示退出 true表示正常运行
protected:
    void run();
signals:
    void LogSend(int err,QString text);
private:
    struct RecordConfig *m_RecordConfig;
    unsigned char audio_write_buffer[2048];
    qint32 audio_write_cnt=0;
    QByteArray audio_buffer;//存放队列里的一帧音频数据

    int StartEncode();
    void ffmpeg_close_stream(OutputStream *ost);
    void ffmpeg_add_stream(OutputStream *ost, AVFormatContext *oc,AVCodec **codec,enum AVCodecID codec_id);
    void ffmpeg_open_video(AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg);
    AVFrame *ffmpeg_alloc_picture(enum AVPixelFormat pix_fmt, int width, int height);
    int ffmpeg_write_video_frame(AVFormatContext *oc, OutputStream *ost);
    AVFrame *get_video_frame(OutputStream *ost);
    void fill_yuv_image(AVFrame *pict, int frame_index,int width, int height);
    int ffmpeg_write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt);
    int ffmpeg_write_audio_frame(AVFormatContext *oc, OutputStream *ost);
    AVFrame *get_audio_frame(OutputStream *ost);
    int ffmpeg_open_audio(AVFormatContext *oc,
                           AVCodec *codec,
                           OutputStream *ost,
                           AVDictionary *opt_arg);

};

#endif // FFMPEG_ENCODETHREAD_H
