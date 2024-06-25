#ifndef ODUNC_ALMA_H
#define ODUNC_ALMA_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class odunc_alma;
}

class odunc_alma : public QDialog
{
    Q_OBJECT

public:
    explicit odunc_alma(QSqlDatabase,QWidget *parent = nullptr);
    ~odunc_alma();
    void listele();
    int kitap_no;
    QString kitap_ad;
    int stok=0;

private slots:
    void on_tv_tum_uye_clicked(const QModelIndex &index);

    void on_tv_tum_kitap_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::odunc_alma *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model;
    QSqlQueryModel *model2;
    QSqlQueryModel *model3;
};

#endif // ODUNC_ALMA_H
