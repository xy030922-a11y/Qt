# 菜单、工具栏与操作 / Menus, Toolbars, and Actions

本章使用 **Qt 6 + C++17 + CMake**，通过一个最小文本编辑器示例介绍 `QMenu`、`QMenuBar`、`QToolBar`、`QAction`、`QActionGroup`、`QWidgetAction`、上下文菜单、快捷键和状态提示。

*This chapter uses **Qt 6 + C++17 + CMake** and introduces `QMenu`, `QMenuBar`, `QToolBar`, `QAction`, `QActionGroup`, `QWidgetAction`, context menus, shortcuts, and status tips through a minimal text editor example.*

## 1. 当前目录 / This Directory

目录中只包含运行本例所需的三个文件：

*The directory contains only the three files required by this example:*

```text
Chapter23.Menus,Toolbars,andActions/
├── README.md        # 双语教程 / Bilingual tutorial
├── main.cpp         # 最小示例 / Minimal example
└── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
```

- `README.md`：说明各个类的用法、业务场景和常见错误。  
  *Explains how the classes are used, their business scenarios, and common mistakes.*
- `main.cpp`：创建一个同时演示菜单、工具栏、操作和右键菜单的小型编辑器。  
  *Creates a small editor that demonstrates menus, a toolbar, actions, and a context menu.*
- `CMakeLists.txt`：查找 Qt 6 Widgets，构建并链接示例。  
  *Finds Qt 6 Widgets, then builds and links the example.*

## 2. 这些类是什么 / What Are These Classes?

### 2.1 QMenu：菜单 / Menus

`QMenu` 表示一组命令，可以放入菜单栏、作为另一个菜单的子菜单，或通过 `exec()` 在鼠标位置弹出。菜单中的实际命令通常由 `QAction` 表示。

*`QMenu` represents a group of commands. It can be placed in a menu bar, nested as a submenu, or shown at the mouse position with `exec()`. Commands inside the menu are normally represented by `QAction` objects.*

### 2.2 QMenuBar：菜单栏 / Menu Bars

`QMenuBar` 是窗口顶部的顶层菜单容器。`QMainWindow::menuBar()` 会按需创建并返回主窗口的菜单栏。

*`QMenuBar` is the top-level menu container at the top of a window. `QMainWindow::menuBar()` creates and returns the main window's menu bar when needed.*

### 2.3 QToolBar：工具栏 / Toolbars

`QToolBar` 把常用命令显示成可以快速点击的按钮，也可以容纳分隔符和控件。通过 `QMainWindow::addToolBar()` 添加后，工具栏通常可以移动或停靠。

*`QToolBar` exposes frequent commands as quickly accessible buttons and can also hold separators and widgets. After it is added with `QMainWindow::addToolBar()`, it can normally be moved or docked.*

### 2.4 QAction 与 QActionGroup / Actions and Action Groups

`QAction` 是“命令”的抽象，可统一保存文本、图标、快捷键、启用状态、选中状态、工具提示和状态提示。同一个操作可同时出现在菜单、工具栏和上下文菜单中。

*`QAction` is an abstraction of a command. It keeps the text, icon, shortcut, enabled state, checked state, tool tip, and status tip together. The same action can appear in a menu, toolbar, and context menu.*

`QActionGroup` 管理一组操作。把可选操作设为 `checkable`，再使用互斥组，可以实现文本对齐方式、主题或工具模式等单选行为。

*`QActionGroup` manages a set of actions. Making actions checkable and placing them in an exclusive group provides radio-button-like behavior for text alignment, themes, or tool modes.*

### 2.5 QWidgetAction：带控件的操作 / Widget-Based Actions

`QWidgetAction` 继承自 `QAction`，用于在工具栏或菜单等基于操作的容器中显示自定义控件。本例把 `QLineEdit` 搜索框作为一个 `QWidgetAction` 放入工具栏。

*`QWidgetAction` inherits `QAction` and displays a custom widget in action-based containers such as toolbars or menus. This example places a `QLineEdit` search field in the toolbar through a `QWidgetAction`.*

### 2.6 上下文菜单 / Context Menus

上下文菜单通常由右键或键盘菜单键打开。它不是单独的命令模型；常见实现是接收 `customContextMenuRequested` 信号，在指定位置弹出一个临时 `QMenu`。

*A context menu is normally opened by right-clicking or pressing the keyboard menu key. It is not a separate command model; a common implementation handles `customContextMenuRequested` and opens a temporary `QMenu` at the requested position.*

### 2.7 快捷键与状态提示 / Shortcuts and Status Tips

`QAction::setShortcut()` 为命令设置键盘入口。`QAction::setStatusTip()` 设置用户指向菜单项或工具栏按钮时显示在状态栏中的简短说明。

*`QAction::setShortcut()` gives a command a keyboard entry point. `QAction::setStatusTip()` defines the short explanation shown in the status bar when the user points to a menu item or toolbar button.*

## 3. 用法 / Usage

### 3.1 创建主窗口和中央控件 / Creating the Main Window and Central Widget

菜单栏、工具栏和状态栏最适合配合 `QMainWindow` 使用。中央编辑区通过 `setCentralWidget()` 设置：

*Menus, toolbars, and status bars work naturally with `QMainWindow`. Set the central editing area with `setCentralWidget()`:*

```cpp
QMainWindow mainWindow;
QTextEdit *editorTextEdit = new QTextEdit(&mainWindow);
mainWindow.setCentralWidget(editorTextEdit);

QStatusBar *mainStatusBar = mainWindow.statusBar();
mainStatusBar->showMessage(QStringLiteral("就绪 / Ready"));
```

### 3.2 创建并连接 QAction / Creating and Connecting a QAction

操作应创建一次，然后复用。给操作传入主窗口作为父对象，可由 Qt 的父子对象机制管理其生命周期：

*Create an action once and reuse it. Giving the main window as its parent lets Qt's parent-child mechanism manage its lifetime:*

```cpp
QAction *saveAction = new QAction(
    QStringLiteral("保存(&S) / Save"), &mainWindow);
saveAction->setShortcut(QKeySequence::Save);
saveAction->setStatusTip(
    QStringLiteral("保存当前内容 / Save the current content"));

QObject::connect(saveAction, &QAction::triggered,
                 &mainWindow, []() {
    // 执行保存逻辑。
    // Perform the save operation.
});
```

操作文本中的 `&` 会指定键盘助记键。若要显示真正的 `&`，应写成 `&&`。

*The `&` in action text marks a keyboard mnemonic. Write `&&` to display a literal ampersand.*

### 3.3 构建菜单栏和菜单 / Building the Menu Bar and Menus

```cpp
QMenuBar *mainMenuBar = mainWindow.menuBar();
QMenu *fileMenu = mainMenuBar->addMenu(
    QStringLiteral("文件(&F) / File"));
fileMenu->addAction(saveAction);
fileMenu->addSeparator();
fileMenu->addAction(exitAction);
```

`addMenu()` 可以继续创建子菜单；`addSeparator()` 用于对相关命令分组，但不应添加过多分隔符。

*`addMenu()` can also create submenus. Use `addSeparator()` to group related commands, but avoid excessive separators.*

### 3.4 复用操作到工具栏 / Reusing Actions in a Toolbar

```cpp
QToolBar *mainToolBar = new QToolBar(
    QStringLiteral("主工具栏 / Main Toolbar"), &mainWindow);
mainWindow.addToolBar(mainToolBar);
mainToolBar->addAction(saveAction);
```

菜单项与工具栏按钮引用同一个 `QAction`，因此它们会共享快捷键、启用状态、选中状态以及 `triggered()` 信号。

*The menu item and toolbar button reference the same `QAction`, so they share the shortcut, enabled state, checked state, and `triggered()` signal.*

给工具栏设置稳定且非空的 `objectName`，以后使用 `QMainWindow::saveState()` 保存布局时可以避免警告并正确恢复工具栏位置。

*Give the toolbar a stable, non-empty `objectName`. This avoids warnings and allows its position to be restored correctly if `QMainWindow::saveState()` is used later.*

### 3.5 使用互斥 QActionGroup / Using an Exclusive QActionGroup

```cpp
QActionGroup *alignmentActionGroup = new QActionGroup(&mainWindow);
alignmentActionGroup->setExclusive(true);

QAction *leftAlignAction = new QAction(
    QStringLiteral("左对齐 / Align Left"), alignmentActionGroup);
leftAlignAction->setCheckable(true);
leftAlignAction->setChecked(true);
```

把 `QActionGroup` 设为父对象会自动把操作加入该组。互斥只对可选操作生效，因此不要忘记 `setCheckable(true)`。

*Using the `QActionGroup` as the parent automatically inserts the action into the group. Exclusivity applies to checkable actions, so do not forget `setCheckable(true)`.*

### 3.6 在工具栏中放置 QWidgetAction / Placing a QWidgetAction in a Toolbar

```cpp
QWidgetAction *searchWidgetAction = new QWidgetAction(&mainWindow);
QLineEdit *searchLineEdit = new QLineEdit;
searchLineEdit->setPlaceholderText(
    QStringLiteral("查找文本 / Find text"));

searchWidgetAction->setDefaultWidget(searchLineEdit);
mainToolBar->addAction(searchWidgetAction);
```

`setDefaultWidget()` 会把控件所有权转交给 `QWidgetAction`，不应再手动删除该控件。一个默认控件同时加入多个容器时，只会显示在第一个支持它的容器中；需要多个实例时应继承 `QWidgetAction` 并重写 `createWidget()`。

*`setDefaultWidget()` transfers ownership of the widget to `QWidgetAction`, so the widget must not be deleted manually. If one default widget is added to multiple containers, it appears only in the first supported container. Subclass `QWidgetAction` and reimplement `createWidget()` when multiple widget instances are required.*

### 3.7 创建上下文菜单 / Creating a Context Menu

```cpp
editorTextEdit->setContextMenuPolicy(Qt::CustomContextMenu);
QObject::connect(
    editorTextEdit, &QWidget::customContextMenuRequested,
    editorTextEdit,
    [editorTextEdit, cutAction, copyAction](const QPoint &position) {
        QMenu contextMenu(editorTextEdit);
        contextMenu.addAction(cutAction);
        contextMenu.addAction(copyAction);
        contextMenu.exec(editorTextEdit->mapToGlobal(position));
    });
```

信号中的坐标属于接收控件，`QMenu::exec()` 需要全局坐标，所以应使用 `mapToGlobal()` 转换。临时菜单关闭后可安全销毁，而复用操作仍由主窗口拥有。

*The signal position is local to the receiving widget, while `QMenu::exec()` expects global coordinates, so convert it with `mapToGlobal()`. The temporary menu can be destroyed after it closes; the reused actions remain owned by the main window.*

## 4. 示例程序 / Example Program

本目录的 `main.cpp` 创建一个小型文本编辑器，包含以下可交互功能：

*The `main.cpp` in this directory creates a small text editor with these interactive features:*

- 使用文件、编辑、格式和帮助菜单。  
  *Use File, Edit, Format, and Help menus.*
- 从菜单、工具栏或快捷键触发同一个操作。  
  *Trigger the same action from a menu, toolbar, or shortcut.*
- 使用互斥操作切换左对齐、居中和右对齐。  
  *Switch among left, center, and right alignment with exclusive actions.*
- 在工具栏搜索框中输入文本并按 Enter 查找。  
  *Enter text in the toolbar search field and press Enter to find it.*
- 在编辑区右键打开复用剪切、复制、粘贴操作的上下文菜单。  
  *Right-click the editor to open a context menu that reuses Cut, Copy, and Paste actions.*
- 将菜单项和工具栏按钮的说明显示在状态栏中。  
  *Show menu-item and toolbar-button descriptions in the status bar.*

示例的关键结构如下，完整且可编译的实现位于 `main.cpp`：

*The key structure is shown below. The complete, buildable implementation is in `main.cpp`:*

```cpp
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QTextEdit>
#include <QToolBar>
#include <QWidgetAction>

int main(int argc, char *argv[])
{
    // 创建应用、主窗口和中央编辑器。
    // Create the application, main window, and central editor.
    QApplication application(argc, argv);
    QMainWindow mainWindow;
    QTextEdit *editorTextEdit = new QTextEdit(&mainWindow);
    mainWindow.setCentralWidget(editorTextEdit);

    // 创建一次操作并连接业务逻辑。
    // Create the action once and connect its business logic.
    QAction *newAction = new QAction(
        QStringLiteral("新建(&N) / New"), &mainWindow);
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(
        QStringLiteral("清空编辑器 / Clear the editor"));
    QObject::connect(newAction, &QAction::triggered,
                     editorTextEdit, &QTextEdit::clear);

    // 同一操作可同时用于菜单和工具栏。
    // The same action can be used by both a menu and a toolbar.
    QMenu *fileMenu = mainWindow.menuBar()->addMenu(
        QStringLiteral("文件(&F) / File"));
    fileMenu->addAction(newAction);

    QToolBar *mainToolBar = new QToolBar(&mainWindow);
    mainWindow.addToolBar(mainToolBar);
    mainToolBar->addAction(newAction);

    // QWidgetAction 可在工具栏中显示输入控件。
    // QWidgetAction can display an input widget in a toolbar.
    QWidgetAction *searchWidgetAction = new QWidgetAction(&mainWindow);
    QLineEdit *searchLineEdit = new QLineEdit;
    searchWidgetAction->setDefaultWidget(searchLineEdit);
    mainToolBar->addAction(searchWidgetAction);

    mainWindow.show();
    return application.exec();
}
```

### 构建和运行 / Build and Run

使用 Qt Creator 时，打开 `CMakeLists.txt`，选择 Qt 6 Desktop Kit，然后构建并运行 `MenusToolbarsActionsExample`。

*In Qt Creator, open `CMakeLists.txt`, select a Qt 6 Desktop Kit, then build and run `MenusToolbarsActionsExample`.*

当前机器也可以使用以下 PowerShell 命令：

*The current machine can also use these PowerShell commands:*

```powershell
$env:Path = "C:\Qt\6.5.3\mingw_64\bin;C:\Qt\Tools\mingw1120_64\bin;C:\Qt\Tools\Ninja;$env:Path"

cmake -S . -B build -G Ninja `
  -DCMAKE_PREFIX_PATH=C:/Qt/6.5.3/mingw_64 `
  -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1120_64/bin/g++.exe `
  -DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/Ninja/ninja.exe

cmake --build build
.\build\MenusToolbarsActionsExample.exe
```

编译器必须与 Qt 套件匹配。MinGW 版 Qt 应使用兼容的 MinGW 编译器，不能直接换成 MSVC。

*The compiler must match the Qt kit. A MinGW Qt build requires a compatible MinGW compiler and cannot simply be replaced by MSVC.*

## 5. 实际业务场景 / Real-World Business Scenarios

### 5.1 文档编辑器 / Document Editors

文件、编辑和格式命令通常同时出现在菜单与工具栏中。使用同一个 `QAction` 能确保“保存”等命令的启用状态、快捷键和逻辑保持一致。

*File, Edit, and Format commands commonly appear in both menus and toolbars. Reusing one `QAction` keeps the enabled state, shortcut, and logic of commands such as Save consistent.*

### 5.2 管理系统和桌面工具 / Administration and Desktop Tools

工具栏适合放置刷新、导出、筛选等高频操作，菜单则可保留完整命令集合。对表格或树视图右键时，可以根据当前选中项动态启用编辑或删除操作。

*A toolbar suits frequent operations such as Refresh, Export, and Filter, while menus retain the full command set. When a table or tree view is right-clicked, Edit or Delete actions can be enabled dynamically according to the current selection.*

### 5.3 绘图和设计软件 / Drawing and Design Applications

`QActionGroup` 可管理选择、画笔、直线等互斥工具；可选操作的选中状态可以同时反映在菜单和工具栏按钮上。

*`QActionGroup` can manage mutually exclusive tools such as Select, Brush, and Line. The checked state appears consistently in menu items and toolbar buttons.*

### 5.4 搜索、缩放和筛选控件 / Search, Zoom, and Filter Controls

`QWidgetAction` 可把搜索框、缩放下拉框或筛选器封装为操作并放入工具栏。需要同一控件同时出现在多个工具栏或菜单时，应为每个容器创建控件实例。

*`QWidgetAction` can wrap a search field, zoom combo box, or filter and place it in a toolbar. Create a widget instance for each container when the same control must appear in multiple toolbars or menus.*

### 5.5 权限与应用状态 / Permissions and Application State

用户没有权限、没有选中内容或应用处于离线状态时，可以调用 `QAction::setEnabled(false)`。所有引用该操作的菜单项和按钮都会同步禁用。

*Call `QAction::setEnabled(false)` when a user lacks permission, nothing is selected, or the application is offline. Every menu item and button that references the action is disabled together.*

## 6. 常见错误 / Common Mistakes

### 错误 1：为菜单和工具栏重复创建操作 / Duplicating Actions for Menus and Toolbars

重复的 `QAction` 容易出现快捷键、启用状态或业务逻辑不一致。应为同一命令创建一个操作，并加入多个界面容器。

*Duplicate `QAction` objects easily develop inconsistent shortcuts, enabled states, or business logic. Create one action per command and add it to multiple UI containers.*

### 错误 2：没有给 QAction 设置父对象 / Giving QAction No Parent

无父对象的堆操作可能泄漏。通常把所属窗口传给构造函数，或把 `QActionGroup` 作为父对象。

*A heap-allocated action without a parent may leak. Normally pass the owning window to the constructor, or use the `QActionGroup` as the parent.*

### 错误 3：QActionGroup 中的操作没有设为可选 / Actions in a Group Are Not Checkable

`setExclusive(true)` 不会自动让操作变为可选。每个参与单选行为的操作仍需调用 `setCheckable(true)`，并通常给一个操作设置初始选中状态。

*`setExclusive(true)` does not make actions checkable automatically. Every action that participates in radio-button-like behavior still needs `setCheckable(true)`, and one action should normally be checked initially.*

### 错误 4：上下文菜单坐标使用错误 / Using the Wrong Context-Menu Coordinates

`customContextMenuRequested` 通常提供控件局部坐标，而 `QMenu::exec()` 需要全局坐标。忘记调用 `mapToGlobal()` 会让菜单出现在错误位置。

*`customContextMenuRequested` normally provides widget-local coordinates, while `QMenu::exec()` needs global coordinates. Forgetting `mapToGlobal()` makes the menu appear at the wrong location.*

### 错误 5：手动删除 QWidgetAction 的默认控件 / Deleting a QWidgetAction's Default Widget Manually

`setDefaultWidget()` 会转移所有权。再次手动删除可能导致悬空指针或重复释放。

*`setDefaultWidget()` transfers ownership. Deleting the widget manually afterward can cause a dangling pointer or double deletion.*

### 错误 6：把一个默认控件加入多个容器 / Adding One Default Widget to Multiple Containers

同一个 QWidget 不能同时显示在多个位置。默认控件只会出现在第一个支持它的容器中。需要多份界面时应重写 `QWidgetAction::createWidget()`。

*One QWidget cannot be displayed in multiple places simultaneously. A default widget appears only in the first supported container. Reimplement `QWidgetAction::createWidget()` when multiple UI instances are required.*

### 错误 7：快捷键冲突或作用域过大 / Conflicting or Overly Broad Shortcuts

两个启用的操作使用相同快捷键时会产生歧义。设置自定义快捷键前应检查冲突，并根据需要选择合适的 `shortcutContext`。

*Two enabled actions with the same shortcut create ambiguity. Check for conflicts before assigning custom shortcuts and choose an appropriate `shortcutContext` when needed.*

### 错误 8：工具栏没有 objectName / Giving a Toolbar No objectName

如果以后使用 `QMainWindow::saveState()` 和 `restoreState()` 保存布局，没有稳定 `objectName` 的工具栏无法可靠恢复位置。

*If `QMainWindow::saveState()` and `restoreState()` are later used to persist the layout, a toolbar without a stable `objectName` cannot have its position restored reliably.*

## Qt 官方文档 / Official Qt Documentation

- [QMenu Class](https://doc.qt.io/qt-6/qmenu.html)
- [QMenuBar Class](https://doc.qt.io/qt-6/qmenubar.html)
- [QToolBar Class](https://doc.qt.io/qt-6/qtoolbar.html)
- [QAction Class](https://doc.qt.io/qt-6/qaction.html)
- [QActionGroup Class](https://doc.qt.io/qt-6/qactiongroup.html)
- [QWidgetAction Class](https://doc.qt.io/qt-6/qwidgetaction.html)
- [QMainWindow Class](https://doc.qt.io/qt-6/qmainwindow.html)
- [QWidget Context Menus](https://doc.qt.io/qt-6/qwidget.html#contextMenuPolicy-prop)
- [QKeySequence Class](https://doc.qt.io/qt-6/qkeysequence.html)
