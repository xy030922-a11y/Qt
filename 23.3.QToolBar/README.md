# QToolBar 工具栏 / QToolBar Toolbars

## 1. 当前目录 / This Directory

```text
23.3.QToolBar/
├── README.md        # 双语教程 / Bilingual tutorial
├── main.cpp         # QToolBar 最小示例 / Minimal QToolBar example
└── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
```

## 2. QToolBar 是什么 / What Is QToolBar?

`QToolBar` 是一组常用命令的快捷入口。它通常停靠在 `QMainWindow` 四周，并把 `QAction` 显示为按钮。

*`QToolBar` provides quick access to common commands. It normally docks around a QMainWindow and displays QAction objects as buttons.*

## 3. QToolBar 的用法 / Using QToolBar

### 创建并添加操作 / Creating and Adding Actions

```cpp
QToolBar *fileToolBar = new QToolBar(
    QStringLiteral("文件 / File"), mainWindow);
mainWindow->addToolBar(fileToolBar);

QAction *saveAction = fileToolBar->addAction(QStringLiteral("保存 / Save"));
fileToolBar->addSeparator();
```

同一个 `QAction` 可以同时加入菜单和工具栏。它们共享文本、图标、快捷键、启用状态和 `triggered()` 信号，避免维护两套命令逻辑。

*The same QAction can appear in both a menu and a toolbar. They share text, icon, shortcut, enabled state, and the `triggered()` signal, avoiding duplicate command logic.*

### 停靠行为 / Docking Behavior

```cpp
fileToolBar->setMovable(true);
fileToolBar->setFloatable(true);
fileToolBar->setAllowedAreas(
    Qt::TopToolBarArea | Qt::BottomToolBarArea);
```

- `setMovable()`：用户能否拖动工具栏。 / *Whether users may move the toolbar.*
- `setFloatable()`：工具栏能否成为浮动窗口。 / *Whether it may become a floating window.*
- `setAllowedAreas()`：限制允许停靠的区域。 / *Restrict allowed docking areas.*
- `setOrientation()`：仅对独立使用的工具栏直接设置方向；由 QMainWindow 管理时方向通常随停靠区变化。 / *Set orientation directly mainly for standalone use; a managed toolbar normally follows its docking area.*

### 显示样式 / Display Style

```cpp
fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
fileToolBar->setIconSize(QSize(24, 24));
```

最终外观受平台样式影响。应为操作提供清晰图标、文本和工具提示，不要只依赖图标含义。

*The platform style affects final appearance. Give actions clear icons, text, and tooltips rather than relying on icon meaning alone.*

## 4. 示例程序 / Example Program

下面的代码与本目录 `main.cpp` 完全一致：

*The following code is identical to `main.cpp` in this directory:*

```cpp
#include <QAction>
#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QToolBar>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环。
    // QApplication manages the event loop of a Widgets application.
    QApplication application(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle(QStringLiteral("QToolBar 最小示例 / Minimal Example"));

    // QMainWindow 负责停靠和管理工具栏。
    // QMainWindow docks and manages the toolbar.
    QToolBar *editToolBar = new QToolBar(
        QStringLiteral("编辑 / Edit"), &mainWindow);
    mainWindow.addToolBar(editToolBar);
    QAction *copyAction = editToolBar->addAction(QStringLiteral("复制 / Copy"));
    QAction *pasteAction = editToolBar->addAction(QStringLiteral("粘贴 / Paste"));

    QLabel *resultLabel = new QLabel(
        QStringLiteral("点击工具栏操作 / Click a toolbar action"));
    resultLabel->setAlignment(Qt::AlignCenter);
    mainWindow.setCentralWidget(resultLabel);

    // 工具栏按钮会触发对应 QAction 的 triggered() 信号。
    // A toolbar button emits triggered() on its corresponding QAction.
    QObject::connect(copyAction, &QAction::triggered, resultLabel, [resultLabel] {
        resultLabel->setText(QStringLiteral("已复制 / Copied"));
    });
    QObject::connect(pasteAction, &QAction::triggered, resultLabel, [resultLabel] {
        resultLabel->setText(QStringLiteral("已粘贴 / Pasted"));
    });

    mainWindow.resize(480, 260);
    mainWindow.show();
    return application.exec();
}
```

## 5. 实际业务场景 / Real-World Business Scenarios

- 编辑器中的新建、保存、撤销和重做。 / *New, save, undo, and redo in an editor.*
- 绘图软件中的工具和颜色入口。 / *Tool and color access in drawing software.*
- 数据应用中的刷新、筛选和导出。 / *Refresh, filter, and export in data applications.*
- 多工具栏工作区，允许用户移动和隐藏工具栏。 / *Workspaces with multiple movable and hideable toolbars.*

## 6. 常见错误 / Common Mistakes

### 在普通 QWidget 中期待停靠 / Expecting Docking in QWidget

自动停靠是 `QMainWindow` 的布局功能。独立 QToolBar 放入普通布局后不会自动获得完整停靠行为。

*Automatic docking is a QMainWindow layout feature. A standalone QToolBar in a plain layout does not automatically gain full docking behavior.*

### 为菜单和工具栏创建重复操作 / Duplicating Menu and Toolbar Actions

应该复用同一个 QAction，而不是创建两个分别连接的操作，否则启用状态和快捷键容易不一致。

*Reuse the same QAction instead of creating separately connected menu and toolbar actions, or their enabled states and shortcuts may diverge.*

### 只提供图标 / Providing Icons Only

图标可能难以理解或在某些主题中不可见。设置文本、工具提示，并使用适合浅色和深色主题的资源。

*Icons may be ambiguous or disappear in some themes. Provide text and tooltips, and use assets suitable for light and dark themes.*

### 不保存用户布局 / Not Saving User Layout

复杂主窗口可使用 `QMainWindow::saveState()` 和 `restoreState()` 保存工具栏位置；恢复前要为工具栏设置稳定的 `objectName`。

*Complex windows can preserve toolbar positions with `QMainWindow::saveState()` and `restoreState()`. Set a stable `objectName` before restoring.*

## Qt 官方文档 / Official Qt Documentation

- [QToolBar Class](https://doc.qt.io/qt-6/qtoolbar.html)
- [QMainWindow Class](https://doc.qt.io/qt-6/qmainwindow.html)
- [QAction Class](https://doc.qt.io/qt-6/qaction.html)
