#ifndef KITAP_H
#define KITAP_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>


namespace Ui {
class kitap;
}

class kitap : public QDialog
{
    Q_OBJECT

public:
    explicit kitap(QSqlDatabase,QWidget *parent = nullptr);
    ~kitap();
    void listele();

private slots:
    void on_btn_kayit_clicked();

    void on_tv_tum_kitap_clicked(const QModelIndex &index);

    void on_btb_guncelle_clicked();

    void on_btn_sil_clicked();

private:
    Ui::kitap *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
};

#endif // KITAP_H
