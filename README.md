# Web服务器压力测试软件

## ![Static Badge](https://img.shields.io/badge/Function-功能-blue) 功能：
**实现：[PyQT6 + C++]**

Windows/Linux双版本接口构建，QT前端。

<img src="https://github.com/user-attachments/assets/f55e1ce2-3358-445b-93f6-ed78dc53ac5e"  width=400 height=200/>



**核心功能：**

- 对已部署的服务器进行HTTP压力测试,测试服务器并发性能。



## ![Static Badge](https://img.shields.io/badge/Intro-文件介绍-green) 文件介绍：

```

Win-Linux-WebBench:
├─Linux
│  ├─Linux_webbench //LinuxWebBench源代码命令行实现
│  └─qt_webbench    //QT前端实现
│     
└─Windows
    ├─Windows_MyWebBench  //MyWinWebBench源代码命令行实现
    └─Windows_QT          //QT前端实现


```
Windows_MyWebBench：
- **BenchCore.exe**，可实现单次HTTP测试。
- **mywinwebbench.exe**，可实现多次HTTP测试。




## ![Static Badge](https://img.shields.io/badge/Use-使用-red) 使用：

### Windows版本：
已包含可执行文件。
在**Windows/Windows_QT**直接以QT项目运行即可。

![image](https://github.com/user-attachments/assets/943e3d7b-29bc-4e98-a9dc-3433153ece2e)


<br />

### Linux版本：
在**Linux/qt_webbench**直接以QT项目运行即可。

![image](https://github.com/user-attachments/assets/9ab68bf5-12e2-4322-a916-58a22123ddcc)

<br />
 
## ![Static Badge](https://img.shields.io/badge/Thanks-致谢-yellow)  致谢：


没有前人的经验和开源，我们无法完成这个项目，致以诚挚的谢意：

[1] WebBench:https://github.com/EZLippi/WebBench

[2] WinWebBench:https://github.com/abumaster/WinWebBench

[3] Google C++ Style Guide:[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

[4] 谷歌代码规范中文版:[C++ 风格指南 - 内容目录 — Google 开源项目风格指南](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents.html)

[5] Linux fork()函数详解:[fork](https://pubs.opengroup.org/onlinepubs/009695399/functions/fork.html)

[6] Winsock:[Winsock 入门 - Win32 apps | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows/win32/winsock/getting-started-with-winsock)

