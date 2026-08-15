#include <QApplication>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGlobal>

namespace {

// 把枚举状态转换为适合界面显示的双语文本。
// Convert an enum state to bilingual text suitable for the UI.
QString stateText(Qt::CheckState state)
{
    switch (state) {
    case Qt::Unchecked:
        return QStringLiteral("未选中 / Unchecked");
    case Qt::PartiallyChecked:
        return QStringLiteral("部分选中 / PartiallyChecked");
    case Qt::Checked:
        return QStringLiteral("已选中 / Checked");
    }
    return QStringLiteral("未知状态 / Unknown state");
}

// 统一 bool 的显示格式，避免在多个 Lambda 中重复判断。
// Keep bool formatting in one place instead of repeating it in each lambda.
QString boolText(bool value)
{
    return value ? QStringLiteral("true") : QStringLiteral("false");
}

} // 匿名命名空间 / Anonymous namespace

int main(int argc, char *argv[])
{
    // QApplication 管理 Widgets 应用的事件循环和全局资源。
    // QApplication owns the event loop and global resources of a Widgets app.
    QApplication app(argc, argv);

    // 使用普通 QWidget 作为顶层窗口，因此示例不需要自定义类或 Q_OBJECT。
    // A plain QWidget is enough, so the example needs no custom class or Q_OBJECT.
    QWidget window;
    window.setWindowTitle(QStringLiteral("QCheckBox 信号观察器 / Signal Inspector"));
    window.resize(720, 480);

    // 所有子控件都以 window 为父对象，会随窗口一起自动销毁。
    // Every child has window as its parent and is deleted with the window.
    auto *layout = new QVBoxLayout(&window);
    auto *description = new QLabel(
        QStringLiteral("操作复选框或点击程序控制按钮；信号会按实际顺序显示。\n"
                       "Operate the check box or use the programmatic controls; "
                       "signals appear in their actual emission order."),
        &window);
    description->setWordWrap(true);
    layout->addWidget(description);

    // 开启三态后，用户可在未选中、部分选中、已选中之间循环。
    // Tristate mode cycles through Unchecked, PartiallyChecked, and Checked.
    auto *checkBox = new QCheckBox(
        QStringLiteral("启用云同步（支持三态） / Enable cloud sync (tristate)"), &window);
    checkBox->setTristate(true);
    checkBox->setCheckState(Qt::Unchecked);
    layout->addWidget(checkBox);

    // 状态标签同时显示三态、布尔 checked 属性和瞬时 down 属性。
    // The label shows the exact state, boolean checked property, and transient down property.
    auto *statusLabel = new QLabel(&window);
    layout->addWidget(statusLabel);

    // 这些按钮用来对比“直接改状态”和“模拟点击”的信号差异。
    // These buttons compare direct state changes with simulated clicks.
    auto *buttonLayout = new QHBoxLayout;
    auto *setPartialButton = new QPushButton(
        QStringLiteral("设为部分选中 / Set partially checked"), &window);
    auto *toggleButton = new QPushButton(QStringLiteral("toggle()"), &window);
    auto *clickButton = new QPushButton(QStringLiteral("click()"), &window);
    auto *resetButton = new QPushButton(
        QStringLiteral("重置 / Reset"), &window);
    buttonLayout->addWidget(setPartialButton);
    buttonLayout->addWidget(toggleButton);
    buttonLayout->addWidget(clickButton);
    buttonLayout->addWidget(resetButton);
    layout->addLayout(buttonLayout);

    auto *log = new QTextEdit(&window);
    log->setReadOnly(true);
    log->setPlaceholderText(
        QStringLiteral("信号日志会显示在这里…… / Signal log appears here..."));
    layout->addWidget(log, 1);

    // 给每条日志加递增序号，以便观察真实的信号发出顺序。
    // Prefix every entry with a sequence number to reveal the real emission order.
    int sequence = 0;
    const auto appendLog = [log, &sequence](const QString &message) {
        log->append(QStringLiteral("%1. %2").arg(++sequence).arg(message));
    };

    // 每次信号到达后，从控件重新读取当前值，而不是猜测状态。
    // Read the current values from the widget after each signal instead of assuming them.
    const auto refreshStatus = [checkBox, statusLabel] {
        statusLabel->setText(
            QStringLiteral("当前状态 / Current state: %1; isChecked() = %2; isDown() = %3")
                .arg(stateText(checkBox->checkState()))
                .arg(boolText(checkBox->isChecked()))
                .arg(boolText(checkBox->isDown())));
    };

    // QCheckBox 从 QAbstractButton 继承的四个交互/布尔状态信号。
    // Four interaction/boolean-state signals inherited from QAbstractButton.
    QObject::connect(checkBox, &QCheckBox::pressed, &window, [=] {
        appendLog(QStringLiteral("pressed(): 按钮进入按下状态 / button becomes pressed"));
        refreshStatus();
    });
    QObject::connect(checkBox, &QCheckBox::released, &window, [=] {
        appendLog(QStringLiteral("released(): 按钮离开按下状态 / button is released"));
        refreshStatus();
    });
    QObject::connect(checkBox, &QCheckBox::clicked, &window, [=](bool checked) {
        appendLog(QStringLiteral("clicked(checked = %1): 完成有效激活 / valid activation completed")
                      .arg(boolText(checked)));
        refreshStatus();
    });
    QObject::connect(checkBox, &QCheckBox::toggled, &window, [=](bool checked) {
        appendLog(QStringLiteral("toggled(checked = %1): checked 属性改变 / property changed")
                      .arg(boolText(checked)));
        refreshStatus();
    });

    // Qt 6.7 引入了强类型信号；Qt 6.5/6.6 使用旧的 int 参数信号。
    // Qt 6.7 added a strongly typed signal; Qt 6.5/6.6 use the older int signal.
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
    QObject::connect(checkBox, &QCheckBox::checkStateChanged, &window,
                     [=](Qt::CheckState state) {
        appendLog(QStringLiteral("checkStateChanged(%1): 精确三态改变 / exact state changed")
                      .arg(stateText(state)));
        refreshStatus();
    });
#else
    QObject::connect(checkBox, &QCheckBox::stateChanged, &window, [=](int state) {
        const auto checkState = static_cast<Qt::CheckState>(state);
        appendLog(QStringLiteral(
                      "stateChanged(%1): 精确三态改变 / exact state changed (Qt 6.5/6.6)")
                      .arg(stateText(checkState)));
        refreshStatus();
    });
#endif

    // 这几个按钮用于比较“直接改状态”和“模拟一次点击”所触发的信号。
    // Compare signals from direct state changes with signals from a simulated click.
    QObject::connect(setPartialButton, &QPushButton::clicked, checkBox, [=] {
        appendLog(QStringLiteral(
            "--- 调用 / Call setCheckState(Qt::PartiallyChecked) ---"));
        checkBox->setCheckState(Qt::PartiallyChecked);
        refreshStatus();
    });
    QObject::connect(toggleButton, &QPushButton::clicked, checkBox, [=] {
        appendLog(QStringLiteral("--- 调用 / Call toggle() ---"));
        checkBox->toggle();
        refreshStatus();
    });
    QObject::connect(clickButton, &QPushButton::clicked, checkBox, [=] {
        appendLog(QStringLiteral("--- 调用 / Call click() ---"));
        checkBox->click();
        refreshStatus();
    });
    QObject::connect(resetButton, &QPushButton::clicked, checkBox, [=] {
        appendLog(QStringLiteral("--- 调用 / Call setCheckState(Qt::Unchecked) ---"));
        checkBox->setCheckState(Qt::Unchecked);
        refreshStatus();
    });

    // 连接信号不会自动刷新初始界面，因此在 show() 前主动同步一次。
    // Connecting signals does not initialize the UI, so synchronize it before show().
    refreshStatus();
    window.show();
    return app.exec();
}
