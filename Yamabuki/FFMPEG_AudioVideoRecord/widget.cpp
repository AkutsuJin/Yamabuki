#include "widget.h"
#include "ui_widget.h"



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("基于Qt+FFMPEG设计的录屏、推流客户端");

    //更新麦克风与摄像头设备
    on_pushButton_UpdateDevInfo_clicked();

    //参数配置结构
    m_RecordConfig=new struct RecordConfig;

    //摄像头采集线程
    m_VideoGetThread=new VideoGetThread(m_RecordConfig);

    //桌面图像采集线程
    m_DesktopImageGetThread=new DesktopImageGetThread(m_RecordConfig);

    //音频采集线程
    m_AudioGetThread =new AudioGetThread(m_RecordConfig);

    //ffmpeg编码线程
    m_ffmpeg_EncodeThread=new ffmpeg_EncodeThread(m_RecordConfig);


    if(m_VideoGetThread==nullptr || m_ffmpeg_EncodeThread==nullptr)
    {
        slot_LogDisplay(0,"线程new失败...\n");
        ui->pushButton_start->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);
        return;
    }



    //关联摄像头图像输出到UI界面
    connect(m_VideoGetThread,SIGNAL(VideoDataOutput(QImage )),ui->widget_video,SLOT(slotSetOneFrame(QImage)));

    //关联桌面图像输出到UI界面
    connect(m_DesktopImageGetThread,SIGNAL(VideoDataOutput(QImage )),ui->widget_video,SLOT(slotSetOneFrame(QImage)));

    //关联日志信号
    connect(m_VideoGetThread,SIGNAL(LogSend(int,QString)),this,SLOT(slot_LogDisplay(int,QString)));

    //关联日志信号
    connect(m_ffmpeg_EncodeThread,SIGNAL(LogSend(int,QString)),this,SLOT(slot_LogDisplay(int,QString)));

    //关联日志信号
    connect(m_DesktopImageGetThread,SIGNAL(LogSend(int,QString)),this,SLOT(slot_LogDisplay(int,QString)));

    //关联日志信号
    connect(m_AudioGetThread,SIGNAL(LogSend(int,QString)),this,SLOT(slot_LogDisplay(int,QString)));

}


Widget::~Widget()
{
    delete ui;
}


//开始（推流或者保存）
void Widget::on_pushButton_start_clicked()
{
    //1. 判断模式
    //本地录制
    if(ui->radioButton_save_stream->isChecked())
    {
         m_RecordConfig->Mode=0;
         slot_LogDisplay(0,QString("模式:本地录制\n"));
    }
    //推流
    else if(ui->radioButton_send_stream->isChecked())
    {
         m_RecordConfig->Mode=1;
         slot_LogDisplay(0,QString("模式:推流\n"));
    }
    //默认模式--本地录制
    else
    {
        m_RecordConfig->Mode=0;
        slot_LogDisplay(0,QString("模式:本地录制\n"));
    }

    //2. 解析分辨率
    QString input_size=ui->comboBox_WxH->currentText();
    if(input_size.contains("x",Qt::CaseSensitive))
    {
        int input_w=input_size.section('x',0,0).toInt();
        int input_h=input_size.section('x',1,1).toInt();
        if(input_w<=0||input_h<=0)
        {
            QMessageBox::warning(this,"提示","请按格式填分辨率参数!",QMessageBox::Ok,QMessageBox::Ok);
            return;
        }
        else
        {
            //保证输入的宽高是2的倍数
            while(input_w%2!=0)
            {
                input_w++;
            }
            while(input_h%2!=0)
            {
                input_h++;
            }

            slot_LogDisplay(0,QString("分辨率:%1x%2\n").arg(input_w).arg(input_h));
            m_RecordConfig->ImageWidth=input_w;
            m_RecordConfig->ImageHeight=input_h;

        }
    }
    else
    {
        QMessageBox::warning(this,"提示","请按格式填分辨率参数!",QMessageBox::Ok,QMessageBox::Ok);
        return;
    }


    //3. 计算RGB24占用大小，并申请空间
    m_RecordConfig->grb24_size=m_RecordConfig->ImageWidth*m_RecordConfig->ImageHeight*3;
    m_RecordConfig->rgb_buffer=(unsigned char *)malloc(m_RecordConfig->grb24_size);
    if(m_RecordConfig->rgb_buffer==nullptr)
    {
        QMessageBox::warning(this,"提示","RGB24空间申请失败!",QMessageBox::Ok,QMessageBox::Ok);
        return;
    }

    //4.计算YUV42OP格式占用大小，并申请空间
    m_RecordConfig->yuv420p_size=m_RecordConfig->ImageWidth*m_RecordConfig->ImageHeight*3/2;
    m_RecordConfig->video_yuv420p_buff=(unsigned char *)malloc(m_RecordConfig->yuv420p_size);
    m_RecordConfig->video_yuv420p_buff_temp=(unsigned char *)malloc(m_RecordConfig->yuv420p_size);
    if(m_RecordConfig->video_yuv420p_buff==nullptr ||
       m_RecordConfig->video_yuv420p_buff_temp==nullptr)
    {
        QMessageBox::warning(this,"提示","YUV42OP空间申请失败!",QMessageBox::Ok,QMessageBox::Ok);
        return;
    }

    //5. 配置图像来源
    //摄像头
    if(ui->comboBox_video_input->currentIndex()==0)
    {
        //来源摄像头
        m_RecordConfig->ImageInptSrc=0;

        if(video_dev_list.size()<=0)
        {
            QMessageBox::warning(this,"提示","没有摄像头可用!",QMessageBox::Ok,QMessageBox::Ok);
            return;
        }
        //得到选择的摄像头
        m_RecordConfig->camera=video_dev_list.at(ui->comboBox_video_list->currentIndex());

         slot_LogDisplay(0,QString("图像源:摄像头\n"));
    }
    //桌面
    else if(ui->comboBox_video_input->currentIndex()==1)
    {
        //来源桌面
        m_RecordConfig->ImageInptSrc=1;

        //桌面的页面
        m_RecordConfig->desktop_page=ui->spinBox_DesktopPage->value();

        slot_LogDisplay(0,QString("图像源:桌面\n"));
    }

    //6. 配置音频来源
    //如果需要声音+画面
    if(ui->comboBox_data_conf->currentIndex()==1)
    {
        //需要声音+画面
        m_RecordConfig->input_DataConf=1;
        slot_LogDisplay(0,QString("捕获:声音+画面\n"));


        if(audio_dev_list.size()<=0)
        {
            QMessageBox::warning(this,"提示","没有音频设备可用!",QMessageBox::Ok,QMessageBox::Ok);
            return;
        }

        //麦克风
        if(ui->comboBox_audio_input->currentIndex()==0)
        {

        }
        //扬声器
        else if(ui->comboBox_audio_input->currentIndex()==1)
        {

        }
        //麦克风+扬声器
        else if(ui->comboBox_audio_input->currentIndex()==2)
        {

        }

        m_RecordConfig->audio=audio_dev_list.at(ui->comboBox_audio_list->currentIndex());
    }
    else
    {
        //只要画面
        m_RecordConfig->input_DataConf=0;
        slot_LogDisplay(0,QString("捕获:只要画面\n"));
    }

    //7. 配置码率
    m_RecordConfig->video_bit_rate=ui->comboBox_rate->currentText().toInt();
    if(m_RecordConfig->video_bit_rate<1000)
    {
        QMessageBox::warning(this,"提示","码率配置有误!",QMessageBox::Ok,QMessageBox::Ok);
        return;
    }
    slot_LogDisplay(0,QString("码率:%1\n").arg(m_RecordConfig->video_bit_rate));

    //8. 配置帧率
    m_RecordConfig->video_frame=ui->spinBox_framCnt->value();
    if(m_RecordConfig->video_frame<10)
    {
        QMessageBox::warning(this,"提示","帧率配置有误!",QMessageBox::Ok,QMessageBox::Ok);
        return;
    }
    slot_LogDisplay(0,QString("帧率:%1\n").arg(m_RecordConfig->video_frame));

    //9. 推流地址
    m_RecordConfig->PushStreamAddr=ui->lineEdit_PushStreamAddr->text().toUtf8();

    //如果是选择推流模式
    if(m_RecordConfig->Mode==1)
    {
        if(m_RecordConfig->PushStreamAddr.isEmpty())
        {
            QMessageBox::warning(this,"提示","推流地址未填写!",QMessageBox::Ok,QMessageBox::Ok);
            return;
        }
        else
        {
            //判断地址是否合理
            if(m_RecordConfig->PushStreamAddr.contains("rtsp")||
               m_RecordConfig->PushStreamAddr.contains("rtmp"))
            {
                //正常了
                slot_LogDisplay(0,QString("推流地址:%1\n").arg(m_RecordConfig->PushStreamAddr.data()));
            }
            else
            {
                QMessageBox::warning(this,"提示","推流地址格式有误!",QMessageBox::Ok,QMessageBox::Ok);
                return;
            }
        }
    }

    //10. 视频保存路径
    m_RecordConfig->VideoSavePath=ui->lineEdit_VideoSavePath->text();

    //如果是本地录制
    if(m_RecordConfig->Mode==0)
    {
        if(m_RecordConfig->VideoSavePath.isEmpty())
        {
            QMessageBox::warning(this,"提示","视频保存目录未选择!",QMessageBox::Ok,QMessageBox::Ok);
            return;
        }
        else
        {
            //如果目录不存在
            if(!QDir(m_RecordConfig->VideoSavePath).exists())
            {
                QMessageBox::warning(this,"提示","视频保存目录不存在!",QMessageBox::Ok,QMessageBox::Ok);
                return;
            }
        }
    }

    //11. 开启采集线程

    //如果画面来源选择摄像头
    if(m_RecordConfig->ImageInptSrc==0)
    {
        //开启视频采集线程
        m_VideoGetThread->start();
    }
    //如果画面来源选择桌面
    else if(m_RecordConfig->ImageInptSrc==1)
    {
        m_DesktopImageGetThread->m_run=1;
        m_DesktopImageGetThread->start();
    }

    //如果需要采集音频
    if(m_RecordConfig->input_DataConf==1)
    {
        //开启音频采集线程
        m_AudioGetThread->start();
    }

    //开启编码线程
    m_ffmpeg_EncodeThread->m_run=1;
    m_ffmpeg_EncodeThread->start();

    //禁止开始按钮重复点击
    ui->pushButton_start->setEnabled(false);
}



//停止 （推流或者保存）
void Widget::on_pushButton_stop_clicked()
{
    //恢复开始按钮点击
    ui->pushButton_start->setEnabled(true);

    //退出视频采集线程
    m_VideoGetThread->quit();
    m_VideoGetThread->wait();

    //退出音频采集线程
    m_AudioGetThread->quit();
    m_AudioGetThread->wait();

    //退出桌面图像采集线程
    m_DesktopImageGetThread->m_run=0;
    m_DesktopImageGetThread->quit();
    m_DesktopImageGetThread->wait();


    //退出编码线程
    m_ffmpeg_EncodeThread->m_run=0;
    m_RecordConfig->video_WaitConditon.wakeAll();
    m_ffmpeg_EncodeThread->quit();
    m_ffmpeg_EncodeThread->wait();


    //释放RGB24和YUV占用的空间
    free(m_RecordConfig->video_yuv420p_buff);
    free(m_RecordConfig->video_yuv420p_buff_temp);
    free(m_RecordConfig->rgb_buffer);

}


//更新设备列表
void Widget::on_pushButton_UpdateDevInfo_clicked()
{
    QComboBox *comboBox_audio=ui->comboBox_audio_list;
    QComboBox *comboBox_video=ui->comboBox_video_list;

    /*1. 获取声卡设备列表*/
    audio_dev_list.clear();
    comboBox_audio->clear();
    foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        audio_dev_list.append(deviceInfo);
        comboBox_audio->addItem(deviceInfo.deviceName());
   }

    /*2. 获取摄像头列表*/
    video_dev_list.clear();
    comboBox_video->clear();
    video_dev_list=QCameraInfo::availableCameras();
    for(int i=0;i<video_dev_list.size();i++)
    {
        comboBox_video->addItem(video_dev_list.at(i).deviceName());
    }

    //没有麦克风设备
    if(audio_dev_list.size()==0)
    {
        slot_LogDisplay(0,"未查询到可用的麦克风设备.\n");
    }

    //没有摄像头设备
    if(video_dev_list.size()==0)
    {
        slot_LogDisplay(0,"未查询到可用的摄像头.\n");
    }
}



//日志显示
//int err  状态值.用来做一些特殊处理
//QString text 日志文本
void Widget::slot_LogDisplay(int err,QString text)
{
    ui->plainTextEdit_log->insertPlainText(text);
    //移动滚动条到底部
    QScrollBar *scrollbar = ui->plainTextEdit_log->verticalScrollBar();
    if(scrollbar)
    {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}


//窗口关闭事件
void Widget::closeEvent(QCloseEvent *event)
{
    //停止采集与编码线程
    if(!ui->pushButton_start->isEnabled())
    {
       on_pushButton_stop_clicked();
    }

    //释放其他空间
    delete m_VideoGetThread;
    delete m_ffmpeg_EncodeThread;
    delete m_RecordConfig;

    event->accept(); //接受事件
}


//图像输入源发生改变
void Widget::on_comboBox_video_input_activated(int index)
{
    //摄像头
    if(index==0)
    {
        //调整默认分辨率码率
        ui->comboBox_rate->setCurrentIndex(0);
        ui->comboBox_WxH->setCurrentIndex(0);
        ui->spinBox_framCnt->setValue(25);
    }
    //桌面
    else if(index==1)
    {
        //调整默认分辨率码率
        ui->comboBox_rate->setCurrentIndex(1);
        ui->comboBox_WxH->setCurrentIndex(1);
        ui->spinBox_framCnt->setValue(10);
    }
}


//选择视频存放路径
void Widget::on_pushButton_selectPath_clicked()
{
    QString path_dir="./";

    QString inpt_path=ui->lineEdit_VideoSavePath->text();

    //系统的视频目录
    QStringList path_list=QStandardPaths::standardLocations(QStandardPaths::MoviesLocation);

    //如果为空
    if(inpt_path.isEmpty())
    {
        if(path_list.size()>0)
        {
            path_dir=path_list.at(0);
        }
    }
    else
    {
        //如果路径不存在
        if(!QDir(inpt_path).exists())
        {
            if(path_list.size()>0)
            {
                path_dir=path_list.at(0);
            }
        }
        else
        {
            //使用之前用户输入的路径
            path_dir=inpt_path;
        }
    }

    QString dir=QFileDialog::getExistingDirectory(this,"选择视频保存目录",path_dir,
    QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);

    //不为空
    if(!dir.isEmpty())
    {
        ui->lineEdit_VideoSavePath->setText(dir);
    }
}



