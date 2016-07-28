#include <QtGui/QApplication>
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include "tp2.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Tp2 *wid = new Tp2;
    wid->show();

    return  a.exec();
}
