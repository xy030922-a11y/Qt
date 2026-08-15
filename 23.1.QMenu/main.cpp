#include <QAction>
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
    windowWidget.setWindowTitle(QStringLiteral("QMenu 最小示例 / Minimal Example"));

    // 按钮通过 setMenu() 持有一个可弹出的菜单。
    // The button exposes a popup menu through setMenu().
    QPushButton *menuPushButton = new QPushButton(
        QStringLiteral("文件操作 / File actions"), &windowWidget);
    QMenu *fileMenu = new QMenu(menuPushButton);
    QAction *openAction = fileMenu->addAction(QStringLiteral("打开 / Open"));
    QAction *saveAction = fileMenu->addAction(QStringLiteral("保存 / Save"));
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction(QStringLiteral("退出 / Exit"));
    menuPushButton->setMenu(fileMenu);

    QLabel *resultLabel = new QLabel(
        QStringLiteral("请选择菜单项 / Select a menu item"), &windowWidget);

    // 每个 QAction 的 triggered() 信号执行对应操作。
    // Each QAction performs its operation through triggered().
    QObject::connect(openAction, &QAction::triggered, resultLabel, [resultLabel] {
        resultLabel->setText(QStringLiteral("已选择打开 / Open selected"));
    });
    QObject::connect(saveAction, &QAction::triggered, resultLabel, [resultLabel] {
        resultLabel->setText(QStringLiteral("已选择保存 / Save selected"));
    });
    QObject::connect(exitAction, &QAction::triggered,
                     &application, &QApplication::quit);

    QVBoxLayout *mainVBoxLayout = new QVBoxLayout(&windowWidget);
    mainVBoxLayout->addWidget(menuPushButton);
    mainVBoxLayout->addWidget(resultLabel);

    windowWidget.resize(360, 140);
    windowWidget.show();
    return application.exec();
}
