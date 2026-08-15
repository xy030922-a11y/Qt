# QCheckBox 详解 / In-Depth QCheckBox Guide

本章使用 **Qt 6 + C++17 + CMake**，讲解 `QCheckBox` 的二态/三态模型、常用 API，以及每一种复选框相关信号的触发条件和适用场景。

*This chapter uses **Qt 6 + C++17 + CMake**. It explains the two-state/tristate model, common APIs, and the trigger conditions and use cases of every check-box-related signal.*

本目录包含以下文件：

*This directory contains the following files:*

```text
18.4.QCheckBox/
├── CMakeLists.txt   # Qt 6 Widgets 工程配置 / Qt 6 Widgets project configuration
├── main.cpp         # 双语信号观察示例 / Bilingual signal-inspector example
└── README.md        # 双语教程 / Bilingual tutorial
```

示例兼容 Qt 6.5 及以上版本。本机 Qt 6.5.3 会使用 `stateChanged(int)`；Qt 6.7 及以上版本会自动使用新的强类型信号 `checkStateChanged(Qt::CheckState)`。

*The example supports Qt 6.5 and later. The local Qt 6.5.3 installation uses `stateChanged(int)`, while Qt 6.7 and later automatically use the strongly typed `checkStateChanged(Qt::CheckState)` signal.*

## 1. QCheckBox 是什么 / What Is QCheckBox?

`QCheckBox` 是 Qt Widgets 模块中的复选框控件，通常表示某个独立选项是否启用。启用三态后，它还可以表达“部分子项已选中”“状态混合”或“保持原值”。

*`QCheckBox` is a check-box widget from the Qt Widgets module. It normally represents whether an independent option is enabled. In tristate mode, it can also express “some child items are selected,” “mixed state,” or “leave unchanged.”*

它的继承关系如下：

*Its inheritance hierarchy is:*

```text
QObject
└── QWidget
    └── QAbstractButton
        └── QCheckBox
```

这意味着它同时拥有两类能力：

*This gives it two groups of capabilities:*

- `QCheckBox` 自己提供精确的二态/三态状态接口。  
  *`QCheckBox` itself provides exact two-state/tristate APIs.*
- 它从 `QAbstractButton` 继承按下、释放、点击和布尔选中状态信号。  
  *It inherits press, release, click, and boolean checked-state signals from `QAbstractButton`.*

头文件和 CMake 依赖：

*Header and CMake dependency:*

```cpp
#include <QCheckBox>
```

```cmake
find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(your_target PRIVATE Qt6::Widgets)
```

最基本的创建方式：

*Minimal construction:*

```cpp
auto *checkBox = new QCheckBox(
    QStringLiteral("启用通知 / Enable notifications"), parentWidget);
checkBox->setChecked(true);
```

传入父对象后，Qt 的父子对象机制会在父窗口销毁时自动销毁复选框，通常不需要手动 `delete`。

*With a parent object, Qt automatically destroys the check box when its parent window is destroyed, so a manual `delete` is normally unnecessary.*

## 2. 二态与三态 / Two-State and Tristate Modes

### 2.1 二态模式 / Two-State Mode

默认情况下，`QCheckBox` 只有未选中和已选中两个状态。

*By default, `QCheckBox` has only unchecked and checked states.*

| 枚举值 / Enum | 数值 / Value | 含义 / Meaning | `isChecked()` |
|---|---:|---|---|
| `Qt::Unchecked` | 0 | 未选中 / Unchecked | `false` |
| `Qt::Checked` | 2 | 已选中 / Checked | `true` |

二态场景可以直接使用布尔接口：

*For a two-state check box, use the boolean API directly:*

```cpp
checkBox->setChecked(true);

if (checkBox->isChecked()) {
    // 当前已选中。
    // It is currently checked.
}
```

### 2.2 三态模式 / Tristate Mode

调用 `setTristate(true)` 后，会增加 `Qt::PartiallyChecked` 状态：

*Calling `setTristate(true)` adds the `Qt::PartiallyChecked` state:*

```cpp
checkBox->setTristate(true);
checkBox->setCheckState(Qt::PartiallyChecked);
```

| 枚举值 / Enum | 数值 / Value | 常见视觉 / Typical Visual | 典型含义 / Typical Meaning | `isChecked()` |
|---|---:|---|---|---|
| `Qt::Unchecked` | 0 | 空框 / Empty box | 全部关闭 / All off | `false` |
| `Qt::PartiallyChecked` | 1 | 方块或横线 / Square or dash | 部分开启、状态混合 / Partly on or mixed | `true` |
| `Qt::Checked` | 2 | 对勾 / Check mark | 全部开启 / All on | `true` |

最容易踩坑的一点是：**部分选中时 `isChecked()` 也返回 `true`**。布尔值不能区分“部分选中”和“完全选中”。需要区分三态时，必须读取 `checkState()`。

*The most important pitfall is that **`isChecked()` also returns `true` for `PartiallyChecked`**. A boolean cannot distinguish partial from full selection. Read `checkState()` whenever the exact state matters.*

```cpp
switch (checkBox->checkState()) {
case Qt::Unchecked:
    // 未选中 / Unchecked
    break;
case Qt::PartiallyChecked:
    // 部分选中 / Partially checked
    break;
case Qt::Checked:
    // 完全选中 / Fully checked
    break;
}
```

三态复选框默认按以下顺序循环：

*A tristate check box cycles in this default order:*

```text
Unchecked → PartiallyChecked → Checked → Unchecked
```

“部分选中”经常是根据多个子项计算出的汇总状态，不一定应该允许用户手动选择。若业务只允许用户选择全开或全关，可以仍用三态显示汇总结果，但在槽函数中按业务规则处理点击。

*A partial state is often an aggregate calculated from child items rather than a state users should select directly. If users may choose only all-on or all-off, tristate can still display the aggregate while click handling enforces the business rule.*

## 3. 常用属性和函数 / Common Properties and Functions

### 3.1 文本与快捷键 / Text and Mnemonics

```cpp
checkBox->setText(QStringLiteral("启用通知 / Enable notifications"));
QString text = checkBox->text();
```

文本中的 `&` 会把后一个字符设为键盘助记键。例如下面通常可通过 `Alt+E` 或平台对应操作激活：

*An `&` in the text makes the following character a keyboard mnemonic. The following can normally be activated with `Alt+E` or the platform equivalent:*

```cpp
auto *checkBox = new QCheckBox(
    QStringLiteral("启用通知(&E) / &Enable notifications"));
```

### 3.2 布尔选中状态 / Boolean Checked State

```cpp
checkBox->setChecked(true);
bool checked = checkBox->isChecked();
checkBox->toggle();
```

- `setChecked(bool)`：明确设置为选中或未选中。  
  *Explicitly set checked or unchecked.*
- `isChecked()`：读取布尔 `checked` 属性。  
  *Read the boolean `checked` property.*
- `toggle()`：翻转布尔状态，但不模拟一次鼠标点击。  
  *Invert the boolean state without simulating a mouse click.*

### 3.3 精确三态 / Exact Tristate State

```cpp
checkBox->setTristate(true);
checkBox->setCheckState(Qt::PartiallyChecked);
Qt::CheckState state = checkBox->checkState();
```

- `setTristate(bool)`：启用或关闭三态。  
  *Enable or disable tristate mode.*
- `isTristate()`：查询是否允许三态。  
  *Report whether tristate mode is enabled.*
- `setCheckState(Qt::CheckState)`：精确设置三个状态之一。  
  *Set one of the three exact states.*
- `checkState()`：读取精确状态。  
  *Read the exact state.*

### 3.4 可用状态与交互 / Enabled State and Interaction

```cpp
checkBox->setEnabled(false);  // 禁止用户操作 / Disable user interaction
checkBox->setVisible(true);   // 显示控件 / Show the widget
checkBox->setCheckable(true); // QCheckBox 默认可切换 / Checkable by default for QCheckBox
```

程序仍可以对禁用的复选框调用 `setChecked()` 或 `setCheckState()`。`setEnabled(false)` 限制的是用户交互，并不会把对象的数据变成只读。

*A program can still call `setChecked()` or `setCheckState()` on a disabled check box. `setEnabled(false)` blocks user interaction; it does not make the object’s data read-only.*

### 3.5 图标 / Icons

`QCheckBox` 继承了按钮的图标接口：

*`QCheckBox` inherits the button icon API:*

```cpp
checkBox->setIcon(QIcon(QStringLiteral(":/icons/cloud.svg")));
checkBox->setIconSize(QSize(20, 20));
```

实际项目建议使用 Qt Resource System（`.qrc`）保存图标，以避免依赖运行目录中的相对路径。

*In production projects, keep icons in the Qt Resource System (`.qrc`) to avoid fragile paths relative to the working directory.*

## 4. 信号总览 / Signal Overview

与复选框交互和选中状态直接相关的信号分为两类：`QCheckBox` 自己的精确状态信号，以及从 `QAbstractButton` 继承的四个按钮信号。

*Signals directly related to interaction and checked state fall into two groups: the exact-state signals declared by `QCheckBox`, and four button signals inherited from `QAbstractButton`.*

| 信号 / Signal | 来源 / Declared By | 关注内容 / What It Describes | 区分三态 / Distinguishes 3 States |
|---|---|---|:---:|
| `pressed()` | `QAbstractButton` | 进入按下状态 / Entering pressed state | 否 / No |
| `released()` | `QAbstractButton` | 离开按下状态 / Leaving pressed state | 否 / No |
| `clicked(bool checked)` | `QAbstractButton` | 完成一次有效激活 / A valid activation completes | 否 / No |
| `toggled(bool checked)` | `QAbstractButton` | 布尔 `checked` 属性改变 / Boolean property changes | 否 / No |
| `checkStateChanged(Qt::CheckState)` | `QCheckBox`，Qt 6.7+ | 精确状态改变 / Exact state changes | 是 / Yes |
| `stateChanged(int)` | `QCheckBox`，旧接口 / Legacy | 精确状态改变 / Exact state changes | 是 / Yes, after conversion |

`QCheckBox` 还继承了 `QObject` 和 `QWidget` 的通用信号，例如 `destroyed()`、`objectNameChanged()`、`windowTitleChanged()` 和 `customContextMenuRequested()`。这些信号不描述复选框的点击或选中状态，因此本章重点讲解上表六个信号。

*`QCheckBox` also inherits general-purpose signals from `QObject` and `QWidget`, including `destroyed()`, `objectNameChanged()`, `windowTitleChanged()`, and `customContextMenuRequested()`. They do not describe check-box activation or checked state, so this chapter focuses on the six signals above.*

## 5. 每种信号的区别 / Differences Between the Signals

### 5.1 用户完成一次操作时的信号顺序 / Signal Order After User Activation

对于默认的 `QCheckBox`，在同一线程使用直接连接，并且槽函数没有再次修改复选框状态时，一次有效的鼠标点击、键盘或助记键激活通常按以下顺序发出信号：

*For a default `QCheckBox` using direct connections in the same thread, provided that the connected slots do not modify the check box again, a valid mouse click, keyboard activation, or mnemonic normally emits signals in this order:*

```text
pressed()
    ↓
toggled(bool)                         仅当布尔 checked 值改变时
    ↓                                 Only if the boolean checked value changes
精确状态信号 / Exact-state signal      仅当 Qt::CheckState 改变时
    ↓                                 Only if Qt::CheckState changes
released()
    ↓
clicked(bool)
```

其中“精确状态信号”在 Qt 6.7 及以上版本是 `checkStateChanged(Qt::CheckState)`，在 Qt 6.5/6.6 中是 `stateChanged(int)`。如果在 Qt 6.7 及以上版本同时连接新旧两个接口，`checkStateChanged()` 会先于 `stateChanged()` 发出。

*The exact-state signal is `checkStateChanged(Qt::CheckState)` in Qt 6.7 and later, and `stateChanged(int)` in Qt 6.5/6.6. If both the new and legacy signals are connected in Qt 6.7 or later, `checkStateChanged()` is emitted before `stateChanged()`.*

常见操作的实际顺序如下：

*Typical operations produce the following sequences:*

| 操作 / Action | 信号顺序 / Signal Order |
|---|---|
| 二态：未选中 → 选中 / Two-state: unchecked → checked | `pressed → toggled(true) → exact-state(Checked) → released → clicked(true)` |
| 三态：未选中 → 部分选中 / Tristate: unchecked → partially checked | `pressed → toggled(true) → exact-state(PartiallyChecked) → released → clicked(true)` |
| 三态：部分选中 → 完全选中 / Tristate: partially checked → checked | `pressed → exact-state(Checked) → released → clicked(true)` |
| 三态：完全选中 → 未选中 / Tristate: checked → unchecked | `pressed → toggled(false) → exact-state(Unchecked) → released → clicked(false)` |
| 鼠标按下后移出控件并释放 / Press, drag outside, and release | `pressed → released`，状态不变且没有 `clicked` / State unchanged; no `clicked` |

三态从 `PartiallyChecked` 变为 `Checked` 时没有 `toggled(true)`，因为这两个精确状态对应的布尔 `checked` 值都是 `true`。一次成功激活中，状态变化信号发生在 `released()` 和最终的 `clicked()` 之前。

*There is no `toggled(true)` when a tristate check box changes from `PartiallyChecked` to `Checked`, because both exact states map to the boolean value `true`. During a successful activation, state-change signals occur before `released()` and the final `clicked()`.*

### 5.2 `pressed()`：开始按下 / Press Begins

```cpp
connect(checkBox, &QCheckBox::pressed, this, [] {
    qDebug() << "按下 / Pressed";
});
```

它表示按钮进入“按下”阶段。此时有效点击尚未完成，选中状态通常也还没有切换。

*It means the button has entered its pressed phase. A valid click has not completed yet, and the checked state has normally not changed yet.*

适合做按下时的视觉/声音反馈、记录长按起点或观察底层交互。它不适合保存最终选中状态。

*Use it for immediate visual/audio feedback, recording the start of a long press, or inspecting low-level interaction. Do not use it to save the final checked state.*

### 5.3 `released()`：结束按下 / Press Ends

```cpp
connect(checkBox, &QCheckBox::released, this, [] {
    qDebug() << "释放 / Released";
});
```

它表示按钮离开按下状态。`released()` 不等于“业务点击成功”：例如按下后把鼠标拖出控件，按下状态可能结束，却不会形成有效的 `clicked()`。

*It means the button has left its pressed state. `released()` does not guarantee a successful business click. For example, dragging the pointer outside after pressing can end the pressed state without producing a valid `clicked()`.*

若目标是处理一次完成的激活，通常应该选择 `clicked()`。

*Use `clicked()` when the goal is to process a completed activation.*

### 5.4 `clicked(bool)`：完成有效激活 / Valid Activation Completes

```cpp
connect(checkBox, &QCheckBox::clicked, this, [](bool checked) {
    qDebug() << "激活后的布尔状态 / Boolean state after activation:" << checked;
});
```

以下操作会触发它：

*It is emitted by:*

- 用户完成一次有效鼠标点击。 / *A valid mouse click.*
- 用户通过键盘或助记键激活。 / *Keyboard or mnemonic activation.*
- 程序调用 `click()` 或 `animateClick()`。 / *A call to `click()` or `animateClick()`.*

以下操作不会触发它：

*It is not emitted by:*

- `setChecked(...)`
- `setCheckState(...)`
- `toggle()`
- `setDown(...)`

因此，当业务只应响应“按钮激活动作”，而不应响应数据同步时，优先使用 `clicked()`。注意程序也可以调用 `click()`；若必须严格区分真实用户输入，还需要在事件层处理。

*Prefer `clicked()` when business logic should react to button activation but not model-to-view synchronization. Note that code can also call `click()`; strict detection of physical user input requires event-level handling.*

三态下，参数 `checked` 无法区分 `PartiallyChecked` 和 `Checked`，因为两者都得到 `true`。需要精确状态时应调用 `checkState()`：

*In tristate mode, the `checked` parameter cannot distinguish `PartiallyChecked` from `Checked`, because both produce `true`. Read `checkState()` for the exact value:*

```cpp
connect(checkBox, &QCheckBox::clicked, this, [checkBox](bool) {
    const Qt::CheckState exactState = checkBox->checkState();
    // 使用精确状态 / Use the exact state
});
```

### 5.5 `toggled(bool)`：布尔状态改变 / Boolean State Changes

```cpp
connect(checkBox, &QCheckBox::toggled, this, [](bool checked) {
    qDebug() << "checked 属性 / checked property:" << checked;
});
```

无论变化来自用户操作还是程序调用，只要布尔 `checked` 属性真正改变，就会发出 `toggled(bool)`。用户点击、`click()`、`toggle()` 和 `setChecked()` 都可能触发它。

*The signal is emitted whenever the boolean `checked` property actually changes, whether caused by a user or by code. A user click, `click()`, `toggle()`, and `setChecked()` can all trigger it.*

重复设置相同值不会再次触发：

*Assigning the same value again does not trigger it:*

```cpp
checkBox->setChecked(true); // false -> true，触发 / emitted
checkBox->setChecked(true); // true  -> true，不触发 / not emitted
```

三态是 `toggled()` 最需要注意的地方：

*Tristate behavior is the key caveat of `toggled()`:*

```text
Unchecked        -> PartiallyChecked : toggled(true)
PartiallyChecked -> Checked          : 不触发 / not emitted
Checked          -> Unchecked        : toggled(false)
```

原因是 `PartiallyChecked` 和 `Checked` 的 `isChecked()` 都为 `true`。若每个精确状态变化都要处理，请使用精确状态信号。

*This happens because `isChecked()` is `true` for both `PartiallyChecked` and `Checked`. Use an exact-state signal when every state transition matters.*

### 5.6 `checkStateChanged(Qt::CheckState)`：强类型精确状态 / Strongly Typed Exact State

Qt 6.7 引入了这个信号：

*Qt 6.7 introduced this signal:*

```cpp
connect(checkBox, &QCheckBox::checkStateChanged,
        this, [](Qt::CheckState state) {
    if (state == Qt::PartiallyChecked) {
        qDebug() << "部分选中 / Partially checked";
    }
});
```

它携带 `Qt::Unchecked`、`Qt::PartiallyChecked` 或 `Qt::Checked`，是新版本 Qt 中观察三态变化的首选。用户操作和程序设置都可能导致状态变化；只有实际状态改变时才会发出。

*It carries `Qt::Unchecked`, `Qt::PartiallyChecked`, or `Qt::Checked`, making it the preferred tristate signal in modern Qt. Both user actions and programmatic assignments can change the state; the signal is emitted only when the value actually changes.*

### 5.7 `stateChanged(int)`：旧版精确状态 / Legacy Exact-State Signal

Qt 6.5/6.6 没有 `checkStateChanged()`，需要使用：

*Qt 6.5/6.6 do not have `checkStateChanged()`, so use:*

```cpp
connect(checkBox, &QCheckBox::stateChanged, this, [](int value) {
    const auto state = static_cast<Qt::CheckState>(value);
    qDebug() << state;
});
```

它表达同一种精确状态变化，但参数是 `int`，类型安全较弱。它从 Qt 6.9 开始被弃用；Qt 6.7 及以上的新代码应使用 `checkStateChanged(Qt::CheckState)`。

*It represents the same exact-state change but uses a less type-safe `int` parameter. It is deprecated starting with Qt 6.9; new code targeting Qt 6.7 or later should use `checkStateChanged(Qt::CheckState)`.*

本例通过条件编译同时支持旧版和新版 Qt：

*The example supports both older and newer Qt through conditional compilation:*

```cpp
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
connect(checkBox, &QCheckBox::checkStateChanged,
        receiver, [](Qt::CheckState state) {
    // Qt 6.7+ 强类型信号 / Strongly typed signal
});
#else
connect(checkBox, &QCheckBox::stateChanged,
        receiver, [](int value) {
    const auto state = static_cast<Qt::CheckState>(value);
    // Qt 6.5/6.6 兼容分支 / Compatibility branch
});
#endif
```

## 6. 示例程序 / Example Program

`main.cpp` 是一个不需要自定义类或 `Q_OBJECT` 的最小可观察示例。窗口包含：

*`main.cpp` is a minimal observable example that needs neither a custom class nor `Q_OBJECT`. Its window contains:*

- 一个三态 `QCheckBox`。 / *A tristate `QCheckBox`.*
- `checkState()`、`isChecked()` 和 `isDown()` 的实时显示。 / *A live view of `checkState()`, `isChecked()`, and `isDown()`.*
- 按真实触发顺序记录相关信号的日志框。 / *A log showing relevant signals in their real emission order.*
- 用来比较 `setCheckState()`、`toggle()` 和 `click()` 的按钮。 / *Buttons that compare `setCheckState()`, `toggle()`, and `click()`.*

建议按以下顺序实验：

*Suggested experiments:*

1. 连续点击复选框三次，观察三态循环。  
   *Click the check box three times and observe the tristate cycle.*
2. 点击“设为部分选中”，观察没有 `pressed`、`released` 或 `clicked`。  
   *Click “Set partially checked” and observe that no `pressed`, `released`, or `clicked` signal appears.*
3. 点击 `toggle()`，观察状态改变但没有模拟点击过程。  
   *Click `toggle()` and observe a state change without a simulated click sequence.*
4. 点击 `click()`，观察完整的点击信号。  
   *Click `click()` and observe the complete click signals.*
5. 在部分选中与完全选中之间切换，观察 `isChecked()` 都为 `true`。  
   *Move between partial and full selection and observe that `isChecked()` is `true` for both.*

## 7. 实际业务场景 / Practical Business Scenarios

`QCheckBox` 通常对应一个可以独立开关的业务选项，也可以用三态表示一组数据的汇总结果。选择信号时，应先判断业务关心的是用户操作、布尔值变化，还是完整的三态值。

*A `QCheckBox` usually represents an independently enabled business option, or a tristate summary of a group. Choose a signal according to whether the business cares about user activation, a boolean value change, or the exact tristate value.*

### 7.1 结算页：勾选“需要发票”后开放填写区域 / Checkout Invoice Form

在电商结算页中，发票抬头、税号等字段只应在用户需要发票时可编辑。这里关心的是选中状态，因此使用 `toggled(bool)`，并在建立连接后主动同步初始状态。

*On a checkout page, invoice fields should be editable only when the customer requests an invoice. This is a boolean-state dependency, so use `toggled(bool)` and synchronize the initial state after connecting.*

```cpp
connect(needInvoiceCheckBox, &QCheckBox::toggled,
        invoiceDetailsWidget, &QWidget::setEnabled);

invoiceDetailsWidget->setEnabled(needInvoiceCheckBox->isChecked());
```

### 7.2 只关心用户操作：保存通知偏好 / User Actions Only: Save Notification Preferences

在设置中心中，“接收促销通知”只应在用户通过鼠标、键盘或助记键主动修改时写入配置并记录审计日志。程序从服务端加载数据后调用 `setChecked()` 回显状态，不应被当成一次用户操作，因此这里使用 `clicked(bool)`，而不是 `toggled(bool)`。

*For a setting such as “Receive promotional notifications,” persist and audit the change only when the user activates the check box with the mouse, keyboard, or mnemonic. Restoring server data with `setChecked()` is not a user action, so use `clicked(bool)` instead of `toggled(bool)`.*

```cpp
connect(marketingNoticeCheckBox, &QCheckBox::clicked,
        this, [this](bool enabled) {
            settings->setMarketingNoticesEnabled(enabled);
            auditLog->recordPreferenceChange("marketingNotices", enabled);
        });

// 仅回显服务端状态，不触发 clicked(bool)。
// Only reflects server state; clicked(bool) is not emitted.
marketingNoticeCheckBox->setChecked(
    settings->marketingNoticesEnabled());
```

注意，代码主动调用 `click()` 或 `animateClick()` 也会模拟一次激活并发出 `clicked()`。如果只是刷新界面，应使用 `setChecked()` 或 `setCheckState()`。

*Calling `click()` or `animateClick()` also simulates activation and emits `clicked()`. Use `setChecked()` or `setCheckState()` when the program only needs to refresh the UI.*

### 7.3 文件管理器：“全选”反映列表选中情况 / File Manager Select All

文件列表中的“全选”复选框需要表达三种结果：没有文件被选中、部分文件被选中、全部文件被选中。此时不能只读取 `isChecked()`，而应根据已选数量设置精确的 `Qt::CheckState`。

*A file manager's “Select all” check box needs three states: none, some, or all files selected. A boolean `isChecked()` value is not precise enough; derive and assign the exact `Qt::CheckState` from the selection count.*

```cpp
Qt::CheckState selectionState(int selectedCount, int totalCount)
{
    if (selectedCount == 0)
        return Qt::Unchecked;
    if (selectedCount == totalCount)
        return Qt::Checked;
    return Qt::PartiallyChecked;
}

selectAllCheckBox->setTristate(true);
selectAllCheckBox->setCheckState(
    selectionState(selectedFiles.size(), allFiles.size()));
```

当用户操作“全选”时，使用 `clicked(bool)` 区分全选和取消全选。程序根据列表变化调用 `setCheckState()` 更新汇总状态时，不会误触发批量选择逻辑；`PartiallyChecked` 只作为列表汇总出来的展示状态。

*Use `clicked(bool)` to distinguish select-all from clear-all when the user activates the check box. Updating the summary with `setCheckState()` as the list changes does not accidentally trigger the bulk-selection logic; `PartiallyChecked` remains a display state derived from the list.*

```cpp
connect(selectAllCheckBox, &QCheckBox::clicked,
        this, [this](bool selectAll) {
            if (selectAll)
                fileSelection->selectAll();
            else
                fileSelection->clear();
        });
```

### 7.4 账户配置：服务端回显时避免重复提交 / Avoid Resubmitting Server State

账户设置页常在保存后重新拉取服务端数据。如果 `toggled(bool)` 的槽会发送更新请求，直接回填控件可能再次触发请求。使用 `QSignalBlocker` 可以只更新界面，不重复调用服务端。

*An account settings page often reloads server data after saving. If a `toggled(bool)` handler sends an update request, assigning the returned value can send the same request again. Use `QSignalBlocker` to refresh the UI without resubmitting it.*

```cpp
#include <QSignalBlocker>

connect(emailNoticeCheckBox, &QCheckBox::toggled,
        this, [this](bool enabled) {
            accountService->updateEmailNotifications(enabled);
        });

void SettingsPage::applyServerState(bool enabled)
{
    const QSignalBlocker blocker(emailNoticeCheckBox);
    emailNoticeCheckBox->setChecked(enabled);
}
```

`QSignalBlocker` 只在其作用域内阻止信号，复选框本身的状态仍会正常更新。

*`QSignalBlocker` suppresses signals only within its scope; the check box state itself is still updated.*

## 8. 常见错误 / Common Mistakes

### 错误 1：把三态值直接当布尔值 / Treating a Tristate Value as a Boolean

```cpp
if (checkBox->checkState()) {
    // PartiallyChecked(1) 和 Checked(2) 都会进入这里。
    // Both PartiallyChecked(1) and Checked(2) enter here.
}
```

应明确比较枚举值：

*Compare the enum explicitly:*

```cpp
if (checkBox->checkState() == Qt::Checked) {
    // 仅完全选中 / Fully checked only
}
```

### 错误 2：用 `clicked()` 监听所有变化 / Using `clicked()` for Every Change

`setChecked()` 和 `setCheckState()` 不会触发 `clicked()`。需要同时监听用户和程序造成的变化时，请用 `toggled()` 或精确状态信号。

*`setChecked()` and `setCheckState()` do not emit `clicked()`. Use `toggled()` or an exact-state signal to observe changes from both users and code.*

### 错误 3：认为 `toggled()` 能区分三态 / Expecting `toggled()` to Distinguish Three States

`toggled()` 只有一个 `bool` 参数，无法区分部分选中和完全选中。三态逻辑应使用精确状态信号。

*`toggled()` carries only a `bool`, so it cannot distinguish partial from full selection. Tristate logic should use an exact-state signal.*

### 错误 4：重复设置相同状态并期待信号 / Expecting a Signal for an Identical Assignment

状态信号描述的是“变化”。新旧值相同时，Qt 不会仅为赋值动作重复发出信号。

*State signals describe a change. Qt does not re-emit them merely because the same value was assigned again.*

### 错误 5：用复选框表达互斥选择 / Using Check Boxes for Exclusive Choices

复选框通常表示可以同时启用的独立选项。若只能选择一个项目，优先使用 `QRadioButton` 和 `QButtonGroup`，语义与可访问性更清楚。

*Check boxes normally represent independent options that may be enabled together. If exactly one item may be selected, prefer `QRadioButton` with `QButtonGroup` for clearer semantics and accessibility.*

## 9. 如何选择信号 / Choosing the Right Signal

```text
需要按下/释放的即时反馈？ / Need immediate press/release feedback?
├── 是 / Yes: pressed() / released()
└── 否 / No
    ├── 只处理一次有效激活？ / Process valid activation only?
    │   └── clicked(bool)
    └── 监听状态数据变化 / Observe state data changes
        ├── 只需要开/关？ / Only on/off?
        │   └── toggled(bool)
        └── 需要完整三态？ / Need the exact tristate value?
            ├── Qt 6.7+: checkStateChanged(Qt::CheckState)
            └── Qt 6.5/6.6: stateChanged(int)
```

一句话记忆：**`clicked` 看动作，`toggled` 看布尔变化，`checkStateChanged` 看完整三态，`pressed/released` 看交互过程。**

*In one sentence: **`clicked` observes activation, `toggled` observes boolean changes, `checkStateChanged` observes the exact tristate value, and `pressed/released` observe the interaction phases.***

## 10. 参考文档 / References

- [Qt 6: QCheckBox](https://doc.qt.io/qt-6/qcheckbox.html)
- [Qt 6: QAbstractButton](https://doc.qt.io/qt-6/qabstractbutton.html)
- [Qt 6: Qt::CheckState](https://doc.qt.io/qt-6/qt.html#CheckState-enum)
- [Qt 6: Obsolete QCheckBox Members](https://doc.qt.io/qt-6/qcheckbox-obsolete.html)
