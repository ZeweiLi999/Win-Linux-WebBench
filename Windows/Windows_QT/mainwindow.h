#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRunButtonClicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    // 新增统计变量
    int totalSuccess=0;   // 总成功数
    int totalFailure=0;   // 总失败数
    int totalBytes=0;     // 总传输字节数

    void parseOutput(const QString &output);  // 解析输出
    void showResults(const int clients);                        // 显示结果
};

#endif // MAINWINDOW_H
