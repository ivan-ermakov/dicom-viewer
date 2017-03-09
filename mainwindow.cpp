#include <QFileDialog>
#include <QMessageBox>
#include <QImageReader>

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmimage/diregist.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    imgLabel(new QLabel(this))
{
    ui->setupUi(this);

    imgLabel->move(20, 50);

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open image file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
}

MainWindow::~MainWindow()
{
    delete openAct;
    delete fileMenu;
    delete ui;
}

void MainWindow::open()
{

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"), "", tr("*.*")); //Image Files (*.png *.jpg *.bmp)

    loadFile(fileName);
}

bool MainWindow::loadFile(const QString &fileName)
{
    /*QImageReader reader(fileName);
      reader.setAutoTransform(true);
      image = reader.read();

      if (image.isNull())
      {
          QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                   tr("Cannot load %1: %2")
                                   .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
          return false;
      }*/

    /*DicomImage *dimage = new DicomImage(fileName.toStdString().c_str());
    if (dimage)
    {
        if (dimage->getStatus() == EIS_Normal)
        {
            std::cout << "Load DICOM image (" << DicomImage::getString(dimage->getStatus()) << ")" << std::endl;
            //dimage->setMinMaxWindow();
            Uint8 *pixelData = (Uint8 *)(dimage->getOutputData(8));

            if (pixelData)
            {
                image = QImage(pixelData, dimage->getWidth(), dimage->getHeight(), QImage::Format::Format_ARGB32);
            }
        }
        else
            std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(dimage->getStatus()) << ")" << std::endl;
    }
    delete dimage;
    QPixmap::loadFromData()

    imgLabel->resize(image.size());
    imgLabel->setPixmap(QPixmap::fromImage(image));*/

    QPixmap *pixmap = nullptr;
    DicomImage *dimage = new DicomImage(fileName.toStdString().c_str());

    if ((dimage) && (dimage->getStatus() == EIS_Normal))
    {
        /* get image extension */
        int width = (int)(dimage->getWidth());
        int height = (int)(dimage->getHeight());
        std::cout << "DicomImage " << width << "x" << height << "\n";
        char header[32];

        /* create PGM header */
        sprintf(header, "P5\n%i %i\n255\n", width, height);
        int offset = strlen(header);
        unsigned int length = width * height + offset;

        /* create output buffer for DicomImage class */
        Uint8 *buffer = new Uint8[length];
        if (buffer)
        {
            /* copy PGM header to buffer */
            memcpy(buffer, header, offset);

            if (dimage->getOutputData((void *)(buffer + offset), length, 8))
            {
                image = QImage::fromData((const unsigned char *)buffer, length, "PGM");
                image = image.scaled(1024, 768, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                imgLabel->resize(image.size());
                imgLabel->setPixmap(QPixmap::fromImage(image));
            }

            /* delete temporary pixel buffer */
            delete[] buffer;
        }
    }

    return true;
}
