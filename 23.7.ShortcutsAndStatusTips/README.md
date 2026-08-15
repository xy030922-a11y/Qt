# 快捷键与状态提示 / Shortcuts and Status Tips

## 1. 当前目录 / This Directory

```text
23.7.ShortcutsAndStatusTips/
├── README.md        # 双语教程 / Bilingual tutorial
├── main.cpp         # 快捷键与状态提示最小示例 / Minimal example
└── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
```

## 2. 快捷键与状态提示是什么 / What Are Shortcuts and Status Tips?

快捷键让用户无需打开菜单即可触发 QAction。状态提示是操作的简短说明，当用户在菜单或工具栏中高亮该操作时，QMainWindow 通常把它显示在状态栏。

*A shortcut triggers a QAction without opening a menu. A status tip briefly describes an action and is normally shown by QMainWindow in its status bar while the action is highlighted in a menu or toolbar.*

## 3. 快捷键与状态提示的用法 / Using Shortcuts and Status Tips

### 设置标准快捷键 / Setting a Standard Shortcut

```cpp
QAction *saveAction = new QAction(
    QStringLiteral("保存 / Save"), parentObject);
saveAction->setShortcut(QKeySequence::Save);
```

优先使用 `QKeySequence::StandardKey`（如 `Save`、`Open`、`Copy`、`Undo`），Qt 会根据操作系统选择符合用户习惯的按键。

*Prefer a QKeySequence standard key such as `Save`, `Open`, `Copy`, or `Undo`. Qt chooses the platform-appropriate key combination.*

自定义快捷键可使用可移植文本：

*A custom shortcut can use portable text:*

```cpp
customAction->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+K")));
```

### 快捷键上下文 / Shortcut Context

```cpp
saveAction->setShortcutContext(Qt::WindowShortcut);
```

- `Qt::WidgetShortcut`：只有父控件获得焦点时有效。 / *Active only when the parent widget has focus.*
- `Qt::WidgetWithChildrenShortcut`：父控件或其子控件有焦点时有效。 / *Active when the parent or one of its children has focus.*
- `Qt::WindowShortcut`：所属窗口活动时有效，QAction 默认使用它。 / *Active in the owning window; this is QAction's default.*
- `Qt::ApplicationShortcut`：应用中任意窗口活动时有效。 / *Active while any application window is active.*

范围越大越容易发生冲突，应选择满足业务需要的最小范围。

*A wider context creates more opportunities for conflicts. Choose the narrowest scope that meets the requirement.*

### 状态提示与工具提示 / Status Tips and Tooltips

```cpp
saveAction->setStatusTip(
    QStringLiteral("保存当前文档 / Save the current document"));
saveAction->setToolTip(
    QStringLiteral("保存 / Save"));
```

- `statusTip` 通常显示在 `QStatusBar`。 / *A status tip normally appears in QStatusBar.*
- `toolTip` 通常显示在工具按钮附近的浮动提示框。 / *A tooltip normally appears in a floating box near a tool button.*
- `whatsThis` 可提供更详细的“这是什么”帮助。 / *`whatsThis` can provide more detailed What's This help.*

`QStatusBar::showMessage(text, timeout)` 可以显示临时业务结果；超时后会恢复永久控件区域，但不会自动恢复之前的普通临时消息。

*`QStatusBar::showMessage(text, timeout)` displays a temporary business result. After the timeout, permanent widgets reappear, but a previous ordinary temporary message is not automatically restored.*

## 4. 示例程序 / Example Program

下面的代码与本目录 `main.cpp` 完全一致：

*The following code is identical to `main.cpp` in this directory:*

```cpp
#include <QAction>
#include <QApplication>
#include <QKeySequence>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环。
    // QApplication manages the event loop of a Widgets application.
    QApplication application(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle(
        QStringLiteral("快捷键与状态提示 / Shortcuts and Status Tips"));

    // 使用标准按键序列可自动适配不同操作系统。
    // A standard key sequence adapts automatically to each operating system.
    QAction *saveAction = new QAction(
        QStringLiteral("保存 / Save"), &mainWindow);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(
        QStringLiteral("保存当前文档 / Save the current document"));

    QMenu *fileMenu = mainWindow.menuBar()->addMenu(
        QStringLiteral("文件(&F) / &File"));
    fileMenu->addAction(saveAction);
    QToolBar *fileToolBar = mainWindow.addToolBar(
        QStringLiteral("文件 / File"));
    fileToolBar->addAction(saveAction);

    QLabel *resultLabel = new QLabel(
        QStringLiteral("按下保存快捷键 / Press the Save shortcut"));
    resultLabel->setAlignment(Qt::AlignCenter);
    mainWindow.setCentralWidget(resultLabel);

    QStatusBar *mainStatusBar = mainWindow.statusBar();
    mainStatusBar->showMessage(QStringLiteral("就绪 / Ready"));

    // 菜单、工具栏和快捷键都会触发同一个 QAction。
    // The menu, toolbar, and shortcut all trigger the same QAction.
    QObject::connect(saveAction, &QAction::triggered,
                     resultLabel, [resultLabel, mainStatusBar] {
        resultLabel->setText(QStringLiteral("文档已保存 / Document saved"));
        mainStatusBar->showMessage(
            QStringLiteral("保存成功 / Saved successfully"), 2000);
    });

    mainWindow.resize(520, 300);
    mainWindow.show();
    return application.exec();
}
```

## 5. 实际业务场景 / Real-World Business Scenarios

- 编辑器的保存、撤销、查找和替换。 / *Save, undo, find, and replace in editors.*
- 专业软件中的工具模式快速切换。 / *Fast tool-mode switching in professional software.*
- 菜单高亮时在状态栏解释危险或复杂操作。 / *Explain risky or complex commands in the status bar while highlighted.*
- 保存、导出或连接完成后显示短暂结果。 / *Show temporary results after saving, exporting, or connecting.*

## 6. 常见错误 / Common Mistakes

### 硬编码平台快捷键 / Hard-Coding Platform Shortcuts

直接写 `Ctrl+S` 可能不符合所有系统习惯。标准命令优先使用 `QKeySequence::Save` 等标准键。

*A literal `Ctrl+S` may not match every platform. Prefer standard keys such as `QKeySequence::Save` for standard commands.*

### 快捷键冲突 / Shortcut Conflicts

同一上下文中的两个可用操作使用相同快捷键时会产生歧义，操作可能不会触发。集中管理并测试快捷键。

*Two enabled actions with the same shortcut in one context create ambiguity, and neither may trigger. Manage shortcuts centrally and test them.*

### QAction 没有加入有效控件 / Action Not Added to an Active Widget

只有被窗口、菜单、工具栏或其他有效控件关联的 QAction 才能在相应上下文中处理快捷键。

*A QAction must be associated with a window, menu, toolbar, or another active widget to handle its shortcut in that context.*

### 混淆状态提示和业务状态 / Confusing Status Tips with Business Status

`statusTip` 解释“这个操作做什么”；`showMessage()` 更适合显示“操作执行结果”。

*A status tip explains what an action does, while `showMessage()` is better for the result of an operation.*

### 只依赖快捷键 / Relying Only on Shortcuts

核心功能还应在菜单或工具栏中可发现，并为辅助技术提供清晰文本。

*Core commands should remain discoverable in menus or toolbars and expose clear text to assistive technology.*

## Qt 官方文档 / Official Qt Documentation

- [QKeySequence Class](https://doc.qt.io/qt-6/qkeysequence.html)
- [QAction Class](https://doc.qt.io/qt-6/qaction.html)
- [QStatusBar Class](https://doc.qt.io/qt-6/qstatusbar.html)
