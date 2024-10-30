#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QTimer>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Test();
    void UpdateTextOut(const QString &output);
    void ProgressBar_Set();
    void UpdateProgress();
    Ui::MainWindow *ui;

private slots:
    void on_pushButtonRun_clicked();


private:
    QTimer* timer;
    QProcess* process;
    int progress_num;
    int time;
    int num;
    QString url;
};
#endif // MAINWINDOW_H
