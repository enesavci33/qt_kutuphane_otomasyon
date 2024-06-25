#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uye.h"
#include "kitap.h"
#include "odunc_alma.h"
#include "teslim_etme.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db.setDatabaseName("C:/Qt Dosyalari/odev_2/kutup.db");
    if(!db.open())
    {
        ui->statusbar->showMessage("veri tabanına bağlanılamadı");
    }
    else
    {
        ui->statusbar->showMessage("veri tabanına bağlanıldı");

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_uye_clicked()
{
    uye *uye1=new uye(db);
    uye1->show();
}


void MainWindow::on_btn_kitap_clicked()
{
    kitap *kitap1=new kitap(db);
    kitap1->show();
}


void MainWindow::on_btn_od_al_clicked()
{
    odunc_alma *odunc_alma1=new odunc_alma(db);
    odunc_alma1->show();
}


void MainWindow::on_btn_od_ver_clicked()
{
    teslim_etme *teslim_etme1=new teslim_etme(db);
    teslim_etme1->show();
}

