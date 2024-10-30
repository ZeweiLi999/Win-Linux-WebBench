#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/new/prefix1/icon/linux.webp"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Test()
{
    //获取UI信息
    QString Url = ui->Url->text();
    QString Time = ui->Time->text();
    QString Client = ui->Client->text();

    //创建进程
    process = new QProcess(this);

    //exe文件绝对路径
    QString program = "/home/dtlu/webbench-1.5/webbench";

    //创建执行命令
    QStringList arguments;
    arguments << "-c" << Client << "-t" << Time  << Url;

    // 连接QProcess的信号到槽函数，当有标准输出可读时触发
    connect(process, &QProcess::readyReadStandardOutput, [this]() {
        QString output = process->readAllStandardOutput();// 读取标准输出
        qDebug().noquote() << output;
        ui->textOut->append(output);
    });

    // 连接QProcess的信号到槽函数，当进程结束时触发
    connect(process, &QProcess::finished, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << "Process finished with exit code:" << exitCode;
        timer->stop();  // 停止进度条更新
        ui->progressBar->setValue(100);  // 将进度条设置为完成
    });

    //设置进度条状态
    ProgressBar_Set();

    qDebug() << "Starting process:" << program << "with arguments:" << arguments;
    process->start(program, arguments);

    if (!process->waitForStarted()) {
        qDebug() << "Failed to start WebBench.";
        return;
    }
}

void MainWindow::ProgressBar_Set()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::UpdateProgress);
    int interval = (ui->Time->text().toInt() + 2 ) * 10;
    timer->start(interval);
    progress_num = 0;  // 初始化进度条数值
}

void MainWindow::UpdateProgress()
{
    progress_num += 1;
    if (progress_num >= 100) {
        timer->stop();  // 停止定时器
        return;
    }
    ui->progressBar->setValue(progress_num);
}

void MainWindow::on_pushButtonRun_clicked()
{
    Test();
}

