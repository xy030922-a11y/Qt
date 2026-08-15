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
