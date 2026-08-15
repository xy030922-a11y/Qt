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
