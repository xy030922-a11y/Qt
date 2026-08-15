# QMenuBar 菜单栏 / QMenuBar Menu Bars

## 1. 当前目录 / This Directory

```text
23.2.QMenuBar/
├── README.md        # 双语教程 / Bilingual tutorial
├── main.cpp         # QMenuBar 最小示例 / Minimal QMenuBar example
└── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
```

## 2. QMenuBar 是什么 / What Is QMenuBar?

`QMenuBar` 是窗口顶部的菜单栏，通常包含“文件”“编辑”“视图”“帮助”等 `QMenu`。它负责组织菜单，具体命令仍由 `QAction` 表示。

*`QMenuBar` is the menu bar at the top of a window. It normally contains `QMenu` objects such as File, Edit, View, and Help, while individual commands remain represented by `QAction`.*

## 3. QMenuBar 的用法 / Using QMenuBar

`QMainWindow` 已经提供菜单栏区域，最简单的做法是使用 `menuBar()`：

*QMainWindow already provides a menu-bar area. The simplest approach is `menuBar()`:*

```cpp
QMenuBar *mainMenuBar = mainWindow->menuBar();
QMenu *fileMenu = mainMenuBar->addMenu(QStringLiteral("文件(&F) / &File"));
QAction *openAction = fileMenu->addAction(QStringLiteral("打开 / Open"));
```

- `addMenu()`：添加下拉菜单。 / *Add a dropdown menu.*
- `addAction()`：直接在菜单栏添加操作。 / *Add an action directly to the menu bar.*
- `addSeparator()`：添加分隔项，具体显示取决于平台样式。 / *Add a separator whose appearance depends on the platform style.*
- `setCornerWidget()`：在菜单栏角落放置自定义控件。 / *Place a custom widget in a menu-bar corner.*

菜单文本中的 `&` 定义助记键。`文件(&F)` 或 `&File` 通常允许用户使用 `Alt+F` 打开菜单。

*An `&` defines a mnemonic. `文件(&F)` or `&File` normally lets the user open the menu with `Alt+F`.*

macOS 可能把菜单栏整合到系统顶部，并根据标准操作角色重新排列某些菜单项，因此不要依赖所有平台完全相同的视觉位置。

*macOS may integrate the menu bar into the system bar and rearrange actions with standard roles, so do not assume identical visual placement on every platform.*

## 4. 示例程序 / Example Program

下面的代码与本目录 `main.cpp` 完全一致：

*The following code is identical to `main.cpp` in this directory:*

```cpp
#include <QAction>
#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环。
    // QApplication manages the event loop of a Widgets application.
    QApplication application(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle(QStringLiteral("QMenuBar 最小示例 / Minimal Example"));

    // QMainWindow::menuBar() 返回由主窗口管理的菜单栏。
    // QMainWindow::menuBar() returns the menu bar managed by the main window.
    QMenuBar *mainMenuBar = mainWindow.menuBar();
    QMenu *fileMenu = mainMenuBar->addMenu(QStringLiteral("文件(&F) / &File"));
    QAction *newAction = fileMenu->addAction(QStringLiteral("新建 / New"));
    QAction *exitAction = fileMenu->addAction(QStringLiteral("退出 / Exit"));

    QLabel *resultLabel = new QLabel(
        QStringLiteral("从菜单栏选择操作 / Select an action from the menu bar"));
    resultLabel->setAlignment(Qt::AlignCenter);
    mainWindow.setCentralWidget(resultLabel);

    // 菜单项仍由 QAction::triggered() 执行业务。
    // Menu commands still run through QAction::triggered().
    QObject::connect(newAction, &QAction::triggered, resultLabel, [resultLabel] {
        resultLabel->setText(QStringLiteral("已新建文档 / New document created"));
    });
    QObject::connect(exitAction, &QAction::triggered,
                     &application, &QApplication::quit);

    mainWindow.resize(480, 260);
    mainWindow.show();
    return application.exec();
}
```

## 5. 实际业务场景 / Real-World Business Scenarios

- 桌面编辑器的文件、编辑和格式菜单。 / *File, edit, and format menus in desktop editors.*
- IDE、设计工具和数据分析软件的分层命令入口。 / *Hierarchical command entry in IDEs, design tools, and analytics software.*
- 使用“最近文件”“窗口”菜单展示动态内容。 / *Dynamic Recent Files or Window menus.*
- 通过“帮助”菜单提供文档、更新和关于对话框。 / *Documentation, updates, and About dialogs under Help.*

## 6. 常见错误 / Common Mistakes

### 在普通 QWidget 中期待自动布局 / Expecting Automatic Layout in QWidget

`QMainWindow` 专门管理菜单栏；普通 `QWidget` 不会自动把 QMenuBar 放到窗口顶部，需要手动加入布局。

*QMainWindow manages a menu bar specially. A plain QWidget does not place QMenuBar automatically; add it to a layout yourself.*

### 手动删除 menuBar() 返回值 / Deleting the menuBar() Result

`QMainWindow` 管理由 `menuBar()` 创建的菜单栏，不要在窗口仍使用它时手动删除。

*QMainWindow manages the bar created by `menuBar()`; do not delete it while the window still uses it.*

### 把业务逻辑写在菜单栏中 / Putting Business Logic in the Menu Bar

让 `QAction` 表达命令并连接业务槽，可让同一个操作同时复用于菜单和工具栏。

*Represent commands with QAction and connect them to business slots so the same operation can be reused by menus and toolbars.*

### 忽略平台差异 / Ignoring Platform Differences

菜单位置、助记键显示和系统级菜单整合会随操作系统变化，应测试目标平台。

*Menu placement, mnemonic display, and system-menu integration vary by operating system. Test every target platform.*

## Qt 官方文档 / Official Qt Documentation

- [QMenuBar Class](https://doc.qt.io/qt-6/qmenubar.html)
- [QMenu Class](https://doc.qt.io/qt-6/qmenu.html)
- [QMainWindow Class](https://doc.qt.io/qt-6/qmainwindow.html)
