# QCompleter 详细教程 / Detailed QCompleter Tutorial

> 适用范围：Qt 6、C++17、Qt Widgets。  
> Scope: Qt 6, C++17, and Qt Widgets.

`QCompleter` 为输入控件提供自动补全。它不是可见控件，而是一个继承自 `QObject` 的控制器：从模型中查找匹配项，再通过弹出列表或行内文本显示候选结果。  
`QCompleter` adds auto-completion to input widgets. It is not a visible widget; it is a `QObject`-based controller that finds matches in a model and presents them in a popup or inline.

官方类文档 / Official class reference: [QCompleter Class](https://doc.qt.io/qt-6/qcompleter.html)

## 1. 学习目标 / Learning Objectives

完成本章后，你应该能够：  
After this chapter, you should be able to:

1. 为 `QLineEdit` 安装一个基于字符串列表的补全器。  
   Install a string-list completer on a `QLineEdit`.
2. 配置匹配方式、大小写规则和候选显示方式。  
   Configure filtering, case sensitivity, and presentation mode.
3. 使用 `QStringListModel`、表格模型及 `QFileSystemModel` 提供数据。  
   Supply data with `QStringListModel`, table models, and `QFileSystemModel`.
4. 处理候选项的高亮与确认信号。  
   Handle candidate highlighting and activation signals.
5. 理解模型所有权、性能优化和常见错误。  
   Understand model ownership, performance optimization, and common mistakes.

## 2. QCompleter 的工作结构 / How QCompleter Works

~~~text
数据模型 / Source model
        ↓
QCompleter：设置前缀并筛选 / sets a prefix and filters
        ↓
completionModel()：只读代理模型 / read-only proxy model
        ↓
弹出列表或行内候选 / popup or inline candidate
        ↓
QLineEdit、QComboBox 或自定义控件 / target widget
~~~

| 组成 / Component | 作用 / Responsibility |
| --- | --- |
| 输入控件 / Input widget | 接收用户输入，例如 `QLineEdit`。<br>Receives user input, such as a `QLineEdit`. |
| 源模型 / Source model | 保存所有候选数据，可以是列表、表格或树模型。<br>Stores candidates; it may be a list, table, or tree model. |
| `QCompleter` | 根据当前补全前缀筛选候选，并协调键盘、弹窗和选择行为。<br>Filters candidates and coordinates keyboard, popup, and selection behavior. |
| 完成模型 / Completion model | `completionModel()` 返回的只读代理模型，只包含当前匹配项。<br>The read-only proxy returned by `completionModel()`, containing current matches. |

对 `QLineEdit` 和可编辑的 `QComboBox`，Qt 已提供直接集成。自定义输入控件通常需要调用 `setWidget()`，并自行更新补全前缀、显示弹窗和插入结果。  
Qt provides direct integration for `QLineEdit` and editable `QComboBox`. A custom editor normally needs `setWidget()` plus custom logic to update the prefix, show the popup, and insert accepted text.

## 3. 示例项目结构 / Example Project Layout

~~~text
19.10.QCompleter/
├── CMakeLists.txt   # CMake 构建配置 / CMake build configuration
├── main.cpp         # 最小可运行示例 / Minimal runnable example
└── README.md        # 本教程 / This tutorial
~~~

`main.cpp` 只创建一个 `QLineEdit`、一个候选词列表和一个 `QCompleter`，便于看清核心用法。  
`main.cpp` creates only one `QLineEdit`, one candidate list, and one `QCompleter`, keeping the essential usage easy to see.

## 4. 构建与运行 / Build and Run

### 4.1 使用 Qt Creator / Using Qt Creator

1. 在 Qt Creator 中打开本目录的 `CMakeLists.txt`。  
   Open this directory's `CMakeLists.txt` in Qt Creator.
2. 选择一个包含 Qt 6 Widgets 的构建套件。  
   Select a kit that includes Qt 6 Widgets.
3. 构建并运行 `QCompleterExample`。  
   Build and run `QCompleterExample`.

### 4.2 使用命令行 / Using the Command Line

请在已配置 Qt 环境的终端中执行：  
Run these commands in a terminal where Qt is configured:

~~~powershell
cmake -S . -B build
cmake --build build --config Debug
~~~

如果 CMake 找不到 Qt，可以传入 Qt 安装前缀：  
If CMake cannot find Qt, pass the Qt installation prefix:

~~~powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc2022_64"
cmake --build build --config Debug
~~~

使用 MinGW 时应改为对应的 `mingw_64` 目录，并确保编译器与 Qt 套件匹配。  
For MinGW, use its `mingw_64` directory and make sure the compiler matches the Qt kit.

## 5. 最小示例 / Minimal Example

核心代码如下，完整版本见 [`main.cpp`](main.cpp)：  
The essential code is shown below; see [`main.cpp`](main.cpp) for the complete version:

~~~cpp
#include <QApplication>
#include <QCompleter>
#include <QLineEdit>
#include <QStringList>

int main(int argc, char *argv[])
{
    // 创建 Widgets 应用对象。
    // Create the Widgets application object.
    QApplication app(argc, argv);

    // 创建接收输入的单行编辑框。
    // Create the single-line editor that receives input.
    QLineEdit input;

    // 准备最简单的静态候选数据。
    // Prepare the simplest static candidate data.
    const QStringList words{
        QStringLiteral("Apple（苹果）"),
        QStringLiteral("Application（应用程序）"),
        QStringLiteral("Apricot（杏）"),
        QStringLiteral("北京（Beijing）"),
        QStringLiteral("北海（Beihai）")
    };

    // input 是父对象，因此不需要手动 delete。
    // input is the parent, so no manual delete is needed.
    auto *completer = new QCompleter(words, &input);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchStartsWith);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    // 将补全器关联到输入框。
    // Attach the completer to the editor.
    input.setCompleter(completer);
    input.show();

    return app.exec();
}
~~~

运行后输入 `ap`，会出现 `Apple`、`Application` 和 `Apricot`；输入“北”，会出现“北京”和“北海”。使用方向键选择，按 Enter/Return 或单击候选项即可确认。  
After launching, type `ap` to see `Apple`, `Application`, and `Apricot`, or type `北` to see `北京` and `北海`. Use the arrow keys to choose and press Enter/Return or click an item to accept it.

## 6. 核心代码解析 / Core Code Explained

### 6.1 为什么需要 QApplication？ / Why Is QApplication Required?

`QApplication` 初始化 Qt Widgets 子系统并启动事件循环。输入、键盘导航、弹出列表和窗口重绘都依赖该事件循环。一个 Widgets 进程只能有一个应用对象。  
`QApplication` initializes Qt Widgets and starts the event loop. Input, keyboard navigation, the popup, and repainting all depend on that loop. A Widgets process has one application object.

### 6.2 候选数据从哪里来？ / Where Do Candidates Come From?

最小示例把 `QStringList` 直接传给构造函数。`QCompleter` 也可以使用任何 `QAbstractItemModel`，因此候选项可以来自内存、文件系统、数据库或自定义业务数据。  
The minimal example passes a `QStringList` directly to the constructor. `QCompleter` can consume any `QAbstractItemModel`, so candidates may come from memory, the file system, a database, or custom application data.

### 6.3 setCompleter() 做了什么？ / What Does setCompleter() Do?

`QLineEdit::setCompleter()` 把补全器安装到输入框。之后输入框会把用户输入交给补全器，并处理弹出列表及结果插入。传入 `nullptr` 可以取消自动补全。  
`QLineEdit::setCompleter()` installs the completer. The editor then forwards user input and handles the popup and insertion. Pass `nullptr` to disable completion.

### 6.4 对象为什么不会泄漏？ / Why Is There No Object Leak?

示例使用 `new QCompleter(words, &input)`，把 `input` 设为父对象。`input` 销毁时，Qt 会自动销毁补全器。不要假定 `setCompleter()` 会替你管理补全器生命周期；明确设置父对象最安全。  
The example makes `input` the parent. Qt deletes the completer when `input` is destroyed. Do not assume `setCompleter()` manages the completer's lifetime; assigning an explicit parent is safest.

## 7. 三种补全显示模式 / The Three Completion Modes

| 模式 / Mode | 行为 / Behavior | 适合场景 / Good For |
| --- | --- | --- |
| `QCompleter::PopupCompletion` | 只在弹出列表中显示匹配项。<br>Shows matching items in a popup. | 搜索框、地址框；也是默认模式。<br>Search and address fields; this is the default. |
| `QCompleter::InlineCompletion` | 把当前建议以内联选中文本显示。<br>Shows the current suggestion inline as selected text. | 快速键盘输入、候选较少的场景。<br>Fast keyboard entry with few candidates. |
| `QCompleter::UnfilteredPopupCompletion` | 弹出全部候选，并把最可能结果设为当前项。<br>Shows all candidates and selects the most likely one. | 需要始终浏览完整列表的输入框。<br>Editors that should expose the full list. |

~~~cpp
// 改为行内补全。
// Switch to inline completion.
completer->setCompletionMode(QCompleter::InlineCompletion);
~~~

## 8. 匹配规则 / Matching Rules

### 8.1 大小写规则 / Case Sensitivity

默认值是 `Qt::CaseSensitive`。搜索框通常改为忽略大小写：  
The default is `Qt::CaseSensitive`. Search fields commonly use case-insensitive matching:

~~~cpp
// 让 "ap" 可以匹配 "Apple"。
// Allow "ap" to match "Apple".
completer->setCaseSensitivity(Qt::CaseInsensitive);
~~~

### 8.2 过滤模式 / Filter Mode

| 过滤方式 / Filter | 示例：输入 `app` / Example: type `app` |
| --- | --- |
| `Qt::MatchStartsWith` | 匹配 `Apple`，不匹配 `Pineapple`；这是默认值。<br>Matches `Apple`, not `Pineapple`; this is the default. |
| `Qt::MatchContains` | 同时匹配 `Apple` 和 `Pineapple`。<br>Matches both `Apple` and `Pineapple`. |
| `Qt::MatchEndsWith` | 匹配以 `app` 结尾的候选。<br>Matches candidates ending with `app`. |

~~~cpp
// 输入候选项中间的字符也能找到结果。
// Allow text in the middle of a candidate to produce a match.
completer->setFilterMode(Qt::MatchContains);
~~~

不要在 `filterMode` 中加入 `Qt::MatchCaseSensitive`；应使用 `setCaseSensitivity()` 单独控制大小写。其他不受支持的匹配标志会产生警告且不会生效。  
Do not add `Qt::MatchCaseSensitive` to `filterMode`; use `setCaseSensitivity()` instead. Other unsupported flags produce a warning and are ignored.

## 9. 常用属性和方法 / Common Properties and Methods

| API | 默认值 / Default | 说明 / Description |
| --- | --- | --- |
| `setCaseSensitivity()` | `Qt::CaseSensitive` | 控制大小写敏感性。<br>Controls case sensitivity. |
| `setCompletionColumn()` | `0` | 指定模型中用于匹配的列。<br>Selects the model column used for matching. |
| `setCompletionRole()` | `Qt::EditRole` | 指定用于匹配的数据角色。<br>Selects the item-data role used for matching. |
| `setCompletionMode()` | `PopupCompletion` | 控制候选的展示方式。<br>Controls suggestion presentation. |
| `setCompletionPrefix()` | 空 / Empty | 手动设置当前待补全前缀。<br>Sets the current prefix manually. |
| `setFilterMode()` | `Qt::MatchStartsWith` | 控制开头、包含或结尾匹配。<br>Controls prefix, substring, or suffix matching. |
| `setMaxVisibleItems()` | `7` | 限制弹窗中同时可见的候选数量。<br>Limits visible popup items. |
| `setWrapAround()` | `true` | 控制导航到末尾后是否回到开头。<br>Controls navigation wrapping. |
| `setModelSorting()` | `UnsortedModel` | 告诉补全器源模型是否已排序。<br>Tells the completer whether the model is sorted. |
| `setPopup()` | 默认列表视图 / Default list view | 替换候选项视图。<br>Replaces the suggestion view. |
| `complete()` | — | 手动显示弹窗或触发行内候选。<br>Shows the popup or triggers inline completion. |
| `currentCompletion()` | — | 返回当前候选文本。<br>Returns the current completion text. |
| `completionModel()` | — | 返回当前匹配项的只读代理模型。<br>Returns the read-only proxy of current matches. |

## 10. 信号：高亮与确认 / Signals: Highlighting and Activation

- `highlighted(...)`：用户在候选列表中移动当前项时发出。  
  `highlighted(...)`: emitted when the current candidate changes.
- `activated(...)`：用户单击候选项或按 Enter/Return 确认时发出。  
  `activated(...)`: emitted when a user clicks a candidate or accepts it with Enter/Return.

两种信号都有 `QString` 和 `QModelIndex` 重载。连接 Lambda 时应明确选择重载：  
Both signals have `QString` and `QModelIndex` overloads. Select the overload explicitly for a lambda:

~~~cpp
#include <QDebug>

// 接收最终确认的候选文本。
// Receive the candidate text accepted by the user.
QObject::connect(
    completer,
    QOverload<const QString &>::of(&QCompleter::activated),
    &input,
    [](const QString &text) {
        // 实际项目可在这里发起搜索或加载数据。
        // A real app could search or load data here.
        qDebug() << "Activated:" << text;
    });
~~~

若需访问多个源字段，`QModelIndex` 重载更合适。该索引属于 `completionModel()`；访问源模型时可能需要进行代理索引映射。  
The `QModelIndex` overload is better when several source fields are needed. Its index belongs to `completionModel()`; source-model access may require proxy-index mapping.

## 11. 动态 QStringListModel / Dynamic QStringListModel

固定数据可直接传入 `QStringList`。运行时更新候选时，显式创建 `QStringListModel`：  
Pass a `QStringList` for fixed data. For runtime updates, create a `QStringListModel` explicitly:

~~~cpp
#include <QStringListModel>

// 让 input 管理 completer，让 completer 管理 model。
// Let input own completer and let completer own model.
auto *completer = new QCompleter(&input);
auto *model = new QStringListModel(completer);

model->setStringList({QStringLiteral("Alpha"), QStringLiteral("Beta")});
completer->setModel(model);
input.setCompleter(completer);

// 稍后替换候选；补全器会响应模型的变更信号。
// Replace candidates later; the completer reacts to model changes.
model->setStringList({
    QStringLiteral("Gamma"),
    QStringLiteral("Garden"),
    QStringLiteral("Galaxy")
});
~~~

频繁更新时应修改现有模型，而不是反复重建 `QCompleter`。模型正确发出行插入、删除、数据改变或重置信号后，补全器会刷新。  
For frequent updates, modify the existing model instead of recreating `QCompleter`. It refreshes when the model emits proper row, data-change, or reset signals.

## 12. 多列模型与自定义角色 / Multi-Column Models and Custom Roles

### 12.1 选择匹配列 / Selecting the Completion Column

~~~cpp
#include <QStandardItemModel>

auto *model = new QStandardItemModel(completer);
model->setHorizontalHeaderLabels({
    QStringLiteral("名称 / Name"),
    QStringLiteral("代码 / Code")
});
model->appendRow({
    new QStandardItem(QStringLiteral("中国 / China")),
    new QStandardItem(QStringLiteral("CN"))
});
model->appendRow({
    new QStandardItem(QStringLiteral("加拿大 / Canada")),
    new QStandardItem(QStringLiteral("CA"))
});

completer->setModel(model);

// 在第 1 列中匹配 CN、CA，而不是第 0 列的国家名称。
// Match CN and CA in column 1 instead of country names in column 0.
completer->setCompletionColumn(1);
~~~

默认弹出视图是列表时，它显示所选的补全列。若要同时显示多列，可设置一个 `QTableView` 作为弹出视图。  
The default list popup displays the selected completion column. To show multiple columns, install a `QTableView` as the popup.

### 12.2 选择匹配角色 / Selecting the Completion Role

默认使用 `Qt::EditRole`。若显示文本与搜索关键词不同，可把关键词放入自定义角色：  
The default is `Qt::EditRole`. If display text and search keywords differ, store keywords in a custom role:

~~~cpp
constexpr int SearchRole = Qt::UserRole + 1;

auto *model = new QStandardItemModel(completer);
auto *item = new QStandardItem(QStringLiteral("苹果 / Apple"));

// 按隐藏关键词匹配，界面仍显示双语名称。
// Match hidden keywords while displaying a bilingual label.
item->setData(QStringLiteral("apple fruit pingguo"), SearchRole);
model->appendRow(item);

completer->setModel(model);
completer->setCompletionRole(SearchRole);
completer->setFilterMode(Qt::MatchContains);
~~~

`completionRole` 决定“用什么数据匹配”，最终插入文本还与编辑角色及 `pathFromIndex()` 有关。复杂业务可继承 `QCompleter` 并重写 `pathFromIndex()`。  
`completionRole` decides what data is matched, while inserted text also depends on the edit role and `pathFromIndex()`. Advanced applications can subclass `QCompleter` and override `pathFromIndex()`.

## 13. 文件路径自动补全 / File-Path Completion

`QFileSystemModel` 是树模型，`QCompleter` 对它提供专门支持，可逐级补全目录和文件名：  
`QFileSystemModel` is a tree model. `QCompleter` has dedicated support for completing directory and file names one path segment at a time:

~~~cpp
#include <QDir>
#include <QFileSystemModel>

auto *fileCompleter = new QCompleter(&input);
auto *fileModel = new QFileSystemModel(fileCompleter);

// 启动文件系统模型；其加载工作在后台进行。
// Start the file-system model; loading happens asynchronously.
fileModel->setRootPath(QDir::rootPath());

fileCompleter->setModel(fileModel);
fileCompleter->setCompletionMode(QCompleter::PopupCompletion);
input.setCompleter(fileCompleter);
~~~

`QFileSystemModel` 作为源模型时，Qt 会根据平台设置大小写策略：Windows 上不区分大小写，其他平台区分大小写。路径分段由 `splitPath()` 处理，候选索引到完整路径的转换由 `pathFromIndex()` 处理。  
With `QFileSystemModel`, Qt sets case sensitivity by platform: case-insensitive on Windows and case-sensitive elsewhere. `splitPath()` handles path components, while `pathFromIndex()` turns a candidate index into a complete path.

## 14. 自定义树模型补全 / Completion with Custom Tree Models

列表模型只需匹配一个字符串，树模型还必须解决两个问题：  
A list model matches one string; a tree model must answer two additional questions:

1. 怎样把输入拆成逐层匹配的路径？  
   How should input be split into per-level path components?
2. 怎样把树节点重新组合为最终文本？  
   How should a tree node be converted back into final text?

因此自定义树模型通常需要继承 `QCompleter` 并重写：  
A custom tree-model completer normally subclasses `QCompleter` and overrides:

~~~cpp
class TreeCompleter : public QCompleter
{
public:
    using QCompleter::QCompleter;

protected:
    // 把 "Asia/China/Bei" 拆成逐层匹配的片段。
    // Split "Asia/China/Bei" into per-level matching components.
    QStringList splitPath(const QString &path) const override;

    // 把模型索引还原为完整路径。
    // Convert a model index back into a complete path.
    QString pathFromIndex(const QModelIndex &index) const override;
};
~~~

文件系统是这一机制的典型例子。可继续阅读 Qt 官方的 [Tree Model Completer Example](https://doc.qt.io/qt-6/qtwidgets-tools-treemodelcompleter-example.html)。  
The file system is a typical use of this mechanism. Continue with Qt's official [Tree Model Completer Example](https://doc.qt.io/qt-6/qtwidgets-tools-treemodelcompleter-example.html).

## 15. 手动设置前缀与遍历结果 / Setting a Prefix and Iterating Manually

在自定义编辑器或非界面逻辑中，可以手动查询补全结果：  
In a custom editor or non-visual workflow, completions can be queried manually:

~~~cpp
// 设置查询前缀，这会自动刷新 completionModel()。
// Set the query prefix; this refreshes completionModel() automatically.
completer->setCompletionPrefix(QStringLiteral("ap"));

// setCurrentRow() 返回 false 时表示没有更多候选。
// setCurrentRow() returns false when there are no more candidates.
for (int row = 0; completer->setCurrentRow(row); ++row) {
    qDebug() << completer->currentCompletion();
}
~~~

尽量不要为了遍历而先调用 `completionCount()`。在未排序的大模型中，它可能扫描整个模型；直接使用 `setCurrentRow()` 通常更合适。  
Avoid calling `completionCount()` merely to iterate. It may scan a large unsorted model; using `setCurrentRow()` directly is generally preferable.

## 16. 手动显示和调整弹窗 / Showing and Adjusting the Popup

`complete()` 可以手动显示弹窗。无参数时，弹窗默认出现在目标控件下方；传入矩形可控制其相对位置与宽度。  
`complete()` shows the popup manually. Without arguments, it appears below the target widget; a rectangle can control its relative position and width.

~~~cpp
// 让弹窗宽度与输入框一致。
// Make the popup as wide as the editor.
QRect popupRect = input.rect();
popupRect.setWidth(input.width());
completer->complete(popupRect);
~~~

也可以访问默认视图进行轻量定制：  
The default view can also be adjusted:

~~~cpp
#include <QAbstractItemView>

// 限制最大可见行数。
// Limit the maximum number of visible rows.
completer->setMaxVisibleItems(10);

// 关闭从最后一项回到第一项的循环导航。
// Disable navigation wrapping from the last item to the first.
completer->setWrapAround(false);

// 设置现有弹出视图的最小宽度。
// Set the minimum width of the existing popup view.
completer->popup()->setMinimumWidth(360);
~~~

调用 `popup()` 返回对象的成员时，源码需要包含 `<QAbstractItemView>`，仅有前向声明不足以调用成员函数。  
When calling methods on the object returned by `popup()`, include `<QAbstractItemView>`; a forward declaration alone is insufficient.

## 17. 大模型性能优化 / Performance with Large Models

默认情况下，补全器不假定模型已排序，通常进行线性查找。若源模型确实按补全列和补全角色排列，可以提供排序提示：  
By default, the completer makes no sorting assumption and usually performs a linear search. If the source model is truly sorted by the completion column and role, provide a sorting hint:

~~~cpp
// 前提：模型已按相同的大小写规则排序。
// Requirement: the model is sorted with the same case-sensitivity rule.
completer->setCaseSensitivity(Qt::CaseInsensitive);
completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
~~~

`QCompleter` 只有在以下条件同时满足时才能利用二分查找：  
`QCompleter` can use binary search only when all of these are true:

1. `filterMode` 是 `Qt::MatchStartsWith`。  
   `filterMode` is `Qt::MatchStartsWith`.
2. 模型确实按补全列与补全角色排序。  
   The model is actually sorted by the completion column and role.
3. 模型排序的大小写规则与补全器一致。  
   Model sorting uses the same case-sensitivity rule as the completer.

不要错误声明模型已排序，否则候选结果可能不正确。`MatchContains` 与 `MatchEndsWith` 不能利用这项二分查找优化。  
Do not falsely claim that a model is sorted, because results may be incorrect. `MatchContains` and `MatchEndsWith` cannot use this binary-search optimization.

对于网络搜索建议，不应在每次按键时立即发出请求。通常需要短暂防抖、取消过期请求，并把最新结果写入长期存在的模型。  
For network-backed suggestions, avoid issuing a request on every keystroke. Add a short debounce, cancel stale requests, and write the latest results into a long-lived model.

## 18. 与 QValidator 配合 / Using QCompleter with QValidator

当 `QLineEdit` 同时安装 `QValidator` 或输入掩码时，候选数据也必须满足输入限制，否则候选项可能无法插入。可先清洗模型，或使用 `QSortFilterProxyModel` 只暴露有效候选。  
When `QLineEdit` also uses a `QValidator` or input mask, candidates must satisfy those constraints or may not be insertable. Sanitize the model or use `QSortFilterProxyModel` to expose valid candidates only.

~~~cpp
// 验证器与补全器应认可同一组最终输入。
// The validator and completer should agree on valid final input.
input.setValidator(validator);
input.setCompleter(completer);
~~~

## 19. 在 QComboBox 中使用 / Using QCompleter with QComboBox

组合框必须可编辑，用户才能输入补全前缀：  
The combo box must be editable so the user can type a prefix:

~~~cpp
#include <QComboBox>

auto *comboBox = new QComboBox;
comboBox->setEditable(true);

// 为组合框安装补全器。
// Install a completer on the combo box.
comboBox->setCompleter(completer);
~~~

若补全器与组合框共享一个模型，应仔细设置 `completionColumn` 和组合框的 `modelColumn`，保证显示与匹配来自预期列。  
If the completer and combo box share a model, configure `completionColumn` and the combo box's `modelColumn` carefully so display and matching use the intended columns.

## 20. 在 QTextEdit 等自定义控件中使用 / Using QCompleter with QTextEdit and Custom Editors

`QTextEdit` 没有 `setCompleter()`。实现单词级补全通常需要：  
`QTextEdit` has no `setCompleter()`. Word-level completion normally requires:

1. 继承 `QTextEdit` 并重写 `keyPressEvent()`。  
   Subclass `QTextEdit` and override `keyPressEvent()`.
2. 用 `QTextCursor` 取得光标下的当前单词。  
   Use `QTextCursor` to obtain the word under the cursor.
3. 调用 `completer->setWidget(this)` 指定目标控件。  
   Call `completer->setWidget(this)` to assign the target editor.
4. 调用 `setCompletionPrefix()` 和 `complete()` 更新候选。  
   Call `setCompletionPrefix()` and `complete()` to update suggestions.
5. 连接 `activated(QString)`，只插入尚未输入的后缀。  
   Connect `activated(QString)` and insert only the missing suffix.

这部分涉及光标、事件和焦点管理。掌握最小示例后，可参考 Qt 官方 [Custom Completer Example](https://doc.qt.io/qt-6/qtwidgets-tools-customcompleter-example.html)。  
This requires cursor, event, and focus management. After mastering the minimal example, consult Qt's official [Custom Completer Example](https://doc.qt.io/qt-6/qtwidgets-tools-customcompleter-example.html).

## 21. 生命周期与所有权 / Lifetime and Ownership

### 21.1 QCompleter 的所有权 / QCompleter Ownership

建议始终为补全器设置明确父对象：  
Always give the completer an explicit parent:

~~~cpp
// lineEdit 销毁时 completer 自动销毁。
// completer is deleted automatically with lineEdit.
auto *completer = new QCompleter(&lineEdit);
lineEdit.setCompleter(completer);
~~~

### 21.2 模型的所有权 / Model Ownership

`setModel()` 接收指针，但不要把它理解为无条件接管模型。若希望模型与补全器一起销毁，应把补全器设为模型的父对象：  
`setModel()` accepts a pointer, but do not treat that as unconditional ownership transfer. To destroy the model with the completer, parent it to the completer:

~~~cpp
auto *model = new QStringListModel(completer);
completer->setModel(model);
~~~

替换模型时，若旧模型的父对象是补全器，Qt 会删除旧模型。因此不要继续保存或访问那个旧模型指针。  
When replacing a model, Qt deletes the old model if the completer is its parent. Do not retain or access that old pointer.

### 21.3 共享补全器 / Sharing a Completer

一个 `QCompleter` 同一时刻只关联一个目标控件，因为 `widget()` 只返回一个指针。多个输入框需要同时补全时，通常为每个输入框创建独立补全器，但可以共享同一个只读源模型。  
A `QCompleter` targets one widget at a time because `widget()` returns a single pointer. For several editors, normally create one completer per editor while sharing a read-only source model when appropriate.

## 22. 常见错误 / Common Mistakes

| 问题 / Problem | 原因 / Cause | 解决方法 / Fix |
| --- | --- | --- |
| 输入后没有候选 / No suggestions | 默认区分大小写，或候选不以前缀开头。<br>Matching is case-sensitive by default, or candidates do not start with the prefix. | 设置 `CaseInsensitive`，或按需求使用 `MatchContains`。<br>Use `CaseInsensitive` or `MatchContains` as needed. |
| 中文能搜，拼音不能搜 / Chinese works but Pinyin does not | `QCompleter` 不会自动生成拼音或同义词。<br>`QCompleter` does not generate Pinyin or synonyms. | 在自定义角色保存拼音关键词。<br>Store Pinyin keywords in a custom role. |
| `MatchCaseSensitive` 无效 / `MatchCaseSensitive` has no effect | 它不是这里受支持的过滤方式。<br>It is not a supported filter mode here. | 使用 `setCaseSensitivity()`。<br>Use `setCaseSensitivity()`. |
| 数据更新后不刷新 / Changes do not refresh | 自定义模型未发出正确通知。<br>The custom model emitted no proper notifications. | 使用模型 API 或正确的 begin/end/reset 通知。<br>Use model APIs or proper begin/end/reset notifications. |
| 大数据量卡顿 / Large data set is slow | 包含匹配线性扫描，或频繁调用 `completionCount()`。<br>Substring matching scans linearly, or `completionCount()` is repeatedly called. | 用前缀匹配、有效排序提示、延迟加载或服务端搜索。<br>Use prefix matching, valid sorting hints, lazy loading, or server-side search. |
| `popup()->...` 编译失败 / `popup()` call fails | 缺少完整的 `QAbstractItemView` 类型定义。<br>The complete `QAbstractItemView` definition is missing. | 添加 `#include <QAbstractItemView>`。<br>Add `#include <QAbstractItemView>`. |
| 退出时崩溃 / Crash at shutdown | 所有权混乱造成重复销毁或悬空指针。<br>Confused ownership causes double deletion or dangling pointers. | 使用清晰的 QObject 父子关系。<br>Use clear QObject parenting. |
| QComboBox 不能输入 / QComboBox cannot accept typing | 组合框不是可编辑状态。<br>The combo box is not editable. | 调用 `setEditable(true)`。<br>Call `setEditable(true)`. |

## 23. API 速查 / API Quick Reference

### 23.1 构造函数 / Constructors

~~~cpp
// 空补全器，稍后调用 setModel()。
// Empty completer; call setModel() later.
QCompleter(QObject *parent = nullptr);

// 使用现有项模型。
// Use an existing item model.
QCompleter(QAbstractItemModel *model, QObject *parent = nullptr);

// 使用静态字符串列表。
// Use a static string list.
QCompleter(const QStringList &list, QObject *parent = nullptr);
~~~

### 23.2 关键读取接口 / Key Read APIs

~~~cpp
QAbstractItemModel *model() const;           // 源模型 / Source model
QAbstractItemModel *completionModel() const; // 当前匹配代理 / Current-match proxy
QString completionPrefix() const;            // 当前前缀 / Current prefix
QString currentCompletion() const;           // 当前候选文本 / Current candidate
QModelIndex currentIndex() const;             // 当前候选索引 / Current index
QAbstractItemView *popup() const;             // 弹出视图 / Popup view
QWidget *widget() const;                      // 目标控件 / Target widget
~~~

### 23.3 关键写入接口 / Key Write APIs

~~~cpp
void setModel(QAbstractItemModel *model);
void setWidget(QWidget *widget);
void setCompletionPrefix(const QString &prefix);
void setCompletionColumn(int column);
void setCompletionRole(int role);
void setCompletionMode(QCompleter::CompletionMode mode);
void setCaseSensitivity(Qt::CaseSensitivity sensitivity);
void setFilterMode(Qt::MatchFlags mode);
void setModelSorting(QCompleter::ModelSorting sorting);
void setMaxVisibleItems(int count);
void setWrapAround(bool enabled);
void complete(const QRect &rect = QRect());
~~~

## 24. 练习建议 / Suggested Exercises

1. 把最小示例改成 `InlineCompletion`，比较操作体验。  
   Change the minimal example to `InlineCompletion` and compare the interaction.
2. 改成 `MatchContains`，输入 `jing` 搜索双语候选。  
   Switch to `MatchContains` and type `jing` to search bilingual entries.
3. 使用 `QStringListModel` 和一个按钮，在运行时加入候选。  
   Use `QStringListModel` and a button to insert candidates at runtime.
4. 连接 `activated(QString)`，在状态标签中显示最终选择。  
   Connect `activated(QString)` and show the accepted value in a status label.
5. 使用 `QFileSystemModel` 制作文件路径输入框。  
   Build a file-path editor with `QFileSystemModel`.
6. 建立“显示名称 + 搜索拼音 + 唯一 ID”的模型，分别使用显示角色、自定义角色和业务角色。  
   Build a model with display name, searchable Pinyin, and unique ID using separate roles.

## 25. 本章结论 / Chapter Summary

`QCompleter` 的核心不是字符串列表本身，而是“输入控件 + 模型 + 匹配规则 + 候选视图”的组合。简单场景只需 `QStringList` 和几行配置；复杂场景通过模型列、数据角色、代理模型以及 `splitPath()`、`pathFromIndex()` 扩展，不必重写整套补全逻辑。  
The core of `QCompleter` is not the string list itself, but the combination of input widget, model, matching rules, and suggestion view. A simple case needs a `QStringList` and a few settings; advanced cases extend model columns, roles, proxy models, `splitPath()`, and `pathFromIndex()` without rebuilding the whole completion system.

## 26. 官方参考 / Official References

- [QCompleter Class](https://doc.qt.io/qt-6/qcompleter.html) — 属性、枚举、信号和成员函数 / Properties, enums, signals, and functions
- [QLineEdit Class](https://doc.qt.io/qt-6/qlineedit.html) — `setCompleter()` 与验证器配合 / `setCompleter()` and validator integration
- [Completer Example](https://doc.qt.io/qt-6/qtwidgets-tools-completer-example.html) — 多模型和多模式演示 / Multiple models and modes
- [Custom Completer Example](https://doc.qt.io/qt-6/qtwidgets-tools-customcompleter-example.html) — 在 `QTextEdit` 中补全 / Completion in `QTextEdit`
- [Tree Model Completer Example](https://doc.qt.io/qt-6/qtwidgets-tools-treemodelcompleter-example.html) — 树模型路径补全 / Tree-model path completion
