#include "uye.h"
#include "ui_uye.h"

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

