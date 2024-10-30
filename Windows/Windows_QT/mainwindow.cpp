#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), timer(new QTimer(this))
{
    ui->setupUi(this);
    setWindowTitle("HTTP压力测试软件(Windows版)");
    connect(ui->pushButtonRun, &QPushButton::clicked, this, &MainWindow::onRunButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRunButtonClicked()
{
    // 重置进度条和输出窗口
    ui->progressBar->setValue(0);

    QString url = ui->lineEdit->text();  // 从输入框获取URL
    int port = ui->lineEditPort->text().toInt();  // 从输入框获取端口
    int time = ui->lineEditTime->text().toInt();  // 从输入框获取测试时间
    int clients = ui->lineEditClient->text().toInt();  // 从输入框获取客户端数量
    QString protocol = ui->comboBoxProtocol->currentText();  // 获取选中的协议
    QString methodname = ui->comboBoxMethod->currentText();  // 获取选中的请求方法
    bool force = ui->radioButton->isChecked();  // 检查是否强制刷新

    // 处理请求方法
    int method=0;
    if (methodname == "GET")
        method = 0;
    else if (methodname == "HEAD")
        method = 1;
    else if (methodname == "OPTIONS")
        method = 2;
    else if (methodname == "TRACE")
        method = 3;

    // 根据选中的协议设置相应的参数
    int http10;
    if (protocol == "HTTP/0.9")
        http10 = 0;
    else if (protocol == "HTTP/1.0")
        http10 = 1;
    else
        http10 = 2;  // HTTP/1.1


    // 构建命令和参数
    QString program = "D:/study/Junior-fall/system_design/two_people_project/qt_webbench/qt_webbench/mywinwebbench.exe";  // 替换为你的可执行文件路径
    QStringList arguments;
    arguments << "-u" << url << "-t" << QString::number(time) << "-c" << QString::number(clients)
              << "-p" << QString::number(port) << "-f" << QString::number(force)
              << "-h" << QString::number(http10) << "-m" << QString::number(method);

    // 设置进度条最大值和初始值
    ui->progressBar->setMaximum(time);
    ui->progressBar->setValue(0);

    // 启动外部程序
    QProcess *process = new QProcess(this);
    process->start(program, arguments);

    // 处理程序的输出
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]() {
        QByteArray output = process->readAllStandardOutput();
        QString textOutput = QString::fromLocal8Bit(output);
        ui->textBrowser->append(textOutput);
        parseOutput(textOutput);
    });

    // 停止之前的定时器连接
    disconnect(timer, &QTimer::timeout, this, nullptr);

    // 更新进度条的定时器，每秒更新一次
    connect(timer, &QTimer::timeout, this, [=]() {
        int currentValue = ui->progressBar->value();
        if (currentValue < time) {
            ui->progressBar->setValue(currentValue + 1);
        } else {
            timer->stop();  // 停止计时器
        }
    });

    // 启动定时器
    timer->start(1000);  // 每秒触发一次

    // 处理程序结束后的清理工作
    connect(process, &QProcess::finished, this, [=]() {
        showResults(clients);
        timer->stop();  // 测试结束时停止计时器
        process->deleteLater();  // 清理进程对象
        QMessageBox::information(this, "完成", "压力测试已完成。");
    });
}

void MainWindow::parseOutput(const QString &output)
{
    // 使用正则表达式提取数据
    QRegularExpression regex(R"(子进程完成:\s*成功请求数:\s*(\d+),\s*失败请求数:\s*(\d+),\s*传输字节数:\s*(\d+))");
    QRegularExpressionMatch match = regex.match(output);

    if (match.hasMatch()) {
        int successCount = match.captured(1).toInt();
        int failureCount = match.captured(2).toInt();
        int bytesTransferred = match.captured(3).toInt();

        totalSuccess += successCount;
        totalFailure += failureCount;
        totalBytes += bytesTransferred;
    }
}

void MainWindow::showResults(int clients)
{
    // 计算和显示结果
    double totalRequests = totalSuccess + totalFailure;
    double successRate = (totalRequests > 0) ? (totalSuccess / totalRequests) * 100.0 : 0.0;
    double speedKB = (totalBytes / 1024.0) / ((timer->interval() / 1000.0) * clients);  // 计算传输速度 (kB/s)

    QString results = QString("总成功数: %1\n总失败数: %2\n成功率: %3%\n平均每个客户端传输速度: %4 kB/s")
                          .arg(totalSuccess)
                          .arg(totalFailure)
                          .arg(successRate, 0, 'f', 2)
                          .arg(speedKB, 0, 'f', 2);

    ui->textBrowser->append(results);
}
