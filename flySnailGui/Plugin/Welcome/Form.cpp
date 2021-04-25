#include "Form.h"
#include "ui_Form.h"
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include "CorePluginExport.h"
#include "Global.h"

#include <QStandardPaths>
#include <QDir>
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->label->setAlignment(Qt::AlignCenter);
    //ui->label->setScaledContents(true);
    QImage image(":/log.jpg");
    QImage scale = image.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap fitpixmap = QPixmap::fromImage(scale);


    ui->label->setPixmap(QPixmap(":/logo.jpg"));
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->show();


}

Form::~Form()
{
    delete ui;
}
