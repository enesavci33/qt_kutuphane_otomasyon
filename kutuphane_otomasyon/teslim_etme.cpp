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

