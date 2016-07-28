#ifndef TRACEUR_H
#define TRACEUR_H
#include <QtGui>
#include "wavBuffer.h"

class Traceur : public QFrame
{
    Q_OBJECT
    wavBuffer *wB;

    public:
        int echelle;
        int scroll;
        int min, max, in, out;
        QVector<QPoint>*point;
        Traceur(QWidget *p);
        ~Traceur(){}

        virtual void paintEvent(QPaintEvent * /* ev */);
        virtual void mousePressEvent(QMouseEvent * /* me */);
        virtual void mouseMoveEvent(QMouseEvent * /* me */);
        void setPoints (wavBuffer *, int canal);
        void setEchelle(int e);
        void setScroll(int s);

    public slots:
        void getLimits(int inf, int sup);

    signals:
        void sendLimits(int inf, int sup);
};

#endif // TRACEUR_H
