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

