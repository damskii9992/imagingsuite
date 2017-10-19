#include <iostream>
#include <string>

#include <QApplication>
#include <QStringList>
#include <QFile>
#include <QSignalBlocker>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QtDebug>

#include <base/timage.h>
#include <io/analyzefileext.h>
#include <io/io_fits.h>
#include <io/io_tiff.h>
#include <io/io_vivaseq.h>

#include "viewermainwindow.h"

#include "ui_viewermainwindow.h"

ViewerMainWindow::ViewerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerMainWindow)
{
    ui->setupUi(this);
    QStringList args=QApplication::arguments();

    std::cout<<"N arguments="<<args.size()<<std::endl;
    if (1<args.size()) {
        std::string fname=args.last().toStdString();
        kipl::base::TImage<float,2> img;
        LoadImage(fname,img);
        ui->viewer->set_image(img.GetDataPtr(),img.Dims());
    }

    setAcceptDrops(true);

}

ViewerMainWindow::~ViewerMainWindow()
{
    delete ui;
}

void ViewerMainWindow::on_actionOpen_triggered()
{

}

void ViewerMainWindow::LoadImage(std::string fname,kipl::base::TImage<float,2> &img)
{
    if (QFile::exists(QString::fromStdString(fname))) {

        m_ext = kipl::io::GetFileExtensionType(fname);
        switch (m_ext) {
            case kipl::io::ExtensionTXT: std::cout<<"Image format not supported"<<std::endl; break;
            case kipl::io::ExtensionDMP: std::cout<<"Image format not supported"<<std::endl; break;
            case kipl::io::ExtensionDAT: std::cout<<"Image format not supported"<<std::endl; break;
            case kipl::io::ExtensionXML: std::cout<<"Image format not supported"<<std::endl; break;
            case kipl::io::ExtensionRAW: std::cout<<"Image format not supported"<<std::endl; break;
            case kipl::io::ExtensionFITS: kipl::io::ReadFITS(img,fname.c_str()); break;
            case kipl::io::ExtensionTIFF: kipl::io::ReadTIFF(img,fname.c_str()); break;
            case kipl::io::ExtensionPNG: std::cout<<"Image format not supported"<<std::endl; break;
            case kipl::io::ExtensionMAT: std::cout<<"Image format not supported"<<std::endl; break;
            case kipl::io::ExtensionHDF: std::cout<<"Image format not supported"<<std::endl; break;
            case kipl::io::ExtensionSEQ :
            {
                kipl::io::ViVaSEQHeader header;
                kipl::io::GetViVaSEQHeader(fname,&header);

                if (fname!=m_fname) {
                    ui->horizontalSlider->setMinimum(0);
                    ui->horizontalSlider->setMaximum(header.numberOfFrames-1);
                    ui->spinBox->setMinimum(0);
                    ui->spinBox->setMaximum(header.numberOfFrames-1);
                    ui->horizontalSlider->setValue(0);

                }

                kipl::io::ReadViVaSEQ(fname,img,ui->horizontalSlider->value());
            }
            break;
        }
        m_fname = fname;
    }
    else {
        std::cout<<"File does not exist"<<endl;
    }
}

void ViewerMainWindow::on_horizontalSlider_sliderMoved(int position)
{
    QSignalBlocker blockspin(ui->spinBox);
    QSignalBlocker blockslider(ui->horizontalSlider);
    kipl::base::TImage<float,2> img;
    ui->spinBox->setValue(position);
    LoadImage(m_fname,img);
    float low,high;
    ui->viewer->get_levels(&low,&high);
    ui->viewer->set_image(img.GetDataPtr(),img.Dims(),low,high);

}

void ViewerMainWindow::on_spinBox_valueChanged(int arg1)
{
    QSignalBlocker blockspin(ui->spinBox);
    QSignalBlocker blockslider(ui->horizontalSlider);
    kipl::base::TImage<float,2> img;
    ui->horizontalSlider->setValue(arg1);
    LoadImage(m_fname,img);
    float low,high;
    ui->viewer->get_levels(&low,&high);
    ui->viewer->set_image(img.GetDataPtr(),img.Dims(),low,high);
}

void ViewerMainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    const QMimeData * mimeData = e->mimeData();

    if (mimeData->hasUrls()) {
        e->acceptProposedAction();
    }
}

void ViewerMainWindow::dropEvent(QDropEvent *e)
{

    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        qDebug() << "Dropped file:" << fileName;

        kipl::base::TImage<float,2> img;
        LoadImage(fileName.toStdString(),img);
        ui->viewer->set_image(img.GetDataPtr(),img.Dims());
    }
}
