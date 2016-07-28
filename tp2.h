#ifndef TP2_H
#define TP2_H

#include <QtGui>
#include <iostream>
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <Phonon/VolumeSlider>
#include "Traceur.h"

using namespace std;

class wavBuffer;

class Tp2 : public QMainWindow
{
    Q_OBJECT
    QAction *save;
    QAction *open;
    QAction *exit;
    QAction *end;
    QAction *play;
    QAction *pause;
    QAction *stop;
    QAction *cut;

    QLineEdit *fichier;
    QLineEdit *canaux;
    QLineEdit *ech;
    QLineEdit *freq;
    QLineEdit *Itime;
    QSlider *sld;
    QSpinBox *epp;
    QScrollBar *track;

    wavBuffer *song;
    Phonon::MediaObject *musique;
    Phonon::AudioOutput *sortie;
    Phonon::MediaSource *source;

    Traceur *right;
    Traceur *left;
    int currentFrame;

    public:
        Tp2();
        qint64 temps;

    public slots:
        void loadFile();
        void saveFile();
        void quit();
        void cutFile();
        void setTime(qint64);
        void changerEchelle(int valeur);
        void changerScroll(int valeur);
        void refresh(qint64);
        void scrollBarPas(int valeur);

};

#endif // TP2_H
