#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QSysInfo>

#include <lyric-tab/LyricTab.h>

#include <filesystem>
#include <iostream>
#include <string>

#include <stdcorelib/str.h>
#include <stdcorelib/system.h>

#include <LangCore/Core/Manager.h>
#include <LangCore/Module/Module.h>

#include <LangCore/Task/SessionTask.h>
#include <LangCore/Task/TaskPlugin.h>

using EP = LangCore::ExecutionProvider;

std::filesystem::path getPluginRootDirectory() {
#if defined(Q_OS_MAC)
    return MacOSUtils::getMainBundlePath() / _TSTR("Contents/PlugIns");
#elif defined(Q_OS_WIN)
    return stdc::system::application_directory() / _TSTR("plugins");
#else
    return stdc::system::application_directory().parent_path() / _TSTR("lib/plugins");
#endif
}

EP parseExecutionProvider(const std::string &provider) {
    const auto providerLower = stdc::to_lower(provider);
    if (providerLower == "dml" || providerLower == "directml") {
        return EP::DMLExecutionProvider;
    }
    if (providerLower == "cuda") {
        return EP::CUDAExecutionProvider;
    }
    if (providerLower == "coreml") {
        return EP::CoreMLExecutionProvider;
    }
    return EP::CPUExecutionProvider;
}

bool initializeOnnxDriver(const LangCore::Manager *mgr, const std::string &ep, const int deviceIndex,
                          const bool loadFromProgress) {
    const auto onnxDriverPlugin = mgr->plugin<LangCore::DriverPlugin>("onnx");
    if (!onnxDriverPlugin) {
        std::cerr << "Failed to load ONNX inference driver" << std::endl;
        return false;
    }

    auto expOnnxDriver = onnxDriverPlugin->create();
    if (!expOnnxDriver) {
        std::cerr << "Failed to load ONNX inference driver" << std::endl;
        return false;
    }

    const auto onnxArgs = LangCore::NO<LangCore::DriverInitArgs>::create();

    const auto ep_ = parseExecutionProvider(ep);
    onnxArgs->ep = ep_;
    const auto ortParentPath = onnxDriverPlugin->path().parent_path() / _TSTR("runtimes") / _TSTR("onnx");
    onnxArgs->runtimePath =
        ep_ == EP::CUDAExecutionProvider ? ortParentPath / _TSTR("cuda") : ortParentPath / _TSTR("default");

    onnxArgs->loadFromProcess = loadFromProgress;
    onnxArgs->deviceIndex = deviceIndex;

    const auto onnxDriver = expOnnxDriver.take();

    if (const auto exp = onnxDriver->initialize(onnxArgs); !exp) {
        std::cerr << "Failed to initialize ONNX driver: " << exp.error().message() << std::endl;
        return false;
    }

    auto &driverCategory = *mgr->category("driver");
    driverCategory.addObject("g2pOnnxDriver", onnxDriver);
    return true;
}

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

    const auto langMgr = LangCore::Manager::instance();

    const auto defaultPluginDir = getPluginRootDirectory() / _TSTR("LangPlugins");
    langMgr->addPluginPath("org.openvpi.Driver", defaultPluginDir / _TSTR("Drivers"));
    langMgr->addPluginPath("org.openvpi.Task", defaultPluginDir / _TSTR("G2ps"));

    const std::filesystem::path packagesRootDir = stdc::system::application_directory() / _TSTR("G2pPackages");
    langMgr->addPackagePath(packagesRootDir);

    if (const auto onnxDriverInitialized = initializeOnnxDriver(langMgr, "cpu", 0, false); !onnxDriverInitialized)
        return -1;

    std::string errorMessage;
    langMgr->initialize(errorMessage);
    if (!langMgr->initialized())
        std::cerr << "Failed to initialize langMgr: " << errorMessage << std::endl;
    qDebug() << "LangMgr: errorMsg" << errorMessage << "initialized:" << langMgr->initialized();

    auto *lyricTab = new FillLyric::LyricTab(
        {
            // Chinese
            LangNote("春"), LangNote("眠"), LangNote("不"), LangNote("觉"), LangNote("晓"),
            // Punctuation
            LangNote("，"),
            // Chinese continued
            LangNote("处"), LangNote("处"), LangNote("闻"), LangNote("啼"), LangNote("鸟"),
            LangNote("。"),
            // Japanese
            LangNote("桜"), LangNote("が"), LangNote("咲"), LangNote("く"),
            // Slur
            LangNote("-"),
            // English
            LangNote("hello", "eng", "eng"), LangNote("world", "eng", "eng"),
            // Mixed: Chinese with English
            LangNote("这"), LangNote("是"), LangNote("test"),
            // Cantonese
            LangNote("你"), LangNote("好"),
            // Numbers and punctuation
            LangNote("123"), LangNote("！"),
            // More Chinese polyphones
            LangNote("重"), LangNote("量"), LangNote("重"), LangNote("新"),
            LangNote("了"), LangNote("了"), LangNote("乐"),
        },
        {"cmn", "jpn", "eng", "yue"}, {}, {true, true});

    window.setCentralWidget(lyricTab);
    window.show();
    window.resize(800, 600);
    lyricTab->setLangNotes(false);

    return QApplication::exec();
}
