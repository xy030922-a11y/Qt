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
