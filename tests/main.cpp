#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QSysInfo>

#include <language-manager/IG2pManager.h>
#include <language-manager/ILanguageManager.h>

#include <lyric-tab/LyricTab.h>

using namespace FillLyric;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;

    if (auto qssFile = QFile(":theme/lite-dark.qss"); qssFile.open(QIODevice::ReadOnly)) {
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

    const auto g2pMgr = LangMgr::IG2pManager::instance();
    const auto langMgr = LangMgr::ILanguageManager::instance();

    QString errorMsg;
    g2pMgr->initialize(qApp->applicationDirPath() + "/dict", errorMsg);

    if (!g2pMgr->initialized())
        qDebug() << "G2pMgr: errorMsg" << errorMsg << "initialized:" << g2pMgr->initialized();

    langMgr->initialize(errorMsg);
    if (!langMgr->initialized())
        qDebug() << "LangMgr: errorMsg" << errorMsg << "initialized:" << langMgr->initialized();

    auto *lyricTab =
        new LyricTab({LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"),
                      LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"),
                      LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好"), LangNote("好")},
                     {true, true});

    window.setCentralWidget(lyricTab);
    window.show();
    window.resize(800, 600);
    lyricTab->setLangNotes(false);

    return QApplication::exec();
}
