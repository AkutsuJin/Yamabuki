/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "videoframedisplay.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox_video_input;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QComboBox *comboBox_audio_input;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboBox_rate;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpinBox *spinBox_framCnt;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QComboBox *comboBox_WxH;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_10;
    QComboBox *comboBox_data_conf;
    QSpacerItem *horizontalSpacer_17;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_9;
    QLineEdit *lineEdit_PushStreamAddr;
    QHBoxLayout *horizontalLayout_16;
    QPushButton *pushButton_selectPath;
    QLineEdit *lineEdit_VideoSavePath;
    QSpacerItem *horizontalSpacer_18;
    QHBoxLayout *horizontalLayout_12;
    QSpacerItem *horizontalSpacer_14;
    QRadioButton *radioButton_send_stream;
    QSpacerItem *horizontalSpacer_16;
    QRadioButton *radioButton_save_stream;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *verticalSpacer;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QComboBox *comboBox_video_list;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QComboBox *comboBox_audio_list;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QSpinBox *spinBox_DesktopPage;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_UpdateDevInfo;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_12;
    QPushButton *pushButton_start;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *pushButton_stop;
    QSpacerItem *horizontalSpacer_13;
    QHBoxLayout *horizontalLayout_11;
    VideoFrameDisplay *widget_video;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QPlainTextEdit *plainTextEdit_log;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(892, 722);
        verticalLayout_3 = new QVBoxLayout(Widget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMaximumSize(QSize(16777215, 370));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        comboBox_video_input = new QComboBox(tab);
        comboBox_video_input->addItem(QString());
        comboBox_video_input->addItem(QString());
        comboBox_video_input->setObjectName(QString::fromUtf8("comboBox_video_input"));

        horizontalLayout->addWidget(comboBox_video_input);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_9->addWidget(label_8);

        comboBox_audio_input = new QComboBox(tab);
        comboBox_audio_input->addItem(QString());
        comboBox_audio_input->addItem(QString());
        comboBox_audio_input->addItem(QString());
        comboBox_audio_input->setObjectName(QString::fromUtf8("comboBox_audio_input"));

        horizontalLayout_9->addWidget(comboBox_audio_input);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_10);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboBox_rate = new QComboBox(tab);
        comboBox_rate->addItem(QString());
        comboBox_rate->addItem(QString());
        comboBox_rate->setObjectName(QString::fromUtf8("comboBox_rate"));

        horizontalLayout_2->addWidget(comboBox_rate);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        spinBox_framCnt = new QSpinBox(tab);
        spinBox_framCnt->setObjectName(QString::fromUtf8("spinBox_framCnt"));
        spinBox_framCnt->setValue(25);

        horizontalLayout_3->addWidget(spinBox_framCnt);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        comboBox_WxH = new QComboBox(tab);
        comboBox_WxH->addItem(QString());
        comboBox_WxH->addItem(QString());
        comboBox_WxH->setObjectName(QString::fromUtf8("comboBox_WxH"));
        comboBox_WxH->setEditable(true);

        horizontalLayout_4->addWidget(comboBox_WxH);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_10 = new QLabel(tab);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_14->addWidget(label_10);

        comboBox_data_conf = new QComboBox(tab);
        comboBox_data_conf->addItem(QString());
        comboBox_data_conf->addItem(QString());
        comboBox_data_conf->setObjectName(QString::fromUtf8("comboBox_data_conf"));

        horizontalLayout_14->addWidget(comboBox_data_conf);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_17);


        verticalLayout->addLayout(horizontalLayout_14);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_9 = new QLabel(tab);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_13->addWidget(label_9);

        lineEdit_PushStreamAddr = new QLineEdit(tab);
        lineEdit_PushStreamAddr->setObjectName(QString::fromUtf8("lineEdit_PushStreamAddr"));

        horizontalLayout_13->addWidget(lineEdit_PushStreamAddr);


        verticalLayout->addLayout(horizontalLayout_13);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        pushButton_selectPath = new QPushButton(tab);
        pushButton_selectPath->setObjectName(QString::fromUtf8("pushButton_selectPath"));

        horizontalLayout_16->addWidget(pushButton_selectPath);

        lineEdit_VideoSavePath = new QLineEdit(tab);
        lineEdit_VideoSavePath->setObjectName(QString::fromUtf8("lineEdit_VideoSavePath"));

        horizontalLayout_16->addWidget(lineEdit_VideoSavePath);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_18);


        verticalLayout->addLayout(horizontalLayout_16);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_14);

        radioButton_send_stream = new QRadioButton(tab);
        radioButton_send_stream->setObjectName(QString::fromUtf8("radioButton_send_stream"));

        horizontalLayout_12->addWidget(radioButton_send_stream);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_16);

        radioButton_save_stream = new QRadioButton(tab);
        radioButton_save_stream->setObjectName(QString::fromUtf8("radioButton_save_stream"));
        radioButton_save_stream->setChecked(true);

        horizontalLayout_12->addWidget(radioButton_save_stream);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_15);


        verticalLayout->addLayout(horizontalLayout_12);

        verticalSpacer = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(tab, QString());
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        comboBox_video_list = new QComboBox(widget);
        comboBox_video_list->setObjectName(QString::fromUtf8("comboBox_video_list"));

        horizontalLayout_5->addWidget(comboBox_video_list);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_7->addWidget(label_6);

        comboBox_audio_list = new QComboBox(widget);
        comboBox_audio_list->setObjectName(QString::fromUtf8("comboBox_audio_list"));

        horizontalLayout_7->addWidget(comboBox_audio_list);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_8);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        spinBox_DesktopPage = new QSpinBox(widget);
        spinBox_DesktopPage->setObjectName(QString::fromUtf8("spinBox_DesktopPage"));

        horizontalLayout_8->addWidget(spinBox_DesktopPage);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        pushButton_UpdateDevInfo = new QPushButton(widget);
        pushButton_UpdateDevInfo->setObjectName(QString::fromUtf8("pushButton_UpdateDevInfo"));

        horizontalLayout_6->addWidget(pushButton_UpdateDevInfo);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        tabWidget->addTab(widget, QString());

        verticalLayout_3->addWidget(tabWidget);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_12);

        pushButton_start = new QPushButton(Widget);
        pushButton_start->setObjectName(QString::fromUtf8("pushButton_start"));

        horizontalLayout_10->addWidget(pushButton_start);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_11);

        pushButton_stop = new QPushButton(Widget);
        pushButton_stop->setObjectName(QString::fromUtf8("pushButton_stop"));

        horizontalLayout_10->addWidget(pushButton_stop);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_13);


        verticalLayout_3->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        widget_video = new VideoFrameDisplay(Widget);
        widget_video->setObjectName(QString::fromUtf8("widget_video"));

        horizontalLayout_11->addWidget(widget_video);

        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(300, 16777215));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        plainTextEdit_log = new QPlainTextEdit(groupBox);
        plainTextEdit_log->setObjectName(QString::fromUtf8("plainTextEdit_log"));
        plainTextEdit_log->setReadOnly(true);

        verticalLayout_4->addWidget(plainTextEdit_log);


        horizontalLayout_11->addWidget(groupBox);


        verticalLayout_3->addLayout(horizontalLayout_11);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        label->setText(QApplication::translate("Widget", "\347\224\273\351\235\242\346\272\220:", nullptr));
        comboBox_video_input->setItemText(0, QApplication::translate("Widget", "\346\221\204\345\203\217\345\244\264", nullptr));
        comboBox_video_input->setItemText(1, QApplication::translate("Widget", "\346\241\214\351\235\242", nullptr));

        label_8->setText(QApplication::translate("Widget", "\345\243\260\351\237\263\346\272\220:", nullptr));
        comboBox_audio_input->setItemText(0, QApplication::translate("Widget", "\351\272\246\345\205\213\351\243\216", nullptr));
        comboBox_audio_input->setItemText(1, QApplication::translate("Widget", "\346\211\254\345\243\260\345\231\250", nullptr));
        comboBox_audio_input->setItemText(2, QApplication::translate("Widget", "\351\272\246\345\205\213\351\243\216+\346\211\254\345\243\260\345\231\250", nullptr));

        label_2->setText(QApplication::translate("Widget", "\347\240\201\347\216\207:", nullptr));
        comboBox_rate->setItemText(0, QApplication::translate("Widget", "400000", nullptr));
        comboBox_rate->setItemText(1, QApplication::translate("Widget", "3000000", nullptr));

        label_3->setText(QApplication::translate("Widget", "\345\270\247\347\216\207:", nullptr));
        label_4->setText(QApplication::translate("Widget", "\345\210\206\350\276\250\347\216\207:", nullptr));
        comboBox_WxH->setItemText(0, QApplication::translate("Widget", "640x480", nullptr));
        comboBox_WxH->setItemText(1, QApplication::translate("Widget", "1280x720", nullptr));

        label_10->setText(QApplication::translate("Widget", "\346\225\260\346\215\256\346\272\220\351\205\215\347\275\256:", nullptr));
        comboBox_data_conf->setItemText(0, QApplication::translate("Widget", "\345\217\252\350\246\201\347\224\273\351\235\242", nullptr));
        comboBox_data_conf->setItemText(1, QApplication::translate("Widget", "\347\224\273\351\235\242+\345\243\260\351\237\263", nullptr));

        label_9->setText(QApplication::translate("Widget", " \346\265\201\345\252\222\344\275\223\345\234\260\345\235\200:", nullptr));
        lineEdit_PushStreamAddr->setText(QApplication::translate("Widget", "rtmp://127.0.0.1:8888/live/video01", nullptr));
        pushButton_selectPath->setText(QApplication::translate("Widget", "\350\247\206\351\242\221\345\255\230\346\224\276\350\267\257\345\276\204", nullptr));
        lineEdit_VideoSavePath->setText(QString());
        radioButton_send_stream->setText(QApplication::translate("Widget", "\346\216\250\346\265\201", nullptr));
        radioButton_save_stream->setText(QApplication::translate("Widget", "\346\234\254\345\234\260\345\275\225\345\210\266", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Widget", " \345\217\202\346\225\260\351\205\215\347\275\256", nullptr));
        label_5->setText(QApplication::translate("Widget", "\346\221\204\345\203\217\345\244\264\350\256\276\345\244\207:", nullptr));
        label_6->setText(QApplication::translate("Widget", "\351\237\263\351\242\221\350\256\276\345\244\207  :", nullptr));
        label_7->setText(QApplication::translate("Widget", " \346\241\214\351\235\242:(\345\233\240\344\270\272\346\241\214\351\235\242\345\217\257\344\273\245\345\210\206\345\261\217\346\234\211\345\217\257\350\203\275\346\234\211\345\244\232\344\270\252\351\241\265\351\235\242)", nullptr));
        pushButton_UpdateDevInfo->setText(QApplication::translate("Widget", "\345\210\267\346\226\260\350\256\276\345\244\207\345\210\227\350\241\250", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(widget), QApplication::translate("Widget", "\350\256\276\345\244\207\351\205\215\347\275\256", nullptr));
        pushButton_start->setText(QApplication::translate("Widget", " \345\274\200\345\247\213", nullptr));
        pushButton_stop->setText(QApplication::translate("Widget", "\345\201\234\346\255\242", nullptr));
        groupBox->setTitle(QApplication::translate("Widget", "\346\227\245\345\277\227", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
