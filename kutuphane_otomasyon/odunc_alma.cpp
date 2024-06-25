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

