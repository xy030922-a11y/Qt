# QCompleter 最小示例 / QCompleter Minimal Example

这是一个使用 Qt 6 Widgets 和 C++17 编写的 `QCompleter` 最小示例。<br>
This is a minimal `QCompleter` example written with Qt 6 Widgets and C++17.

运行程序后，输入 `ap` 或“北”即可查看自动补全效果。<br>
After launching the program, type `ap` or `北` to see auto-completion in action.

## 1. 当前目录 / Current Directory

```text
19.10.QCompleter/
├── README.md        # 项目说明 / Project documentation
├── main.cpp         # 最小示例 / Minimal example
└── CMakeLists.txt   # 构建配置 / Build configuration
```

| 文件 / File | 说明 / Description |
| --- | --- |
| `README.md` | 介绍控件、用法、示例和常见问题。<br>Introduces the widget, its usage, the example, and common issues. |
| `main.cpp` | 包含可以直接运行的最小示例代码。<br>Contains the minimal runnable example. |
| `CMakeLists.txt` | 配置并构建 Qt 6 Widgets 程序。<br>Configures and builds the Qt 6 Widgets application. |

## 2. QCompleter 是什么 / What Is QCompleter?

`QCompleter` 是 Qt 提供的自动补全类。它根据候选数据匹配用户输入，并通过弹出列表显示结果。<br>
`QCompleter` is Qt's auto-completion class. It matches user input against candidate data and displays the results in a popup.

它通常与 `QLineEdit` 或可编辑的 `QComboBox` 一起使用。候选数据可以来自字符串列表，也可以来自 Qt 的数据模型。<br>
It is commonly used with `QLineEdit` or an editable `QComboBox`. Candidates can come from a string list or a Qt item model.

## 3. QCompleter 的用法 / How to Use QCompleter

基本用法只有三步：<br>
The basic setup has only three steps:

1. 准备候选数据。<br>Prepare the candidate data.
2. 创建并配置 `QCompleter`。<br>Create and configure a `QCompleter`.
3. 使用 `QLineEdit::setCompleter()` 将补全器安装到输入框。<br>Install the completer on the input box with `QLineEdit::setCompleter()`.

```cpp
const QStringList completionStringList{
    QStringLiteral("Apple"),
    QStringLiteral("Application"),
    QStringLiteral("Apricot")
};

QCompleter *inputCompleter =
    new QCompleter(completionStringList, &inputLineEdit);
inputCompleter->setCaseSensitivity(Qt::CaseInsensitive);
inputCompleter->setFilterMode(Qt::MatchStartsWith);
inputLineEdit.setCompleter(inputCompleter);
```

常用设置：<br>
Common settings:

| API | 作用 / Purpose |
| --- | --- |
| `setCaseSensitivity(Qt::CaseInsensitive)` | 忽略英文大小写。<br>Ignores letter case. |
| `setFilterMode(Qt::MatchStartsWith)` | 匹配候选项的开头。<br>Matches the beginning of each candidate. |
| `setFilterMode(Qt::MatchContains)` | 匹配候选项中的任意位置。<br>Matches text anywhere in each candidate. |
| `setCompletionMode(QCompleter::InlineCompletion)` | 使用行内补全代替弹出列表。<br>Uses inline completion instead of a popup. |

使用 Qt Creator 时，打开 `CMakeLists.txt` 并选择带有 Qt 6 Widgets 的构建套件即可运行。<br>
In Qt Creator, open `CMakeLists.txt` and select a kit that includes Qt 6 Widgets.

也可以在已经配置好 Qt 环境的终端中构建：<br>
You can also build from a terminal where Qt is configured:

```powershell
cmake -S . -B build
cmake --build build
```

## 4. 示例程序 / Example Program

完整示例位于 [`main.cpp`](main.cpp)：<br>
The complete example is available in [`main.cpp`](main.cpp):

```cpp
#include <QApplication>
#include <QCompleter>
#include <QLineEdit>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // 创建输入框。
    // Create the input box.
    QLineEdit inputLineEdit;
    inputLineEdit.setWindowTitle(
        QStringLiteral("QCompleter 最小示例 / Minimal Example"));
    inputLineEdit.setPlaceholderText(
        QStringLiteral("输入 ap 或 北 / Type ap or 北"));
    inputLineEdit.resize(460, 40);

    // 准备候选词。
    // Prepare completion candidates.
    const QStringList completionStringList{
        QStringLiteral("Apple"),
        QStringLiteral("Application"),
        QStringLiteral("Apricot"),
        QStringLiteral("Banana"),
        QStringLiteral("北京"),
        QStringLiteral("北海"),
        QStringLiteral("上海")
    };

    // 创建补全器并安装到输入框。
    // Create the completer and install it on the input box.
    QCompleter *inputCompleter =
        new QCompleter(completionStringList, &inputLineEdit);
    inputCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    inputCompleter->setFilterMode(Qt::MatchStartsWith);
    inputLineEdit.setCompleter(inputCompleter);

    inputLineEdit.show();
    return application.exec();
}
```

运行后：<br>
After launching:

- 输入 `ap`，会显示 `Apple`、`Application` 和 `Apricot`。<br>Type `ap` to display `Apple`, `Application`, and `Apricot`.
- 输入“北”，会显示“北京”和“北海”。<br>Type `北` to display `北京` and `北海`.
- 使用方向键选择候选项，按 Enter/Return 确认。<br>Use the arrow keys to select a candidate and press Enter/Return to accept it.

## 5. 实际业务场景 / Real-World Use Cases

| 场景 / Use Case | 用法 / Usage |
| --- | --- |
| 搜索框 / Search box | 提示商品、文章或关键词。<br>Suggests products, articles, or keywords. |
| 客户选择 / Customer selection | 根据姓名、编号或拼音查找客户。<br>Finds customers by name, ID, or Pinyin. |
| 地址输入 / Address input | 补全城市、街道或常用地址。<br>Completes cities, streets, or saved addresses. |
| 文件路径 / File path | 配合 `QFileSystemModel` 补全目录和文件名。<br>Completes folders and file names with `QFileSystemModel`. |
| 历史记录 / Input history | 使用 `QStringListModel` 动态更新历史输入。<br>Updates previous inputs dynamically with `QStringListModel`. |

如果候选数据来自网络或数据库，应异步查询并更新模型，避免阻塞界面。<br>
If candidates come from a network service or database, query them asynchronously and update the model without blocking the UI.

## 6. 常见错误 / Common Mistakes

| 问题 / Problem | 原因与解决方法 / Cause and Solution |
| --- | --- |
| 没有显示候选列表<br>No suggestions appear | 检查是否调用了 `setCompleter()`，以及候选数据是否为空。<br>Check that `setCompleter()` was called and that the candidate data is not empty. |
| `ap` 无法匹配 `Apple`<br>`ap` does not match `Apple` | 默认区分大小写，请设置 `Qt::CaseInsensitive`。<br>Matching is case-sensitive by default; use `Qt::CaseInsensitive`. |
| 无法匹配中间文字<br>Text in the middle does not match | `Qt::MatchStartsWith` 只匹配开头，可改用 `Qt::MatchContains`。<br>`Qt::MatchStartsWith` only matches prefixes; use `Qt::MatchContains` instead. |
| 动态数据没有更新<br>Dynamic data does not refresh | 使用 `QStringListModel::setStringList()` 或让自定义模型正确发送变更信号。<br>Use `QStringListModel::setStringList()` or emit the correct change signals from a custom model. |
| 程序退出时崩溃<br>The program crashes on exit | 确保补全器和模型具有正确的父对象及生命周期。<br>Make sure the completer and model have correct parents and lifetimes. |
| 数据量大时卡顿<br>Large datasets are slow | 先限制候选数量，并避免在界面线程中执行耗时查询。<br>Limit the candidate count and avoid slow queries on the UI thread. |

更多信息请参考 [Qt QCompleter 官方文档](https://doc.qt.io/qt-6/qcompleter.html)。<br>
For more information, see the [official Qt QCompleter documentation](https://doc.qt.io/qt-6/qcompleter.html).
