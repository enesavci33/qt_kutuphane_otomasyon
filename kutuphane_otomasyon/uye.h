#ifndef UYE_H
#define UYE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class uye;
}

class uye : public QDialog
{
    Q_OBJECT

public:
    explicit uye(QSqlDatabase,QWidget *parent = nullptr);
    ~uye();
    void listele();

private slots:
    void on_tv_uye_clicked(const QModelIndex &index);

    void on_btn_yeni_kayit_clicked();

    void on_btn_gun_clicked();

    void on_btn_sil_clicked();

private:
    Ui::uye *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model;
};

#endif // UYE_H
