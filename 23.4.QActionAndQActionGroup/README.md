# QAction 与 QActionGroup / Actions and Action Groups

## 1. 当前目录 / This Directory

```text
23.4.QActionAndQActionGroup/
├── README.md        # 双语教程 / Bilingual tutorial
├── main.cpp         # 最小操作组示例 / Minimal action-group example
└── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
```

## 2. QAction 和 QActionGroup 是什么 / What Are QAction and QActionGroup?

`QAction` 表示一个可执行命令，集中保存文本、图标、快捷键、状态提示、启用状态和选中状态。菜单、工具栏等控件只是同一操作的不同展示方式。

*QAction represents an executable command and centralizes its text, icon, shortcut, status tip, enabled state, and checked state. Menus and toolbars are different presentations of the same action.*

`QActionGroup` 管理一组操作，可统一启用/禁用，并可让可选操作互斥，类似单选按钮组。

*QActionGroup manages a collection of actions, can enable or disable them together, and can make checkable actions exclusive like radio buttons.*

## 3. QAction 与 QActionGroup 的用法 / Using QAction and QActionGroup

### 创建操作 / Creating an Action

```cpp
QAction *saveAction = new QAction(
    QStringLiteral("保存 / Save"), parentObject);
saveAction->setShortcut(QKeySequence::Save);
saveAction->setStatusTip(QStringLiteral("保存当前文档 / Save the current document"));

connect(saveAction, &QAction::triggered, receiver, [] {
    // 执行保存 / Perform save
});
```

常用信号：

*Common signals:*

- `triggered(bool)`：操作被激活时发出；`setChecked()` 不会触发它。  
  *Emitted when activated; `setChecked()` does not emit it.*
- `toggled(bool)`：可选操作的选中状态改变时发出。  
  *Emitted when a checkable action changes checked state.*
- `changed()`：文本、图标、启用状态等属性改变时发出。  
  *Emitted when properties such as text, icon, or enabled state change.*
- `hovered()`：用户高亮操作时发出。  
  *Emitted when the user highlights the action.*

### 创建互斥操作组 / Creating an Exclusive Group

```cpp
QActionGroup *alignmentActionGroup = new QActionGroup(parentObject);
alignmentActionGroup->setExclusive(true);

QAction *leftAction = alignmentActionGroup->addAction(
    QStringLiteral("左对齐 / Align left"));
QAction *rightAction = alignmentActionGroup->addAction(
    QStringLiteral("右对齐 / Align right"));
leftAction->setCheckable(true);
rightAction->setCheckable(true);
leftAction->setChecked(true);
```

`QActionGroup::triggered(QAction *)` 会告诉槽函数具体触发了哪个成员，适合集中处理整个组。

*`QActionGroup::triggered(QAction *)` identifies the member that was triggered, which is convenient for centralized handling.*

Qt 6 的排他策略还可表达“必须始终选中一个”或“允许再次取消当前项”。选择策略时要匹配业务语义。

*Qt 6 exclusion policies can express either “one item must remain selected” or “the current item may be unchecked.” Match the policy to the business meaning.*

## 4. 示例程序 / Example Program

下面的代码与本目录 `main.cpp` 完全一致：

*The following code is identical to `main.cpp` in this directory:*

```cpp
#include <QAction>
#include <QActionGroup>
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
    windowWidget.setWindowTitle(
        QStringLiteral("QActionGroup 最小示例 / Minimal Example"));

    // 互斥组保证同一时间只有一个可选操作处于选中状态。
    // An exclusive group keeps exactly one checkable action selected at a time.
    QActionGroup *themeActionGroup = new QActionGroup(&windowWidget);
    themeActionGroup->setExclusive(true);
    QAction *lightAction = themeActionGroup->addAction(
        QStringLiteral("浅色主题 / Light theme"));
    QAction *darkAction = themeActionGroup->addAction(
        QStringLiteral("深色主题 / Dark theme"));
    lightAction->setCheckable(true);
    darkAction->setCheckable(true);
    lightAction->setChecked(true);

    QPushButton *themePushButton = new QPushButton(
        QStringLiteral("选择主题 / Choose theme"), &windowWidget);
    QMenu *themeMenu = new QMenu(themePushButton);
    themeMenu->addActions(themeActionGroup->actions());
    themePushButton->setMenu(themeMenu);

    QLabel *resultLabel = new QLabel(
        QStringLiteral("当前主题：浅色 / Current theme: Light"), &windowWidget);

    // 监听组信号即可统一处理所有成员操作。
    // Handle every member action through the group's signal.
    QObject::connect(themeActionGroup, &QActionGroup::triggered,
                     resultLabel, [resultLabel](QAction *selectedAction) {
        resultLabel->setText(
            QStringLiteral("当前主题 / Current theme: %1")
                .arg(selectedAction->text()));
    });

    QVBoxLayout *mainVBoxLayout = new QVBoxLayout(&windowWidget);
    mainVBoxLayout->addWidget(themePushButton);
    mainVBoxLayout->addWidget(resultLabel);

    windowWidget.resize(380, 140);
    windowWidget.show();
    return application.exec();
}
```

## 5. 实际业务场景 / Real-World Business Scenarios

- 同一“保存”操作同时出现在菜单、工具栏和快捷键中。 / *One Save action shared by a menu, toolbar, and shortcut.*
- 文本对齐、主题或工具模式的互斥选择。 / *Exclusive text alignment, theme, or tool-mode selection.*
- 根据文档状态统一启用或禁用一组编辑命令。 / *Enable or disable editing commands together based on document state.*
- 使用可选 QAction 表示显示/隐藏面板。 / *Checkable actions that show or hide panels.*

## 6. 常见错误 / Common Mistakes

### 为每个入口重复业务逻辑 / Duplicating Logic for Every Presentation

菜单和工具栏应共享 QAction。否则快捷键、禁用状态和图标很容易不同步。

*Menus and toolbars should share QAction objects, or shortcuts, disabled states, and icons easily become inconsistent.*

### 忘记 setCheckable(true) / Forgetting setCheckable(true)

只有可选操作才会保存选中状态并发出 `toggled()`。

*Only a checkable action stores a checked state and emits `toggled()`.*

### 组内没有初始选中项 / No Initially Checked Member

互斥组不会自动替你选择业务默认项。创建后应明确调用一个成员的 `setChecked(true)`。

*An exclusive group does not choose your business default automatically. Explicitly check one member after creation.*

### 混淆 triggered 与 toggled / Confusing triggered and toggled

`triggered()` 表示激活动作；`toggled()` 表示布尔选中状态变化。程序调用 `setChecked()` 只会涉及状态变化。

*`triggered()` means activation, while `toggled()` means a boolean checked-state change. A programmatic `setChecked()` concerns only state changes.*

## Qt 官方文档 / Official Qt Documentation

- [QAction Class](https://doc.qt.io/qt-6/qaction.html)
- [QActionGroup Class](https://doc.qt.io/qt-6/qactiongroup.html)
- [QKeySequence Class](https://doc.qt.io/qt-6/qkeysequence.html)
