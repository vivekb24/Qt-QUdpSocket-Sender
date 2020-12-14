#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpsend= new QUdpSocket(this);

    udprecev= new QUdpSocket(this);
    udprecev->bind(QHostAddress::LocalHost,5555);
    memset(&m_Data, 0, sizeof(m_Data));
    memset(&recv_Data, 0, sizeof(recv_Data));
    //    connect(udprecev,SIGNAL(readyread()),this,SLOT());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SendPort4444_clicked()
{
    m_Data.source_Id=2;
    m_Data.destination_Id=1;
    m_Data.msg_Id=0x542A;
    m_Data.x=10;
    m_Data.y=20;
    m_Data.z=30;
    m_Data.s_checksum=45;
    QByteArray dataToSend;
    dataToSend = QByteArray::fromRawData((char*)&m_Data,sizeof(m_Data));
    //dataToSend = QByteArray::fromRawData(reinterpret_cast<char*>(&m_Data),sizeof(m_Data));
    qint64 sizeSent =  udpsend->writeDatagram(dataToSend,QHostAddress::LocalHost,4444);

    qDebug("Sizeof m_data %d",sizeof (m_Data));
    qDebug()<<"sizeSent is : "<<sizeSent;
    ui->labelSent->setText("sizeSent is :"+QString::number(sizeSent));

    ui->listWidget_send->insertItem(0,"Msg Id:"+QString::number(m_Data.msg_Id,16));
    ui->listWidget_send->insertItem(0,"Source Id:"+QString::number(m_Data.source_Id));
    ui->listWidget_send->insertItem(0,"Destination Id:"+QString::number(m_Data.destination_Id));
    ui->listWidget_send->insertItem(0,"X:"+QString::number(m_Data.x));
    ui->listWidget_send->insertItem(0,"Y:"+QString::number(m_Data.y));
    ui->listWidget_send->insertItem(0,"Z:"+QString::number(m_Data.z));
    ui->listWidget_send->insertItem(0,"CheckSum:"+QString::number(m_Data.s_checksum));
    ui->listWidget_send->insertItem(0,tr("Size Of Structure: %1").arg(sizeof(m_Data)));
}

void MainWindow::on_RecePort5555_clicked()
{
    QByteArray dataToRece;
    while (udprecev->hasPendingDatagrams())
    {
        dataToRece.resize(udprecev->pendingDatagramSize());
        udprecev->readDatagram(dataToRece.data(),dataToRece.size(),&senderip,&senderportno);
        ui->labelReceive->setText("sizeReceive is :"+QString::number(dataToRece.size()));
        memcpy((char*)&recv_Data,dataToRece.data(),sizeof(recv_Data));
        qDebug()<<"readData : "<<sizeof (recv_Data);

        if(recv_Data.msg_Id == 0xAAA1){
            ui->listWidget_rece->insertItem(0,"Msg Id:"+QString::number(recv_Data.msg_Id,16));
            ui->listWidget_rece->insertItem(0,"Source Id:"+QString::number(recv_Data.source_Id));
            ui->listWidget_rece->insertItem(0,"Destination Id:"+QString::number(recv_Data.destination_Id));
            ui->listWidget_rece->insertItem(0,"X:"+QString::number(recv_Data.x));
            ui->listWidget_rece->insertItem(0,"Y:"+QString::number(recv_Data.y));
            ui->listWidget_rece->insertItem(0,"Z:"+QString::number(recv_Data.z));
            ui->listWidget_rece->insertItem(0,"CheckSum:"+QString::number(recv_Data.s_checksum));
            ui->listWidget_rece->insertItem(0,tr("Size Of Structure: %1").arg(sizeof(recv_Data)));
        }else {
            qDebug()<<"Error:";
        }
    }
}

