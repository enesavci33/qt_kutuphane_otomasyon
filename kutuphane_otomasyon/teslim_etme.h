#ifndef TESLIM_ETME_H
#define TESLIM_ETME_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class teslim_etme;
}

class teslim_etme : public QDialog
{
    Q_OBJECT

public:
    explicit teslim_etme(QSqlDatabase,QWidget *parent = nullptr);
    ~teslim_etme();
    void listele();
    QString alma;


private slots:
    void on_tv_od_al_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::teslim_etme *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model;
    QSqlQueryModel *model2;
};

#endif // TESLIM_ETME_H
