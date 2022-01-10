#include "textedit.h"
#include <QtWidgets/QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);

    QTranslator translator, translatorBase;
    translator.load("textedit_zh.qm");
    translatorBase.load("qt_zh_CN.qm");
    a.installTranslator(&translator);
    a.installTranslator(&translatorBase);
    Textedit w;
    w.show();
    return a.exec();
}
