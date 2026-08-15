# QMenu 菜单 / QMenu Menus

## 1. 当前目录 / This Directory

本目录包含三个文件：

*This directory contains three files:*

```text
23.1.QMenu/
├── README.md        # 双语教程 / Bilingual tutorial
├── main.cpp         # QMenu 最小示例 / Minimal QMenu example
└── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
```

## 2. QMenu 是什么 / What Is QMenu?

`QMenu` 是由多个 `QAction` 组成的菜单控件，可作为菜单栏中的下拉菜单、按钮菜单或上下文菜单。

*`QMenu` is a menu widget made of `QAction` objects. It can serve as a menu-bar dropdown, a button menu, or a context menu.*

## 3. QMenu 的用法 / Using QMenu

### 添加菜单项 / Adding Items

```cpp
QMenu *fileMenu = new QMenu(parentWidget);
QAction *openAction = fileMenu->addAction(QStringLiteral("打开 / Open"));
fileMenu->addSeparator();
QMenu *recentMenu = fileMenu->addMenu(QStringLiteral("最近文件 / Recent files"));
```

`addAction()` 添加普通操作，`addSeparator()` 添加分隔线，`addMenu()` 添加子菜单。菜单通过操作的 `triggered()` 信号执行业务。

*`addAction()` adds an operation, `addSeparator()` adds a separator, and `addMenu()` adds a submenu. Business logic normally runs from an action's `triggered()` signal.*

### 显示菜单 / Showing a Menu

- `button->setMenu(menu)`：让按钮自动管理菜单弹出。  
  *Let a button display the menu automatically.*
- `menu->popup(globalPosition)`：异步显示并立即返回。  
  *Show asynchronously and return immediately.*
- `menu->exec(globalPosition)`：同步显示，关闭菜单后返回所选操作。  
  *Show synchronously and return the selected action after closing.*

通常优先使用信号和 `popup()`；只有确实需要立即获得返回值时才使用会启动局部事件循环的 `exec()`。

*Prefer signals and `popup()` in most cases. Use `exec()`, which starts a local event loop, only when an immediate return value is genuinely useful.*

### 常用信号 / Common Signals

- `aboutToShow()` / `aboutToHide()`：菜单显示或隐藏前发出。  
  *Emitted immediately before showing or hiding.*
- `hovered(QAction *)`：高亮菜单项时发出。  
  *Emitted when an item is highlighted.*
- `triggered(QAction *)`：菜单中的操作被触发时发出。  
  *Emitted when an action in the menu is triggered.*

## 4. 示例程序 / Example Program

下面的代码与本目录 `main.cpp` 完全一致：

*The following code is identical to `main.cpp` in this directory:*

```cpp
#include <QAction>
#include <QApplication>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环。
    // QApplication manages the event loop of a Widgets application.
    QApplication application(argc, argv);

    QWidget windowWidget;
    windowWidget.setWindowTitle(QStringLiteral("QMenu 最小示例 / Minimal Example"));

    // 按钮通过 setMenu() 持有一个可弹出的菜单。
    // The button exposes a popup menu through setMenu().
    QPushButton *menuPushButton = new QPushButton(
        QStringLiteral("文件操作 / File actions"), &windowWidget);
    QMenu *fileMenu = new QMenu(menuPushButton);
    QAction *openAction = fileMenu->addAction(QStringLiteral("打开 / Open"));
    QAction *saveAction = fileMenu->addAction(QStringLiteral("保存 / Save"));
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction(QStringLiteral("退出 / Exit"));
    menuPushButton->setMenu(fileMenu);

    QLabel *resultLabel = new QLabel(
        QStringLiteral("请选择菜单项 / Select a menu item"), &windowWidget);

    // 每个 QAction 的 triggered() 信号执行对应操作。
    // Each QAction performs its operation through triggered().
    QObject::connect(openAction, &QAction::triggered, resultLabel, [resultLabel] {
        resultLabel->setText(QStringLiteral("已选择打开 / Open selected"));
    });
    QObject::connect(saveAction, &QAction::triggered, resultLabel, [resultLabel] {
        resultLabel->setText(QStringLiteral("已选择保存 / Save selected"));
    });
    QObject::connect(exitAction, &QAction::triggered,
                     &application, &QApplication::quit);

    QVBoxLayout *mainVBoxLayout = new QVBoxLayout(&windowWidget);
    mainVBoxLayout->addWidget(menuPushButton);
    mainVBoxLayout->addWidget(resultLabel);

    windowWidget.resize(360, 140);
    windowWidget.show();
    return application.exec();
}
```

## 5. 实际业务场景 / Real-World Business Scenarios

- 文件、编辑和视图命令菜单。 / *File, edit, and view command menus.*
- 工具按钮的下拉选项。 / *Dropdown choices on tool buttons.*
- 动态生成“最近文件”或“导出格式”子菜单。 / *Dynamic recent-file or export-format submenus.*
- 鼠标右键上下文操作。 / *Right-click context operations.*

## 6. 常见错误 / Common Mistakes

### 使用 `show()` 显示弹出菜单 / Showing a Popup Menu with `show()`

弹出菜单应使用 `popup()`、`exec()`，或交给按钮/菜单栏显示，而不是直接调用 `show()`。

*Use `popup()`, `exec()`, a button, or a menu bar to display a popup menu rather than calling `show()` directly.*

### 坐标系错误 / Using the Wrong Coordinate System

`popup()` 和 `exec()` 接收全局坐标。来自控件的局部坐标要先使用 `mapToGlobal()` 转换。

*`popup()` and `exec()` expect global coordinates. Convert widget-local positions with `mapToGlobal()` first.*

### 重复执行逻辑 / Executing Logic Twice

若既连接 `triggered()` 又处理 `exec()` 的返回值，不要在两个位置重复执行同一业务。

*If you both connect `triggered()` and inspect the return value of `exec()`, do not perform the same operation twice.*

### 忽略对象所有权 / Ignoring Ownership

为菜单和操作设置合适的父对象，避免泄漏或悬空指针。`addAction(QString)` 创建的操作由菜单管理。

*Give menus and actions suitable parents to avoid leaks and dangling pointers. An action created by `addAction(QString)` is managed by the menu.*

## Qt 官方文档 / Official Qt Documentation

- [QMenu Class](https://doc.qt.io/qt-6/qmenu.html)
- [QAction Class](https://doc.qt.io/qt-6/qaction.html)
