# Qt 常用类教程目录 / Qt Common Classes Tutorial Contents

> 本教程以 **Qt 6 + C++** 为主线，按照五个常用核心模块依次展开：`Qt Core`、`Qt GUI`、`Qt Widgets`、`Qt Network`、`Qt SQL`。  
> This tutorial uses **Qt 6 + C++** and is organized around five commonly used core modules: `Qt Core`, `Qt GUI`, `Qt Widgets`, `Qt Network`, and `Qt SQL`.

## 教程导读 / Tutorial Guide

1. Qt 简介与版本选择 / Introduction to Qt and version selection
2. Qt Creator、编译器与构建套件 / Qt Creator, compilers, and kits
3. CMake 项目的基本结构 / Basic structure of a CMake project
4. 第一个 Qt 程序 / Your first Qt application
5. Qt 帮助文档的阅读方法 / How to use the Qt documentation
6. Qt 的对象所有权与父子对象机制 / Object ownership and the parent-child mechanism

---

## 第一篇：Qt Core——非界面核心类 / Part I: Qt Core — Non-GUI Core Classes

### 第 1 章：QObject 与元对象系统 / Chapter 1: QObject and the Meta-Object System

1. `QObject`：Qt 对象模型的基础 / Foundation of the Qt object model
2. `Q_OBJECT`：启用元对象能力 / Enabling meta-object features
3. `QMetaObject`：运行时类型与方法信息 / Runtime type and method information
4. `QMetaProperty`：属性系统 / The property system
5. `QMetaType`：自定义类型注册 / Custom type registration
6. `QPointer`：安全跟踪 QObject 指针 / Safely tracking QObject pointers
7. `QSharedPointer`、`QWeakPointer` 与 `QScopedPointer` / Shared, weak, and scoped pointers
8. 父子对象与自动内存管理 / Parent-child objects and automatic memory management

### 第 2 章：信号与槽 / Chapter 2: Signals and Slots

1. 信号与槽的基本语法 / Basic signal-slot syntax
2. `QObject::connect()` 的常见写法 / Common QObject::connect() patterns
3. Lambda 表达式作为槽函数 / Using lambda expressions as slots
4. 自动连接与 `QMetaObject::connectSlotsByName()` / Automatic signal-slot connections
5. 直接、队列与阻塞队列连接 / Direct, queued, and blocking queued connections
6. `QObject::sender()` 与信号来源 / Identifying the signal sender
7. `QSignalBlocker`：临时屏蔽信号 / Temporarily blocking signals
8. 自定义信号、槽与参数类型 / Custom signals, slots, and parameter types

### 第 3 章：字符串、字节与通用数据 / Chapter 3: Strings, Bytes, and Generic Data

1. `QString`：Unicode 字符串 / Unicode strings
2. `QStringView`：轻量字符串视图 / Lightweight string views
3. `QByteArray`：二进制字节数组 / Binary byte arrays
4. `QChar`：字符处理 / Character handling
5. `QRegularExpression`：正则表达式 / Regular expressions
6. `QVariant`：通用值容器 / Generic value containers
7. `QAnyStringView`：统一字符串参数 / Unified string parameters
8. 字符串编码与类型转换 / String encoding and type conversion

### 第 4 章：Qt 容器与迭代器 / Chapter 4: Qt Containers and Iterators

1. `QList` 与 `QVector`：顺序容器 / Sequential containers
2. `QMap` 与 `QMultiMap`：有序关联容器 / Ordered associative containers
3. `QHash` 与 `QMultiHash`：哈希容器 / Hash-based containers
4. `QSet`：集合容器 / Set containers
5. `QPair`：二元数据 / Value pairs
6. `QStringList`：字符串列表 / String lists
7. Java 风格与 STL 风格迭代器 / Java-style and STL-style iterators
8. Qt 容器与 STL 容器的转换 / Converting between Qt and STL containers

### 第 5 章：时间、定时器与事件循环 / Chapter 5: Time, Timers, and Event Loops

1. `QDate`、`QTime` 与 `QDateTime` / Dates, times, and date-times
2. `QTimeZone`：时区处理 / Time-zone handling
3. `QElapsedTimer`：耗时测量 / Elapsed-time measurement
4. `QDeadlineTimer`：截止时间控制 / Deadline control
5. `QTimer`：周期任务与延迟执行 / Periodic and delayed execution
6. `QChronoTimer`：高精度与大范围定时 / High-precision, wide-range timing
7. `QEvent`：事件基础 / Event fundamentals
8. `QEventLoop`：局部事件循环 / Local event loops
9. `QCoreApplication`：应用程序事件循环 / The application event loop

### 第 6 章：文件、目录与数据流 / Chapter 6: Files, Directories, and Data Streams

1. `QFile`：文件读写 / Reading and writing files
2. `QFileInfo`：文件信息 / File information
3. `QDir`：目录操作 / Directory operations
4. `QDirIterator`：目录遍历 / Directory iteration
5. `QIODevice`：输入输出设备抽象 / I/O device abstraction
6. `QTextStream`：文本流 / Text streams
7. `QDataStream`：二进制序列化 / Binary serialization
8. `QSaveFile`：安全写入文件 / Safe file writing
9. `QTemporaryFile` 与 `QTemporaryDir` / Temporary files and directories
10. `QFileSystemWatcher`：监视文件变化 / Monitoring file-system changes
11. `QStandardPaths`：标准系统路径 / Standard system paths
12. `QResource`：Qt 资源系统 / The Qt resource system

### 第 7 章：JSON、XML 与配置 / Chapter 7: JSON, XML, and Settings

1. `QJsonDocument`：JSON 文档 / JSON documents
2. `QJsonObject`：JSON 对象 / JSON objects
3. `QJsonArray`：JSON 数组 / JSON arrays
4. `QJsonValue`：JSON 值 / JSON values
5. `QXmlStreamReader`：流式读取 XML / Streaming XML input
6. `QXmlStreamWriter`：流式写入 XML / Streaming XML output
7. `QSettings`：持久化应用配置 / Persistent application settings
8. 配置文件的版本兼容设计 / Version-compatible configuration design

### 第 8 章：进程、线程与并发 / Chapter 8: Processes, Threads, and Concurrency

1. `QProcess`：启动和管理外部进程 / Starting and managing external processes
2. `QThread`：线程基础 / Thread fundamentals
3. 工作对象移动到线程 / Moving worker objects to threads
4. `QThreadPool` 与 `QRunnable` / Thread pools and runnable tasks
5. `QMutex` 与 `QMutexLocker` / Mutexes and scoped locking
6. `QReadWriteLock`：读写锁 / Read-write locks
7. `QWaitCondition`：线程等待与唤醒 / Thread waiting and wake-up
8. `QSemaphore`：信号量 / Semaphores
9. `QFuture` 与 `QFutureWatcher` / Asynchronous results and monitoring
10. 跨线程信号槽与线程安全 / Cross-thread signals, slots, and thread safety

### 第 9 章：实用工具类 / Chapter 9: Utility Classes

1. `QUrl`：URL 解析与编码 / URL parsing and encoding
2. `QUuid`：唯一标识符 / Universally unique identifiers
3. `QLocale`：本地化格式 / Locale-aware formatting
4. `QTranslator`：界面国际化 / User-interface translation
5. `QCommandLineParser`：命令行参数 / Command-line arguments
6. `QLoggingCategory`：分类日志 / Categorized logging
7. `QRandomGenerator`：随机数 / Random-number generation
8. `QCryptographicHash`：哈希计算 / Cryptographic hashes
9. `QLibrary`：动态库加载 / Dynamic library loading

---

## 第二篇：Qt GUI——图形与窗口基础类 / Part II: Qt GUI — Graphics and Windowing Classes

### 第 10 章：GUI 应用与窗口系统 / Chapter 10: GUI Applications and Windowing

1. `QGuiApplication`：GUI 应用程序入口 / GUI application entry point
2. `QWindow`：原生窗口抽象 / Native window abstraction
3. `QScreen`：屏幕与多显示器 / Screens and multi-monitor environments
4. `QSurface` 与 `QSurfaceFormat` / Rendering surfaces and formats
5. 高 DPI 与设备像素比 / High DPI and device pixel ratios
6. 窗口状态、标志与几何信息 / Window states, flags, and geometry

### 第 11 章：颜色、字体与图像 / Chapter 11: Colors, Fonts, and Images

1. `QColor`：颜色表示与转换 / Color representation and conversion
2. `QPalette`：界面调色板 / User-interface palettes
3. `QFont`：字体设置 / Font configuration
4. `QFontMetrics` 与 `QFontMetricsF` / Font measurements
5. `QImage`：像素级图像处理 / Pixel-level image processing
6. `QPixmap`：面向屏幕显示的图像 / Screen-optimized images
7. `QBitmap`：单色位图 / Monochrome bitmaps
8. `QIcon`：多状态图标 / Multi-state icons
9. `QImageReader` 与 `QImageWriter` / Image reading and writing
10. `QMovie`：动态图像播放 / Animated image playback

### 第 12 章：二维绘图系统 / Chapter 12: The 2D Painting System

1. `QPainter`：绘图入口 / Painting entry point
2. `QPaintDevice`：绘图设备 / Paint devices
3. `QPen`：线条样式 / Line styles
4. `QBrush`：填充样式 / Fill styles
5. `QPainterPath`：复杂路径 / Complex painter paths
6. `QPolygon` 与 `QPolygonF` / Integer and floating-point polygons
7. `QRegion`：区域运算 / Region operations
8. `QTransform`：二维变换 / 2D transformations
9. 渐变、透明度与抗锯齿 / Gradients, opacity, and antialiasing
10. 坐标系统与裁剪区域 / Coordinate systems and clipping

### 第 13 章：输入事件 / Chapter 13: Input Events

1. `QInputEvent`：输入事件基类 / Base class for input events
2. `QMouseEvent`：鼠标事件 / Mouse events
3. `QKeyEvent`：键盘事件 / Keyboard events
4. `QWheelEvent`：滚轮事件 / Wheel events
5. `QEnterEvent`：鼠标进入事件 / Enter events
6. `QTouchEvent`：触摸事件 / Touch events
7. `QTabletEvent`：数位板事件 / Tablet events
8. `QInputMethodEvent`：输入法事件 / Input-method events
9. 事件接受、忽略与传播 / Event acceptance, ignoring, and propagation

### 第 14 章：剪贴板与拖放 / Chapter 14: Clipboard and Drag-and-Drop

1. `QClipboard`：系统剪贴板 / The system clipboard
2. `QMimeData`：MIME 数据封装 / MIME data containers
3. `QDrag`：拖放操作 / Drag-and-drop operations
4. `QDragEnterEvent`：拖入事件 / Drag-enter events
5. `QDragMoveEvent`：拖动事件 / Drag-move events
6. `QDropEvent`：放下事件 / Drop events
7. 自定义 MIME 类型 / Custom MIME types

### 第 15 章：GUI 常用辅助类 / Chapter 15: Common GUI Helper Classes

1. `QAction`：用户操作抽象 / User-action abstraction
2. `QActionGroup`：互斥与分组操作 / Exclusive and grouped actions
3. `QShortcut`：快捷键 / Keyboard shortcuts
4. `QKeySequence`：按键序列 / Key sequences
5. `QCursor`：鼠标指针 / Mouse cursors
6. `QValidator`：输入校验基类 / Base class for input validation
7. `QIntValidator`、`QDoubleValidator` 与 `QRegularExpressionValidator` / Numeric and regular-expression validators
8. `QStandardItem` 与 `QStandardItemModel` / Standard data items and models
9. `QDesktopServices`：调用桌面服务 / Accessing desktop services

---

## 第三篇：Qt Widgets——桌面界面控件类 / Part III: Qt Widgets — Desktop UI Classes

### 第 16 章：Widgets 应用基础 / Chapter 16: Widgets Application Basics

1. `QApplication`：Widgets 应用程序入口 / Widgets application entry point
2. `QWidget`：所有控件的基础 / Base class for all widgets
3. `QMainWindow`：主窗口框架 / Main-window framework
4. `QDialog`：对话框基础 / Dialog fundamentals
5. `QFrame`：带边框的基础控件 / Framed base widgets
6. 控件的父子关系与生命周期 / Widget ownership and lifetime
7. 控件的尺寸策略与几何属性 / Size policies and geometry

### 第 17 章：布局管理 / Chapter 17: Layout Management

1. `QLayout`：布局基类 / Base layout class
2. `QHBoxLayout`：水平布局 / Horizontal layouts
3. `QVBoxLayout`：垂直布局 / Vertical layouts
4. `QGridLayout`：网格布局 / Grid layouts
5. `QFormLayout`：表单布局 / Form layouts
6. `QStackedLayout`：堆叠布局 / Stacked layouts
7. `QSpacerItem`：弹性空间 / Flexible spacing
8. `QSizePolicy`：尺寸策略 / Size policies
9. 嵌套布局与自适应窗口 / Nested layouts and responsive windows

### 第 18 章：按钮与选择控件 / Chapter 18: Buttons and Selection Widgets

1. `QAbstractButton`：按钮基类 / Base class for buttons
2. `QPushButton`：命令按钮 / Command buttons
3. `QToolButton`：工具按钮 / Tool buttons
4. `QCheckBox`：复选框 / Check boxes
5. `QRadioButton`：单选按钮 / Radio buttons
6. `QButtonGroup`：按钮分组 / Button groups
7. `QComboBox`：下拉选择框 / Combo boxes
8. `QFontComboBox`：字体选择框 / Font combo boxes

### 第 19 章：文本与数值输入 / Chapter 19: Text and Numeric Input

1. `QLineEdit`：单行文本输入 / Single-line text input
2. `QTextEdit`：富文本编辑 / Rich-text editing
3. `QPlainTextEdit`：纯文本编辑 / Plain-text editing
4. `QAbstractSpinBox`：步进输入基类 / Base class for spin boxes
5. `QSpinBox`：整数输入 / Integer input
6. `QDoubleSpinBox`：浮点数输入 / Floating-point input
7. `QDateEdit`、`QTimeEdit` 与 `QDateTimeEdit` / Date and time input
8. `QKeySequenceEdit`：快捷键输入 / Key-sequence input
9. 输入掩码、校验器与自动补全 / Input masks, validators, and completion
10. `QCompleter`：输入补全 / Input completion

### 第 20 章：信息显示控件 / Chapter 20: Display Widgets

1. `QLabel`：文本与图片显示 / Displaying text and images
2. `QLCDNumber`：数字显示 / LCD-style numeric display
3. `QProgressBar`：进度显示 / Progress display
4. `QSlider`：滑块 / Sliders
5. `QDial`：旋钮 / Dials
6. `QScrollBar`：滚动条 / Scroll bars
7. `QStatusBar`：状态栏 / Status bars
8. `QToolTip`：工具提示 / Tooltips

### 第 21 章：容器与页面控件 / Chapter 21: Container and Page Widgets

1. `QGroupBox`：分组框 / Group boxes
2. `QTabWidget`：标签页 / Tabbed pages
3. `QStackedWidget`：堆叠页面 / Stacked pages
4. `QToolBox`：工具箱 / Toolbox pages
5. `QScrollArea`：滚动区域 / Scroll areas
6. `QSplitter`：可调分隔布局 / Adjustable split layouts
7. `QMdiArea` 与 `QMdiSubWindow` / Multiple-document interfaces
8. `QDockWidget`：停靠窗口 / Dock widgets

### 第 22 章：项目视图与模型/视图 / Chapter 22: Item Views and Model/View Programming

1. `QListWidget` 与 `QListWidgetItem` / List widgets and items
2. `QTreeWidget` 与 `QTreeWidgetItem` / Tree widgets and items
3. `QTableWidget` 与 `QTableWidgetItem` / Table widgets and items
4. `QAbstractItemView`：视图基类 / Base class for item views
5. `QListView`：列表视图 / List views
6. `QTreeView`：树形视图 / Tree views
7. `QTableView`：表格视图 / Table views
8. `QAbstractItemModel`：模型基类 / Base class for item models
9. `QAbstractListModel` 与 `QAbstractTableModel` / List and table model bases
10. `QSortFilterProxyModel`：排序与过滤 / Sorting and filtering
11. `QStyledItemDelegate`：自定义委托 / Custom item delegates
12. 自定义模型、视图与委托 / Building custom models, views, and delegates

### 第 23 章：菜单、工具栏与操作 / Chapter 23: Menus, Toolbars, and Actions

1. `QMenu`：菜单 / Menus
2. `QMenuBar`：菜单栏 / Menu bars
3. `QToolBar`：工具栏 / Toolbars
4. `QAction` 与 `QActionGroup` / Actions and action groups
5. `QWidgetAction`：带控件的操作 / Widget-based actions
6. 上下文菜单 / Context menus
7. 快捷键与状态提示 / Shortcuts and status tips

### 第 24 章：标准对话框 / Chapter 24: Standard Dialogs

1. `QMessageBox`：消息对话框 / Message boxes
2. `QFileDialog`：文件对话框 / File dialogs
3. `QColorDialog`：颜色对话框 / Color dialogs
4. `QFontDialog`：字体对话框 / Font dialogs
5. `QInputDialog`：输入对话框 / Input dialogs
6. `QProgressDialog`：进度对话框 / Progress dialogs
7. 模态与非模态对话框 / Modal and modeless dialogs

### 第 25 章：自定义控件与样式 / Chapter 25: Custom Widgets and Styling

1. 重写 `paintEvent()` 绘制控件 / Painting widgets by overriding paintEvent()
2. 重写鼠标、键盘与尺寸事件 / Overriding mouse, keyboard, and resize events
3. `QStyle` 与 `QProxyStyle` / Styles and proxy styles
4. `QStyleOption` 系列类 / QStyleOption classes
5. Qt 样式表（QSS）/ Qt Style Sheets (QSS)
6. 动态属性与样式刷新 / Dynamic properties and style refresh
7. `QGraphicsEffect`：图形效果 / Graphics effects
8. `QPropertyAnimation`：属性动画 / Property animations
9. 自定义控件的属性、信号和槽 / Properties, signals, and slots in custom widgets

### 第 26 章：Graphics View 框架 / Chapter 26: The Graphics View Framework

1. `QGraphicsScene`：图形场景 / Graphics scenes
2. `QGraphicsView`：场景视图 / Scene views
3. `QGraphicsItem`：图元基类 / Base class for graphics items
4. 常用图元类 / Common graphics item classes
5. 图元选择、碰撞与变换 / Item selection, collision, and transformation
6. 自定义图元 / Custom graphics items

### 第 27 章：桌面集成与打印 / Chapter 27: Desktop Integration and Printing

1. `QSystemTrayIcon`：系统托盘 / System tray icons
2. `QFileIconProvider`：系统文件图标 / System file icons
3. `QPrinter`：打印设备 / Printer devices
4. `QPrintDialog`：打印对话框 / Print dialogs
5. `QPrintPreviewDialog`：打印预览 / Print preview

---

## 第四篇：Qt Network——网络通信类 / Part IV: Qt Network — Networking Classes

### 第 28 章：网络基础 / Chapter 28: Networking Fundamentals

1. Qt 网络模块概览 / Overview of the Qt Network module
2. `QHostAddress`：IP 地址 / IP addresses
3. `QHostInfo`：主机名与 DNS 查询 / Host names and DNS lookups
4. `QNetworkInterface`：本机网络接口 / Local network interfaces
5. `QAbstractSocket`：套接字基类 / Base class for sockets
6. 网络错误处理与超时设计 / Network error handling and timeout design

### 第 29 章：TCP 通信 / Chapter 29: TCP Communication

1. `QTcpSocket`：TCP 客户端 / TCP clients
2. `QTcpServer`：TCP 服务端 / TCP servers
3. 建立连接与断开连接 / Establishing and closing connections
4. 读取、写入与缓冲区 / Reading, writing, and buffering
5. 自定义消息协议与粘包处理 / Custom protocols and message framing
6. 多客户端连接管理 / Managing multiple clients
7. 心跳、重连与超时 / Heartbeats, reconnection, and timeouts

### 第 30 章：UDP 通信 / Chapter 30: UDP Communication

1. `QUdpSocket`：UDP 通信 / UDP communication
2. 数据报的发送与接收 / Sending and receiving datagrams
3. 广播 / Broadcasting
4. 多播 / Multicasting
5. UDP 丢包、乱序与数据边界 / Packet loss, ordering, and datagram boundaries

### 第 31 章：HTTP 客户端 / Chapter 31: HTTP Clients

1. `QNetworkAccessManager`：网络请求管理 / Network request management
2. `QNetworkRequest`：请求配置 / Request configuration
3. `QNetworkReply`：响应处理 / Response handling
4. `QUrlQuery`：查询参数 / Query parameters
5. GET、POST、PUT 与 DELETE 请求 / GET, POST, PUT, and DELETE requests
6. JSON 请求与响应 / JSON requests and responses
7. 表单提交与文件上传 / Form submission and file uploads
8. 文件下载、进度与断点续传 / Downloads, progress, and resuming
9. 重定向、超时与请求取消 / Redirects, timeouts, and cancellation
10. `QNetworkCookie` 与 `QNetworkCookieJar` / Cookies and cookie jars
11. `QAbstractNetworkCache` 与 `QNetworkDiskCache` / Network caching

### 第 32 章：认证、代理与安全连接 / Chapter 32: Authentication, Proxies, and Secure Connections

1. `QAuthenticator`：身份认证 / Authentication
2. `QNetworkProxy`：网络代理 / Network proxies
3. `QSslSocket`：TLS 加密套接字 / TLS-encrypted sockets
4. `QSslConfiguration`：TLS 配置 / TLS configuration
5. `QSslCertificate`：数字证书 / Digital certificates
6. `QSslKey`：加密密钥 / Cryptographic keys
7. `QSslError`：安全连接错误 / Secure-connection errors
8. 证书校验与常见 TLS 错误 / Certificate validation and common TLS errors

### 第 33 章：本地进程间通信 / Chapter 33: Local Inter-Process Communication

1. `QLocalSocket`：本地套接字客户端 / Local socket clients
2. `QLocalServer`：本地套接字服务端 / Local socket servers
3. 单实例应用 / Single-instance applications
4. 本地消息协议设计 / Local message protocol design

---

## 第五篇：Qt SQL——数据库访问类 / Part V: Qt SQL — Database Classes

### 第 34 章：数据库连接 / Chapter 34: Database Connections

1. Qt SQL 模块与驱动概览 / Overview of Qt SQL and database drivers
2. `QSqlDatabase`：创建和管理连接 / Creating and managing connections
3. SQLite 数据库连接 / Connecting to SQLite
4. MySQL 与 PostgreSQL 数据库连接 / Connecting to MySQL and PostgreSQL
5. 多数据库与命名连接 / Multiple databases and named connections
6. 连接生命周期与线程限制 / Connection lifetime and thread restrictions
7. 数据库驱动的部署与排错 / Deploying and troubleshooting database drivers

### 第 35 章：SQL 查询与结果处理 / Chapter 35: SQL Queries and Result Handling

1. `QSqlQuery`：执行 SQL 语句 / Executing SQL statements
2. `QSqlRecord`：读取结果记录 / Reading result records
3. `QSqlField`：字段信息 / Field information
4. `QSqlIndex`：索引信息 / Index information
5. `QSqlError`：错误诊断 / Error diagnostics
6. 参数绑定与预处理语句 / Parameter binding and prepared statements
7. 批量插入与批量更新 / Batch inserts and updates
8. SQL 注入防护 / Preventing SQL injection

### 第 36 章：事务与数据一致性 / Chapter 36: Transactions and Data Consistency

1. 开启、提交与回滚事务 / Starting, committing, and rolling back transactions
2. `QSqlDatabase::transaction()`：开启事务 / Starting a transaction
3. `QSqlDatabase::commit()`：提交事务 / Committing a transaction
4. `QSqlDatabase::rollback()`：回滚事务 / Rolling back a transaction
5. 事务失败处理 / Handling transaction failures
6. 并发访问与锁 / Concurrent access and locking

### 第 37 章：SQL 模型与界面绑定 / Chapter 37: SQL Models and UI Binding

1. `QSqlQueryModel`：只读查询模型 / Read-only query models
2. `QSqlTableModel`：单表编辑模型 / Editable single-table models
3. `QSqlRelationalTableModel`：关系表模型 / Relational table models
4. `QSqlRelationalDelegate`：关系数据委托 / Relational data delegates
5. SQL 模型与 `QTableView` 配合 / Using SQL models with QTableView
6. 编辑策略、提交与撤销 / Edit strategies, submission, and reversion
7. 排序、筛选与分页 / Sorting, filtering, and pagination

### 第 38 章：数据库综合实践 / Chapter 38: Database Practice

1. 数据访问层设计 / Data-access layer design
2. 数据库初始化与版本迁移 / Database initialization and migrations
3. 联系人管理系统 / Contact management system
4. 登录、用户与权限管理 / Login, users, and permissions
5. 日志查询与分页展示 / Log querying and paginated display
6. 数据导入、导出与备份 / Data import, export, and backup

---

## 综合项目篇 / Integrated Projects

### 第 39 章：桌面记事本 / Chapter 39: Desktop Notepad

- 使用 `QMainWindow`、`QTextEdit`、`QFile`、`QSettings` / Use QMainWindow, QTextEdit, QFile, and QSettings
- 实现文件编辑、最近文件、自动保存和界面配置 / Implement editing, recent files, auto-save, and UI settings

### 第 40 章：网络下载器 / Chapter 40: Network Downloader

- 使用 `QNetworkAccessManager`、`QNetworkReply`、`QSaveFile` / Use QNetworkAccessManager, QNetworkReply, and QSaveFile
- 实现进度显示、取消下载、错误处理和断点续传 / Implement progress, cancellation, error handling, and resuming

### 第 41 章：数据库管理工具 / Chapter 41: Database Management Tool

- 使用 `QSqlDatabase`、`QSqlTableModel`、`QTableView` / Use QSqlDatabase, QSqlTableModel, and QTableView
- 实现增删改查、筛选、事务和数据导出 / Implement CRUD, filtering, transactions, and data export

### 第 42 章：TCP 聊天工具 / Chapter 42: TCP Chat Application

- 使用 `QTcpServer`、`QTcpSocket`、`QThread` / Use QTcpServer, QTcpSocket, and QThread
- 实现客户端管理、消息协议、心跳和断线重连 / Implement client management, messaging, heartbeats, and reconnection

---

## 附录 / Appendices

1. Qt 5 到 Qt 6 的常见迁移问题 / Common Qt 5 to Qt 6 migration issues
2. qmake 与 CMake 常用写法对照 / Common qmake and CMake patterns
3. 常用模块的 CMake 链接配置 / CMake linking for common modules
4. Qt 对象所有权与内存排查清单 / Object ownership and memory checklist
5. 信号槽常见错误清单 / Common signal-slot errors
6. Qt 插件与数据库驱动部署 / Deploying Qt plugins and database drivers
7. Windows、Linux 与 macOS 应用发布 / Deploying on Windows, Linux, and macOS
8. 常用类速查表 / Common class quick reference
