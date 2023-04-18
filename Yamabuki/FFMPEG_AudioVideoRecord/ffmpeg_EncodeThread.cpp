#include "ffmpeg_EncodeThread.h"


//关闭流
void ffmpeg_EncodeThread::ffmpeg_close_stream(OutputStream *ost)
{
    avcodec_free_context(&ost->enc);
    av_frame_free(&ost->frame);
    av_frame_free(&ost->tmp_frame);
    sws_freeContext(ost->sws_ctx);
    swr_free(&ost->swr_ctx);
}


//添加输出流
void ffmpeg_EncodeThread::ffmpeg_add_stream(OutputStream *ost, AVFormatContext *oc,AVCodec **codec,enum AVCodecID codec_id)
{
    AVCodecContext *c;
    *codec = avcodec_find_encoder(codec_id);
    ost->st = avformat_new_stream(oc,nullptr);
    ost->st->id = oc->nb_streams-1;
    c=avcodec_alloc_context3(*codec);
    ost->enc = c;
    switch ((*codec)->type)
    {
    case AVMEDIA_TYPE_AUDIO:
        //设置数据格式
        c->sample_fmt  = FFMPEG_AudioFormat;
        c->bit_rate    = AUDIO_BIT_RATE_SET;  //设置码率
        c->sample_rate = AUDIO_RATE_SET;  //音频采样率
        //设置采样通道
        c->channels= av_get_channel_layout_nb_channels(c->channel_layout);
        c->channel_layout = AUDIO_CHANNEL_SET; //AV_CH_LAYOUT_MONO 单声道   AV_CH_LAYOUT_STEREO 立体声
        c->channels=av_get_channel_layout_nb_channels(c->channel_layout); //通道数
        ost->st->time_base={1,c->sample_rate};

        //c->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL; //允许使用实验性AAC编码器-增加的
        break;

    case AVMEDIA_TYPE_VIDEO:
        c->codec_id = codec_id;
        //码率：影响体积，与体积成正比：码率越大，体积越大；码率越小，体积越小。
        c->bit_rate = m_RecordConfig->video_bit_rate; //设置码率400000   400kps   500000
        /*分辨率必须是2的倍数。 */
        c->width    = m_RecordConfig->ImageWidth;
        c->height   = m_RecordConfig->ImageHeight;

        /*时基：这是基本的时间单位，以秒为单位*/
        ost->st->time_base={1,m_RecordConfig->video_frame}; //按帧率计算时间基准
        c->framerate = {m_RecordConfig->video_frame,1}; //设置帧率

        c->time_base     = ost->st->time_base;
        c->gop_size      = 10; // 关键帧间隔 /* 最多每十二帧发射一帧内帧 */
        c->pix_fmt       = AV_PIX_FMT_YUV420P;  //固定设置为420P
        c->max_b_frames  = 0; // 不使用b帧
        if(c->codec_id == AV_CODEC_ID_MPEG1VIDEO)
        {
            c->mb_decision = 2;
        }
        // 预设：快速
        av_opt_set(c->priv_data,"preset", "superfast", 0);
    break;

    default:
        break;
    }

    /* 某些格式希望流头分开*/
    if(oc->oformat->flags & AVFMT_GLOBALHEADER)c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

AVFrame *ffmpeg_EncodeThread::ffmpeg_alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    picture=av_frame_alloc();
    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;
    /*为帧数据分配缓冲区*/
    av_frame_get_buffer(picture,32);
    return picture;
}

void ffmpeg_EncodeThread::ffmpeg_open_video(AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg)
{
    AVCodecContext *c = ost->enc;
    AVDictionary *opt = nullptr;
    av_dict_copy(&opt, opt_arg, 0);

    //H264编码器的一些参数设置
    c->qmin = 10;
    c->qmax = 51;
    //Optional Param
    c->max_b_frames = 0;

#if 1
             //下面设置两个参数影响编码延时，如果不设置，编码器默认会缓冲很多帧
            // Set H264 preset and tune
           // av_dict_set(&opt, "preset", "fast", 0);
           //   av_dict_set(&opt, "tune", "zerolatency", 0);
#else
             /**
              * ultrafast,superfast, veryfast, faster, fast, medium
              * slow, slower, veryslow, placebo.
              注意：这是x264编码速度的选项， 设置该参数可以降低编码延时
              */
            av_opt_set(c->priv_data,"preset","superfast",0);
#endif

    avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    ost->frame = ffmpeg_alloc_picture(c->pix_fmt, c->width,c->height);
    ost->tmp_frame = nullptr;
    /* 将流参数复制到多路复用器 */
    avcodec_parameters_from_context(ost->st->codecpar,c);
}


/*
准备图像数据
YUV422占用内存空间 = w * h * 2
YUV420占用内存空间 = width*height*3/2
*/
void ffmpeg_EncodeThread::fill_yuv_image(AVFrame *pict, int frame_index,int width, int height)
{
    unsigned int y_size=width*height;
    m_RecordConfig->video_encode_mutex.lock();
    m_RecordConfig->video_WaitConditon.wait(&m_RecordConfig->video_encode_mutex);
    memcpy(m_RecordConfig->video_yuv420p_buff_temp,
           m_RecordConfig->video_yuv420p_buff,
           m_RecordConfig->yuv420p_size);
    m_RecordConfig->video_encode_mutex.unlock();

    //将YUV数据拷贝到缓冲区  y_size=wXh
    memcpy(pict->data[0],m_RecordConfig->video_yuv420p_buff_temp,y_size);
    memcpy(pict->data[1],m_RecordConfig->video_yuv420p_buff_temp+y_size,y_size/4);
    memcpy(pict->data[2],m_RecordConfig->video_yuv420p_buff_temp+y_size+y_size/4,y_size/4);
}



//获取一帧视频
AVFrame *ffmpeg_EncodeThread::get_video_frame(OutputStream *ost)
{
    AVCodecContext *c = ost->enc;

    /* 检查我们是否要生成更多帧---判断是否结束录制 */
//      if(av_compare_ts(ost->next_pts, c->time_base,STREAM_DURATION, (AVRational){ 1, 1 }) >= 0)
//      return nullptr;

    /*当我们将帧传递给编码器时，它可能会保留对它的引用
    *内部； 确保我们在这里不覆盖它*/
    if (av_frame_make_writable(ost->frame) < 0) return nullptr;


    //获取图像
    //DTS（解码时间戳）和PTS（显示时间戳）
    fill_yuv_image(ost->frame, ost->next_pts, c->width, c->height);

    ost->frame->pts = ost->next_pts++;

    //视频帧添加水印
    return ost->frame;
}


int ffmpeg_EncodeThread::ffmpeg_write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt)
{
    /*将输出数据包时间戳值从编解码器重新调整为流时基 */

    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;
    /*将压缩的帧写入媒体文件*/
    return av_interleaved_write_frame(fmt_ctx, pkt);
}


/*
*编码一个视频帧并将其发送到多路复用器
*/
int ffmpeg_EncodeThread::ffmpeg_write_video_frame(AVFormatContext *oc, OutputStream *ost)
{
    int ret;
    AVCodecContext *c;
    AVFrame *frame;
    int got_packet = 0;
    AVPacket pkt;
    c=ost->enc;

    //获取一帧数据
    frame = get_video_frame(ost);
    if(frame==nullptr)
    {
        emit LogSend(-1,"视频帧制作失败...\n");
        return -1;
    }

    av_init_packet(&pkt);

    //编码图像
    ret=avcodec_encode_video2(c, &pkt, frame, &got_packet);
    if(ret < 0)
    {
        emit LogSend(-1,"视频帧编码出错...\n");
        return ret;
    }

    if(got_packet)
    {
        ret=ffmpeg_write_frame(oc, &c->time_base, ost->st, &pkt);
    }
    else
    {
        ret = 0;
    }

    if(ret < 0)
    {
        emit LogSend(-1,"写入视频帧时出错...\n");
        return ret;
    }
    return 0;
}

/*获取一帧音频数据 */
AVFrame *ffmpeg_EncodeThread::get_audio_frame(OutputStream *ost)
{
    AVFrame *frame = ost->tmp_frame;
    int16_t *q = (int16_t*)frame->data[0];

    /* 检查我们是否要生成更多帧----用于判断是否结束*/
//    if(av_compare_ts(ost->next_pts, ost->enc->time_base,
//                      STREAM_DURATION, (AVRational){ 1, 1 }) >= 0)
//    {
//      //  if(videoaudioencode.audio_data_queue.isEmpty())
//        return nullptr;
//    }

    // qDebug("frame->nb_samples=%d\n",frame->nb_samples); //2048
    // qDebug("ost->enc->channels=%d\n",ost->enc->channels); //1

    //判断缓冲区的数据是否足够2048字节
   if(audio_write_cnt+2048<=audio_buffer.size())
   {
       memcpy(audio_write_buffer,audio_buffer.data()+audio_write_cnt,2048);
       audio_write_cnt+=2048;  //每次累加2048
   }
   else
   {
       int temp_cnt=0;
       if(audio_buffer.size()-audio_write_cnt>0) //上一次缓冲区结尾还有数据,但是不足1024
       {
           memcpy(audio_write_buffer,audio_buffer.data()+audio_write_cnt,audio_buffer.size()-audio_write_cnt);
           temp_cnt=audio_buffer.size()-audio_write_cnt; //拷贝的字节数量
           audio_write_cnt=0; //缓冲区的数据已经写完了
         //  qDebug()<<"temp_cnt="<<temp_cnt;
       }
       //判断当前缓冲区是否有数据 ,没有数据就从队列里取出一帧
       //qDebug()<<"从队列里取出一帧数据.";

       //等待队列里有数据才能取出来
       while(m_RecordConfig->AudioDataQueue.isEmpty())
       {
           QThread::msleep(5);
           if(m_run==0)return nullptr;
       }
       //消费者 -从队列里取出数据
       m_RecordConfig->audio_encode_mutex.lock();
       audio_buffer=m_RecordConfig->AudioDataQueue.dequeue();

#ifdef PCM_DATA_SAVE
    pcm_data->write(audio_buffer);
#endif

       m_RecordConfig->audio_encode_mutex.unlock();
      // qDebug()<<"out_put="<<videoaudioencode.audio_data_queue.size();

       if(temp_cnt==0)
       {
           memcpy(audio_write_buffer,audio_buffer.data(),2048);
           audio_write_cnt=2048;
       }
       else
       {
           memcpy(audio_write_buffer+temp_cnt,audio_buffer.data(),2048-temp_cnt);
           audio_write_cnt=2048-temp_cnt;
       }
   }
    // qDebug()<<"audio_write_cnt="<<audio_write_cnt; AV_SAMPLE_FMT_FLTP

   //nb_samples: 此帧描述的音频样本数（每个通道）
   //channels:音频通道数
   //音频数据赋值  1024x2
    memcpy(q,audio_write_buffer,frame->nb_samples*sizeof(int16_t)*ost->enc->channels);

    frame->pts = ost->next_pts;
    ost->next_pts  += frame->nb_samples;
    return frame;
}


/*
 *编码一个音频帧并将其发送到多路复用器
 *编码完成后返回0
 */
int ffmpeg_EncodeThread::ffmpeg_write_audio_frame(AVFormatContext *oc, OutputStream *ost)
{
    AVCodecContext *c;
    AVPacket pkt; // data and size must be 0;
    AVFrame *frame;
    int ret;
    int got_packet;
    int dst_nb_samples;

    av_init_packet(&pkt);
    c = ost->enc;

    frame=get_audio_frame(ost);
    if(frame==nullptr)return -1;

    if(frame)
    {
        /*使用重采样器将样本从本机格式转换为目标编解码器格式*/
         /*计算样本的目标数量--AAC固定1024字节*/
            dst_nb_samples = av_rescale_rnd(swr_get_delay(ost->swr_ctx, c->sample_rate) + frame->nb_samples,
                                            c->sample_rate, c->sample_rate, AV_ROUND_UP);
            av_assert0(dst_nb_samples == frame->nb_samples);

        /*当我们将帧传递给编码器时，它可能会保留对它的引用
         *内部；
         *确保我们不会在这里覆盖它
         */
        ret = av_frame_make_writable(ost->frame);
        if (ret < 0)
        {
            return ret;
        }

        /*转换为目标格式 */
        swr_convert(ost->swr_ctx,
                    ost->frame->data,
                    dst_nb_samples,
                    (const uint8_t **)frame->data,
                    frame->nb_samples);

        frame = ost->frame;
        //c->time_base=(44100,1)
        //c->sample_rate=44100
        //ost->samples_counts每次累加1024
        frame->pts = av_rescale_q(ost->samples_count,{1,c->sample_rate},c->time_base);
        ost->samples_count += dst_nb_samples; //每次固定累加1024
    }

    avcodec_encode_audio2(c, &pkt, frame, &got_packet);
    if(got_packet)
    {
        ret=ffmpeg_write_frame(oc, &c->time_base, ost->st, &pkt);
        if (ret < 0)
        {
            return ret;
        }
    }
    return 0;
}


AVFrame *ffmpeg_alloc_audio_frame(enum AVSampleFormat sample_fmt,
                                  uint64_t channel_layout,
                                  int sample_rate, int nb_samples)
{
    AVFrame *frame = av_frame_alloc();
    int ret;
    frame->format = sample_fmt;
    frame->channel_layout = channel_layout;
    frame->sample_rate = sample_rate;
    frame->nb_samples = nb_samples;

    if (nb_samples) {
        ret = av_frame_get_buffer(frame, 0);
        if (ret < 0) {
            qDebug("Error allocating an audio buffer\n");
            exit(1);
        }
    }
    return frame;
}



int ffmpeg_EncodeThread::ffmpeg_open_audio(AVFormatContext *oc,
                       AVCodec *codec,
                       OutputStream *ost,
                       AVDictionary *opt_arg)
{
    AVCodecContext *c;
    int nb_samples;
    int ret;
    AVDictionary *opt = nullptr;
    c = ost->enc;
    /* open it */
    av_dict_copy(&opt, opt_arg, 0);

    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    nb_samples = 1024;

    ost->frame=ffmpeg_alloc_audio_frame(c->sample_fmt, c->channel_layout,
                                       c->sample_rate, nb_samples);
    ost->tmp_frame=ffmpeg_alloc_audio_frame(FFMPEG_AudioFormat, c->channel_layout,
                                       c->sample_rate, nb_samples);
    /*将流参数复制到多路复用器 */
    ret=avcodec_parameters_from_context(ost->st->codecpar, c);
    if(ret < 0)
    {
       qDebug("无法复制流参数\n");
       return ret;
    }

    /* 创建重采样器上下文 */
    ost->swr_ctx = swr_alloc();
    if(!ost->swr_ctx)
    {
        qDebug("无法分配重采样器上下文\n");
        return -1;
    }
    /* 设定选项 */
    av_opt_set_int       (ost->swr_ctx, "in_channel_count",   c->channels,       0);
    av_opt_set_int       (ost->swr_ctx, "in_sample_rate",     c->sample_rate,    0);
    av_opt_set_sample_fmt(ost->swr_ctx, "in_sample_fmt",      AV_SAMPLE_FMT_S16, 0);
    av_opt_set_int       (ost->swr_ctx, "out_channel_count",  c->channels,       0);
    av_opt_set_int       (ost->swr_ctx, "out_sample_rate",    c->sample_rate,    0);
    av_opt_set_sample_fmt(ost->swr_ctx, "out_sample_fmt",     c->sample_fmt,     0);

    qDebug("音频通道数=%d\n",c->channels);
    qDebug("音频采样率=%d\n",c->sample_rate);

    /* 初始化重采样上下文 */
    if ((ret = swr_init(ost->swr_ctx)) < 0)
    {
        qDebug("无法初始化重采样上下文\n");
        return -1;
    }
    return 0;
}


//开始编码
int ffmpeg_EncodeThread::StartEncode()
{
     //基本变量结构的定义
     int ret = -1;
     OutputStream video_st;
     AVOutputFormat *fmt=nullptr;
     AVFormatContext *oc=nullptr;
     AVCodec *video_codec=nullptr;
     AVDictionary *opt = nullptr;

     int have_video=0;
     int encode_video=0;
     memset(&video_st,0,sizeof(OutputStream));

     //音频相关
     OutputStream audio_st;
     AVCodec *audio_codec=nullptr;
     int have_audio=0;
     int encode_audio=0;
     memset(&audio_st,0,sizeof(OutputStream));

     //获取当前时间用来设置当前视频文件的名称
     QDateTime dateTime(QDateTime::currentDateTime());
     //时间效果: 2020-03-05 16:25::04 周四
     QByteArray VideoSavePath="";
     VideoSavePath+=m_RecordConfig->VideoSavePath; //视频保存路径
     VideoSavePath+="/";
     VideoSavePath+=dateTime.toString("yyyy-MM-dd-hh-mm-ss");
     VideoSavePath+=".mp4";

     //保存最终的地址
     QByteArray ffmpegOutPath;


     //本地录制
     if(m_RecordConfig->Mode==0)
     {
         ret=avformat_alloc_output_context2(&oc,nullptr,nullptr,VideoSavePath.data()); //存放到文件
         if(ret!=0)
         {
             emit LogSend(-1,tr("地址无法打开:%1\n").arg(VideoSavePath.data()));
             return ret;
         }
         emit LogSend(0,tr("视频保存路径:%1\n").arg(VideoSavePath.data()));

         //如果文件已经存在
         if(QFile(VideoSavePath).exists())
         {
             //删除原来的文件
            QFile::remove(VideoSavePath);
         }

         ffmpegOutPath=VideoSavePath;
     }
     //推流
     else if(m_RecordConfig->Mode==1)
     {
         QByteArray FormatType;
         ffmpegOutPath=m_RecordConfig->PushStreamAddr;
         //如果是rtsp
         if(ffmpegOutPath.contains("rtsp"))
         {
             FormatType="rtsp";
         }
         //如果是rtmp
         else if(ffmpegOutPath.contains("rtmp"))
         {
             FormatType="flv";
         }

         ret=avformat_alloc_output_context2(&oc,nullptr,FormatType,ffmpegOutPath.data()); //发布到网络
         if(ret!=0)
         {
             LogSend(-1,tr("地址无法打开:%1\n").arg(ffmpegOutPath.data()));
             return ret;
         }
         emit LogSend(0,tr("推流地址:%1\n").arg(ffmpegOutPath.data()));
     }

     //设置视频编码格式
     fmt=oc->oformat;
     fmt->video_codec=AV_CODEC_ID_H264;  //视频编码为H264
     fmt->audio_codec=AV_CODEC_ID_AAC;   //音频编码为AAC


     //只要图像
     if(m_RecordConfig->input_DataConf==0)
     {
         have_video = 1;
         encode_video = 1;
         //配置视频流
         ffmpeg_add_stream(&video_st,oc,&video_codec,fmt->video_codec);
     }
     //图像+音频
     else if(m_RecordConfig->input_DataConf==1)
     {
         have_video = 1;
         encode_video = 1;
         have_audio = 1;
         encode_audio = 1;
         ffmpeg_add_stream(&video_st,oc,&video_codec,fmt->video_codec);
         ffmpeg_add_stream(&audio_st,oc,&audio_codec,fmt->audio_codec);
     }


     //配置视频参数
     if (have_video)
     {
         ffmpeg_open_video(video_codec,&video_st,opt);
     }
     if (have_audio)
     {
         ret=ffmpeg_open_audio(oc,audio_codec,&audio_st,opt);
         if (ret < 0)
         {
             LogSend(-1,tr("初始化失败:%1\n").arg(ffmpegOutPath.data()));
             return ret;
         }
     }

     //打印当前流的编码信息
      av_dump_format(oc, 0, ffmpegOutPath.data(), 1);


     //打开输出文件
     if(!(fmt->flags & AVFMT_NOFILE))
     {
         ret = avio_open(&oc->pb, ffmpegOutPath.data(), AVIO_FLAG_WRITE);
         if (ret < 0)
         {
             LogSend(-1,tr("无法打开流:%1\n").arg(ffmpegOutPath.data()));
             return ret;
         }
     }

     //编写流头
     ret=avformat_write_header(oc,&opt);
     if(ret<0)
     {
         LogSend(-1,tr("输出文件打开出错:%1\n").arg(ffmpegOutPath.data()));
         return ret;
     }

     //参考值清0
     video_st.next_pts=0;
     audio_st.next_pts=0;

     //开始编码
     while(m_run)
     {
         /*选择要编码的流*/
         if(encode_video &&(!encode_audio || av_compare_ts(video_st.next_pts, video_st.enc->time_base,audio_st.next_pts, audio_st.enc->time_base) <= 0))
         {
             //写视频帧
             ret=ffmpeg_write_video_frame(oc,&video_st);
             if(ret<0)
             {
                 break;
             }
         }
         else
         {
             //写音频帧
             ret=ffmpeg_write_audio_frame(oc,&audio_st);
             if(ret<0)
             {
                 break;
             }
         }
     }

     av_write_trailer(oc);

     /*关闭流*/
     if (have_video)
         ffmpeg_close_stream(&video_st);
     if (have_audio)
         ffmpeg_close_stream(&audio_st);

     if (!(fmt->flags & AVFMT_NOFILE))
         avio_closep(&oc->pb);

     avformat_free_context(oc);

     LogSend(0,"编程线程已退出...\n");
     return ret;
}


//设置配置参数
ffmpeg_EncodeThread::ffmpeg_EncodeThread(RecordConfig *config)
{
    m_RecordConfig=config;
}


//线程开始运行
void ffmpeg_EncodeThread::run()
{
    //开始编码
    StartEncode();
}

