#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QSysInfo>

#include <language-manager/ILanguageManager.h>

#include <lyric-tab/LyricTab.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;

    if (auto qssFile = QFile(":/tests/lyric-tab/Resources/theme/lite-dark.qss"); qssFile.open(QIODevice::ReadOnly)) {
        const auto qssBase = qssFile.readAll();
        qssFile.close();
        if (QSysInfo::productType() == "windows") {
            if (QSysInfo::productVersion() == "11")
                window.setStyleSheet(QString("QMainWindow { background: transparent }") + qssBase);
            else
                window.setStyleSheet(QString("QMainWindow { background: #232425; }") + qssBase);
        } else
            window.setStyleSheet(QString("QMainWindow { background: #232425; }") + qssBase);
    }

    const auto langMgr = LangMgr::ILanguageManager::instance();

    QString errorMsg;
    auto args = QJsonObject();
    args.insert("pinyinDictPath", qApp->applicationDirPath() + "/dict");

    langMgr->initialize(args, errorMsg);
    qDebug() << "LangMgr: errorMsg" << errorMsg << "initialized:" << langMgr->initialized();

    auto *lyricTab = new FillLyric::LyricTab(
        {LangNote("好"), LangNote("好的"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"),
         LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"),
         LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好")},
        {}, {true, true});

    window.setCentralWidget(lyricTab);
    window.show();
    window.resize(800, 600);
    lyricTab->setLangNotes(false);

    return QApplication::exec();
}
