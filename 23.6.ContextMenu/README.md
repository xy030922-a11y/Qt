# 上下文菜单 / Context Menus

## 1. 当前目录 / This Directory

```text
23.6.ContextMenu/
├── README.md        # 双语教程 / Bilingual tutorial
├── main.cpp         # 上下文菜单最小示例 / Minimal context-menu example
└── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
```

## 2. 上下文菜单是什么 / What Is a Context Menu?

上下文菜单是在特定位置调用的操作列表，桌面端通常由鼠标右键或键盘菜单键打开。菜单内容应针对用户点击的对象或区域。

*A context menu is a list of operations invoked at a specific location, usually with a right-click or menu key on desktop systems. Its contents should apply to the object or area that the user targeted.*

Qt 中通常使用 `QWidget::contextMenuPolicy`、`customContextMenuRequested()` 和 `QMenu` 实现。

*In Qt, it is commonly implemented with `QWidget::contextMenuPolicy`, `customContextMenuRequested()`, and QMenu.*

## 3. 上下文菜单的用法 / Using Context Menus

### CustomContextMenu 方式 / The CustomContextMenu Approach

```cpp
targetWidget->setContextMenuPolicy(Qt::CustomContextMenu);

connect(targetWidget, &QWidget::customContextMenuRequested,
        targetWidget, [targetWidget](const QPoint &position) {
    QMenu contextMenu(targetWidget);
    contextMenu.addAction(QStringLiteral("复制 / Copy"));
    contextMenu.exec(targetWidget->mapToGlobal(position));
});
```

信号参数通常是相对于控件的局部坐标，而 `QMenu::exec()` 和 `popup()` 需要全局坐标，因此必须使用 `mapToGlobal()`。

*The signal normally provides widget-local coordinates, while `QMenu::exec()` and `popup()` require global coordinates, so convert with `mapToGlobal()`.*

### 其他策略 / Other Policies

- `Qt::NoContextMenu`：控件不处理上下文菜单。 / *The widget does not handle context menus.*
- `Qt::DefaultContextMenu`：通过重写 `contextMenuEvent()` 处理。 / *Handle by overriding `contextMenuEvent()`.*
- `Qt::ActionsContextMenu`：自动把控件的 `actions()` 列表显示为菜单。 / *Automatically display the widget's `actions()` list.*
- `Qt::PreventContextMenu`：阻止上下文菜单事件继续处理。 / *Prevent further context-menu handling.*

可复用的固定操作适合 `ActionsContextMenu`；需要根据点击对象动态生成内容时，`CustomContextMenu` 更直接。

*Use `ActionsContextMenu` for a reusable fixed action list, and `CustomContextMenu` when the content depends on the clicked object.*

### exec() 与 popup() / exec() Versus popup()

`exec()` 同步等待并返回所选 QAction；`popup()` 异步显示，应通过 QAction 信号处理结果。复杂应用通常更适合异步信号方式。

*`exec()` waits synchronously and returns the selected QAction. `popup()` is asynchronous, so results are handled through action signals. Signal-based asynchronous handling usually scales better in complex applications.*

## 4. 示例程序 / Example Program

下面的代码与本目录 `main.cpp` 完全一致：

*The following code is identical to `main.cpp` in this directory:*

```cpp
#include <QAction>
#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QMenu>
#include <QWidget>

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环。
    // QApplication manages the event loop of a Widgets application.
    QApplication application(argc, argv);

    QLabel contextLabel(
        QStringLiteral("在此处单击右键 / Right-click here"));
    contextLabel.setWindowTitle(
        QStringLiteral("上下文菜单最小示例 / Context Menu Example"));
    contextLabel.setAlignment(Qt::AlignCenter);
    contextLabel.setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    // CustomContextMenu 让控件通过信号报告右键位置。
    // CustomContextMenu makes the widget report the right-click position by signal.
    contextLabel.setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(&contextLabel, &QWidget::customContextMenuRequested,
                     &contextLabel, [&contextLabel](const QPoint &position) {
        QMenu contextMenu(&contextLabel);
        QAction *copyAction = contextMenu.addAction(
            QStringLiteral("复制 / Copy"));
        QAction *resetAction = contextMenu.addAction(
            QStringLiteral("重置 / Reset"));

        // exec() 需要全局坐标，并返回用户选择的操作。
        // exec() expects global coordinates and returns the selected action.
        QAction *selectedAction = contextMenu.exec(
            contextLabel.mapToGlobal(position));
        if (selectedAction == copyAction) {
            contextLabel.setText(QStringLiteral("已选择复制 / Copy selected"));
        } else if (selectedAction == resetAction) {
            contextLabel.setText(
                QStringLiteral("在此处单击右键 / Right-click here"));
        }
    });

    contextLabel.resize(420, 220);
    contextLabel.show();
    return application.exec();
}
```

## 5. 实际业务场景 / Real-World Business Scenarios

- 文本编辑器中的复制、粘贴和格式操作。 / *Copy, paste, and formatting in a text editor.*
- 文件列表中的打开、重命名和删除。 / *Open, rename, and delete in a file list.*
- 表格单元格的筛选、排序和导出。 / *Filter, sort, and export for table cells.*
- 画布对象的图层、对齐和属性操作。 / *Layer, alignment, and property commands for canvas objects.*

## 6. 常见错误 / Common Mistakes

### 忘记转换坐标 / Forgetting Coordinate Conversion

直接把局部 `position` 传给 `exec()` 会让菜单出现在错误位置。使用目标控件的 `mapToGlobal()`。

*Passing the local `position` directly to `exec()` places the menu incorrectly. Use `mapToGlobal()` on the target widget.*

### 在父控件监听子控件右键 / Listening on the Parent Instead of the Child

右键事件会到达指针下的具体控件。若子控件覆盖父窗口，应在正确的子控件上设置策略或统一处理事件。

*A context event reaches the widget under the pointer. If children cover the parent, set the policy on the correct child or centralize event handling deliberately.*

### 显示不适用的操作 / Showing Inapplicable Actions

应根据当前选择设置 QAction 的启用和可见状态，例如没有选中文本时禁用“复制”。

*Set action enabled and visible states from the current selection—for example, disable Copy when no text is selected.*

### 只支持鼠标 / Supporting Mouse Only

确保键盘菜单键、Shift+F10 和辅助技术也能调用核心操作；不要把功能只藏在右键菜单中。

*Ensure that the menu key, Shift+F10, and assistive technologies can reach core operations. Do not hide essential functionality exclusively in a right-click menu.*

## Qt 官方文档 / Official Qt Documentation

- [QWidget Context Menu Policy](https://doc.qt.io/qt-6/qwidget.html#contextMenuPolicy-prop)
- [QContextMenuEvent Class](https://doc.qt.io/qt-6/qcontextmenuevent.html)
- [QMenu Class](https://doc.qt.io/qt-6/qmenu.html)
