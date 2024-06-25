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

uye::uye(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uye)
{
    ui->setupUi(this);
    sorgu=new QSqlQuery(db);
    listele();

}

uye::~uye()
{
    delete ui;
}

void uye::listele()
{
    sorgu->prepare("select * from uye");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    model=new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tv_uye->setModel(model);
}

void uye::on_tv_uye_clicked(const QModelIndex &index)
{
    ui->ln_uye_no->setText(model->index(index.row(),0).data().toString());
    ui->ln_uye_ad->setText(model->index(index.row(),1).data().toString());
    ui->ln_uye_soyad->setText(model->index(index.row(),2).data().toString());
}


void uye::on_btn_yeni_kayit_clicked()
{
    if(ui->ln_uye_ad->text()=="" && ui->ln_uye_soyad->text()=="")
    {
        QMessageBox::critical(this,"hata","alanları doldur","ok");
        return;
    }
    sorgu->prepare("insert into uye(uye_ad,uye_soyad) values(?,?)");
    sorgu->addBindValue(ui->ln_uye_ad->text());
    sorgu->addBindValue(ui->ln_uye_soyad->text());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    listele();
}


void uye::on_btn_gun_clicked()
{
    sorgu->prepare("update uye set uye_ad=?,uye_soyad=? where uye_no=?");
    sorgu->addBindValue(ui->ln_uye_ad->text());
    sorgu->addBindValue(ui->ln_uye_soyad->text());
    sorgu->addBindValue(ui->ln_uye_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    listele();
}


void uye::on_btn_sil_clicked()
{
    sorgu->prepare("select * from odunc_alinan where uye_no=?");
    sorgu->addBindValue(ui->ln_uye_no->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    int sayac=0;
    while(sorgu->next())
    {
        sayac++;
    }
    if(sayac>0)
    {
        QMessageBox::critical(this,"hata","silinemez","ok");

    }
    else
    {
        sorgu->prepare("delete from uye where uye_no=?");
        sorgu->addBindValue(ui->ln_uye_no->text().toInt());
        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"hata","hata","ok");
            return;
        }
        listele();
    }
}

#include "teslim_etme.h"
#include "ui_teslim_etme.h"
#include <ctime>
#include <QDate>
#include <QDebug>
teslim_etme::teslim_etme(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::teslim_etme)
{
    ui->setupUi(this);
    sorgu=new QSqlQuery(db);
    listele();
}

teslim_etme::~teslim_etme()
{
    delete ui;
}

void teslim_etme::listele()
{
    sorgu->prepare("select * from odunc_alinan");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    model=new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tv_od_al->setModel(model);

    sorgu->prepare("select * from odunc_teslim_edilen");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    model2=new QSqlQueryModel();
    model2->setQuery(*sorgu);
    ui->tv_teslim->setModel(model2);


}

void teslim_etme::on_tv_od_al_clicked(const QModelIndex &index)
{
    ui->ln_uye_no->setText(model->index(index.row(),0).data().toString());
    ui->ln_kitap_no->setText(model->index(index.row(),1).data().toString());
    alma=model->index(index.row(),2).data().toString();
}




void teslim_etme::on_pushButton_clicked()
{

    QStringList tarih,tarih2;
    tarih=alma.split(".");
    tarih2=ui->de_tarih->text().split(".");
    QDate startDate(tarih[2].toInt(),tarih[1].toInt(),tarih[0].toInt());
    QDate endDate(tarih2[2].toInt(),tarih2[1].toInt(),tarih2[0].toInt());
    int dayCount=startDate.daysTo(endDate);
    qDebug() << dayCount;
    if(dayCount>15)
    {
        int borc=dayCount-15;
        sorgu->prepare("insert into odunc_teslim_edilen(uye_no,kitap_no,alma_tarihi,verme_tarihi,borc) values(?,?,?,?,?)");
        sorgu->addBindValue(ui->ln_uye_no->text().toInt());
        sorgu->addBindValue(ui->ln_kitap_no->text().toInt());
        sorgu->addBindValue(alma);
        sorgu->addBindValue(ui->de_tarih->text());
        sorgu->addBindValue(borc);
        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"hata","hata 1","ok");
            return;
        }
        sorgu->prepare("delete from odunc_alinan where uye_no=? and kitap_no=?");
        sorgu->addBindValue(ui->ln_uye_no->text().toInt());
        sorgu->addBindValue(ui->ln_kitap_no->text().toInt());
        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"hata","hata2","ok");
            return;
        }
        listele();
    }
    else
    {
        int borc=0;
        sorgu->prepare("insert into odunc_teslim_edilen(uye_no,kitap_no,alma_tarihi,verme_tarihi,borc) values(?,?,?,?,?)");
        sorgu->addBindValue(ui->ln_uye_no->text().toInt());
        sorgu->addBindValue(ui->ln_kitap_no->text().toInt());
        sorgu->addBindValue(alma);
        sorgu->addBindValue(ui->de_tarih->text());
        sorgu->addBindValue(borc);
        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"hata","hata 3","ok");
            return;
        }

        sorgu->prepare("delete from odunc_alinan where uye_no=? and kitap_no=?");
        sorgu->addBindValue(ui->ln_uye_no->text().toInt());
        sorgu->addBindValue(ui->ln_kitap_no->text().toInt());
        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"hata","hata2","ok");
            return;
        }
        listele();

    }

}

#include "odunc_alma.h"
#include "ui_odunc_alma.h"
#include "kitap.h"
odunc_alma::odunc_alma(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::odunc_alma)
{
    ui->setupUi(this);
    sorgu=new QSqlQuery(db);
    listele();
}

odunc_alma::~odunc_alma()
{
    delete ui;
}

void odunc_alma::listele()
{
    sorgu->prepare("select * from uye");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    model=new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tv_tum_uye->setModel(model);

    sorgu->prepare("select * from kitap");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    model2=new QSqlQueryModel();
    model2->setQuery(*sorgu);
    ui->tv_tum_kitap->setModel(model2);

    sorgu->prepare("select * from odunc_alinan");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    model3=new QSqlQueryModel();
    model3->setQuery(*sorgu);
    ui->tv_od_al_kitap->setModel(model3);



}

void odunc_alma::on_tv_tum_uye_clicked(const QModelIndex &index)
{
    ui->ln_uye_no->setText(model->index(index.row(),0).data().toString());

}


void odunc_alma::on_tv_tum_kitap_clicked(const QModelIndex &index)
{
    ui->ln_kitap_no->setText(model2->index(index.row(),0).data().toString());
    stok=model2->index(index.row(),2).data().toInt();

}


void odunc_alma::on_pushButton_clicked()
{
    if(stok>0)
    {
        sorgu->prepare("insert into odunc_alinan(uye_no,kitap_no,odunc_alma_tarihi) values(?,?,?)");
        sorgu->addBindValue(ui->ln_uye_no->text().toInt());
        sorgu->addBindValue(ui->ln_kitap_no->text().toInt());
        sorgu->addBindValue(ui->de_tarih->text());
        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"hata","alınamaz","ok");
                return;
        }
        stok--;
        sorgu->prepare("update kitap set kitap_sayisi=? where kitap_no=?");
        sorgu->addBindValue(stok);
        sorgu->addBindValue(ui->ln_kitap_no->text().toInt());
        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"hata","alınamaz","ok");
                return;
        }
        listele();
    }
    else
    {
        QMessageBox::critical(this,"hata","alınamaz22","ok");
        return;
    }


}

#include "kitap.h"
#include "ui_kitap.h"

kitap::kitap(QSqlDatabase db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kitap)
{
    ui->setupUi(this);
    sorgu=new QSqlQuery(db);
    listele();
}

kitap::~kitap()
{
    delete ui;
}

void kitap::listele()
{
    sorgu->prepare("select * from kitap");
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    model=new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tv_tum_kitap->setModel(model);
}

void kitap::on_btn_kayit_clicked()
{
    sorgu->prepare("insert into kitap(kitap_ad,kitap_sayisi) values(?,?)");
    sorgu->addBindValue(ui->ln_kitap_ad->text());
    sorgu->addBindValue(ui->ln_kitap_stok->text().toInt());

    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    listele();
}


void kitap::on_tv_tum_kitap_clicked(const QModelIndex &index)
{
    ui->ln_kitapno->setText(model->index(index.row(),0).data().toString());
    ui->ln_kitap_ad->setText(model->index(index.row(),1).data().toString());
    ui->ln_kitap_stok->setText(model->index(index.row(),2).data().toString());

    sorgu->prepare("select * from odunc_alinan where kitap_no=?");
    sorgu->addBindValue(ui->ln_kitapno->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    model2=new QSqlQueryModel();
    model2->setQuery(*sorgu);
    ui->tv_od_al_dur->setModel(model2);

    sorgu->prepare("select * from odunc_teslim_edilen where kitap_no=?");
    sorgu->addBindValue(ui->ln_kitapno->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    model3=new QSqlQueryModel();
    model3->setQuery(*sorgu);
    ui->tv_on_od_al_dur->setModel(model3);
}



void kitap::on_btb_guncelle_clicked()
{
    sorgu->prepare("update kitap set kitap_ad=?,kitap_sayisi=? where kitap_no=?");
    sorgu->addBindValue(ui->ln_kitap_ad->text());
    sorgu->addBindValue(ui->ln_kitap_stok->text());
    sorgu->addBindValue(ui->ln_kitapno->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    listele();
}


void kitap::on_btn_sil_clicked()
{
    sorgu->prepare("select * from odunc_alinan where kitap_no=?");
    sorgu->addBindValue(ui->ln_kitapno->text().toInt());
    if(!sorgu->exec())
    {
        QMessageBox::critical(this,"hata","hata","ok");
        return;
    }
    int sayac=0;
    while(sorgu->next())
    {
        sayac++;
    }
    if(sayac>0)
    {
        QMessageBox::critical(this,"hata","silinemez","ok");

    }
    else
    {
        sorgu->prepare("delete from kitap where kitap_no=?");
        sorgu->addBindValue(ui->ln_kitapno->text().toInt());
        if(!sorgu->exec())
        {
            QMessageBox::critical(this,"hata","hata","ok");
            return;
        }
        listele();
    }
}

