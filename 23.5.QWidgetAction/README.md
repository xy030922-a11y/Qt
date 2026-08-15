# QWidgetAction 带控件的操作 / Widget-Based Actions

## 1. 当前目录 / This Directory

```text
23.5.QWidgetAction/
├── README.md        # 双语教程 / Bilingual tutorial
├── main.cpp         # QWidgetAction 最小示例 / Minimal QWidgetAction example
└── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
```

## 2. QWidgetAction 是什么 / What Is QWidgetAction?

`QWidgetAction` 是一种可以在菜单或工具栏中显示自定义 `QWidget` 的操作，例如滑块、搜索框、组合框或颜色选择器。

*`QWidgetAction` is an action that can display a custom QWidget—such as a slider, search field, combo box, or color picker—inside a menu or toolbar.*

## 3. QWidgetAction 的用法 / Using QWidgetAction

### 设置默认控件 / Setting a Default Widget

```cpp
QWidgetAction *searchWidgetAction = new QWidgetAction(parentMenu);
QLineEdit *searchLineEdit = new QLineEdit;
searchWidgetAction->setDefaultWidget(searchLineEdit);
parentMenu->addAction(searchWidgetAction);
```

`setDefaultWidget()` 把控件交给 QWidgetAction 管理。操作被删除，或替换默认控件时，旧控件会被删除，因此不要再手动删除或复用该指针。

*`setDefaultWidget()` transfers management of the widget to QWidgetAction. The old widget is deleted when the action is deleted or its default widget is replaced, so do not delete or reuse that pointer manually.*

默认控件适合只显示在一个容器中。同一个操作需要同时出现在多个容器时，应继承 `QWidgetAction` 并重写 `createWidget()`，为每个容器创建独立控件。

*A default widget is suitable for one container. If the same action must appear in multiple containers, subclass QWidgetAction and override `createWidget()` to create a separate widget for each container.*

### 处理嵌入控件 / Handling the Embedded Widget

嵌入后，控件仍然使用自己的属性和信号：

*Once embedded, the widget continues to use its normal properties and signals:*

```cpp
connect(volumeSlider, &QSlider::valueChanged,
        receiver, [](int value) {
    // 使用新音量 / Use the new volume
});
```

对复杂控件，应考虑键盘焦点、最小尺寸和菜单关闭时机。菜单中的普通 QAction 被触发时通常会关闭菜单，而用户操作嵌入控件时往往希望菜单保持打开。

*For complex widgets, consider keyboard focus, minimum size, and when the menu closes. A normal QAction usually closes a menu when triggered, while users often expect the menu to remain open as they manipulate an embedded widget.*

## 4. 示例程序 / Example Program

下面的代码与本目录 `main.cpp` 完全一致：

*The following code is identical to `main.cpp` in this directory:*

```cpp
#include <QApplication>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QWidgetAction>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环。
    // QApplication manages the event loop of a Widgets application.
    QApplication application(argc, argv);

    QWidget windowWidget;
    windowWidget.setWindowTitle(
        QStringLiteral("QWidgetAction 最小示例 / Minimal Example"));

    QPushButton *volumePushButton = new QPushButton(
        QStringLiteral("调节音量 / Adjust volume"), &windowWidget);
    QMenu *volumeMenu = new QMenu(volumePushButton);

    // QWidgetAction 把一个真实的 QSlider 嵌入菜单。
    // QWidgetAction embeds a real QSlider inside the menu.
    QWidgetAction *volumeWidgetAction = new QWidgetAction(volumeMenu);
    QSlider *volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    volumeSlider->setMinimumWidth(220);
    volumeWidgetAction->setDefaultWidget(volumeSlider);
    volumeMenu->addAction(volumeWidgetAction);
    volumePushButton->setMenu(volumeMenu);

    QLabel *volumeLabel = new QLabel(
        QStringLiteral("音量 / Volume: 50"), &windowWidget);

    // 滑块仍使用自己的 valueChanged() 信号。
    // The embedded slider still uses its own valueChanged() signal.
    QObject::connect(volumeSlider, &QSlider::valueChanged,
                     volumeLabel, [volumeLabel](int value) {
        volumeLabel->setText(
            QStringLiteral("音量 / Volume: %1").arg(value));
    });

    QVBoxLayout *mainVBoxLayout = new QVBoxLayout(&windowWidget);
    mainVBoxLayout->addWidget(volumePushButton);
    mainVBoxLayout->addWidget(volumeLabel);

    windowWidget.resize(380, 140);
    windowWidget.show();
    return application.exec();
}
```

## 5. 实际业务场景 / Real-World Business Scenarios

- 音量、缩放比例或笔刷大小滑块。 / *Volume, zoom, or brush-size sliders.*
- 工具栏中的搜索输入框。 / *Search fields in toolbars.*
- 字体、颜色或排序方式组合框。 / *Font, color, or sort-order combo boxes.*
- 菜单中的账号切换器或简短筛选面板。 / *Account switchers or compact filter panels in menus.*

## 6. 常见错误 / Common Mistakes

### 手动删除默认控件 / Manually Deleting the Default Widget

调用 `setDefaultWidget()` 后由 QWidgetAction 管理该控件。再次手动删除可能造成悬空指针或重复释放。

*After `setDefaultWidget()`, QWidgetAction manages the widget. Deleting it manually can cause a dangling pointer or double deletion.*

### 在多个容器中复用一个控件 / Reusing One Widget in Multiple Containers

一个 QWidget 同时只能属于一个父控件。需要多处显示时重写 `createWidget()`，不要反复移动同一个默认控件。

*A QWidget can have only one parent at a time. Override `createWidget()` for multiple presentations instead of repeatedly moving one default widget.*

### 忽略焦点和键盘操作 / Ignoring Focus and Keyboard Use

嵌入输入控件后，应测试 Tab、方向键、Esc 和屏幕阅读器行为。

*After embedding an input widget, test Tab, arrow keys, Esc, and screen-reader behavior.*

### 把整个复杂页面塞进菜单 / Embedding an Entire Complex Page

菜单适合紧凑的临时操作。复杂表单应使用对话框、停靠面板或普通页面。

*Menus suit compact transient controls. Use a dialog, dock panel, or regular page for a complex form.*

## Qt 官方文档 / Official Qt Documentation

- [QWidgetAction Class](https://doc.qt.io/qt-6/qwidgetaction.html)
- [QMenu Class](https://doc.qt.io/qt-6/qmenu.html)
- [QToolBar Class](https://doc.qt.io/qt-6/qtoolbar.html)
