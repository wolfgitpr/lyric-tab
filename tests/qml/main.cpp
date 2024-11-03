#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFormLayout>
#include <QQmlContext>
#include <QMainWindow>
#include <QQuickView>
#include <QSpinBox>
#include <QUrl>
#include <QVBoxLayout>

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool wrap READ wrap WRITE setWrap NOTIFY wrapChanged)

public:
    AppController() : m_wrap(true) {}

    bool wrap() const { return m_wrap; }
    void setWrap(bool wrap) {
        if (m_wrap != wrap) {
            m_wrap = wrap;
            emit wrapChanged();
        }
    }

signals:
    void wrapChanged();

private:
    bool m_wrap;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow win;

    AppController controller; // 创建控制器
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appController", &controller); // 注册到 QML

    // 加载 QML 文件
    engine.load(QUrl(QStringLiteral("qrc:/tests/qml/lyricWrapView.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return QApplication::exec();
}
#include "main.moc"
