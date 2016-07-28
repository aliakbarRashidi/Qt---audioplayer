#include "Traceur.h"
#include "wavBuffer.h"
#include <iostream>

using namespace std;

Traceur::Traceur(QWidget *p) : QFrame(p)
{
    wB = new wavBuffer();
    point = new QVector<QPoint>();
}

void Traceur::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    if (wB->getFileName() == "")
    {
        // Effacer fond du widget de dessin
        p.setViewport(0, 0, width(), height());
        p.setClipRect(0,0,width(),height());
        p.setBackgroundMode( Qt::OpaqueMode );
        p.setBackground( QBrush(QColor(151, 207, 52)));
        p.eraseRect(p.window());

        // Tracer cadre autour de la zonde de dessin
        p.setPen( Qt::SolidLine );
        p.drawRect(p.window());

        p.drawLine(0,height(),width(),0) ;
        p.drawLine(0,0,width(),height()) ;
    }
    else
    {
        // Effacer fond du widget de dessin
        p.setViewport(0, 0, width(), height());
        p.setClipRect(0,0,width(),height());
        p.setBackgroundMode(Qt::OpaqueMode);
        p.setBackground((QBrush(QColor(151, 207, 52))));
        p.eraseRect(p.window());

        if( in != out)
        {
            QRect sel((in - scroll)/echelle, 0, (out-in)/echelle,height());
            p.fillRect(sel,QBrush(QColor(20,20,250)));
        }

        //p.setRenderHint(QPainter::HighQualityAntialiasing);
        QPen pen (p.pen());
        pen.setColor(QColor(0,0,0));
        p.setPen(pen);

        for (int i = 0; i < (width()); i++)
        {
            p.drawLine( i, point->value(i).x(), i, point->value(i).y()+1) ;
        }
    }
}

void Traceur::setPoints(wavBuffer *WBuff, int canal)
{
    wB = WBuff;
    int left, right;
    int valeur;

    for (int i = 0; i < width(); i++)
    {
        wB->getFrame(scroll-(scroll%echelle)+(i*echelle), left, right);
        if (canal == 0)
        {
            valeur = ((right+(65536/2))*height())/65536;
            min = valeur;
            max = valeur;
        }
        else
        {
            valeur = ((left+(65536/2))*height())/65536;
            min = valeur;
            max = valeur;
        }


        // Calcul de min et max pour chaque bloc de k echantillons ( k == echelle)
        for (int j = 0; j < echelle; j++)
        {
            wB->getFrame(scroll-(scroll%echelle)+(i*echelle)+j, left, right);
            if (canal == 0)
            {
                valeur = ((right+(65536/2))*height())/65536;
                if(valeur < min) min = valeur;
                if(valeur > max) max = valeur;
            }
            else
            {
                valeur = ((left+(65536/2))*height())/65536;
                if(valeur < min) min = valeur;
                if(valeur > max) max = valeur;
            }
        }
        point->insert(i, QPoint(min, max));
    }
    update();
}

void Traceur::setEchelle(int e)
{
    echelle = e;
}

void Traceur::setScroll(int s)
{
    scroll = s;
}

void Traceur::mousePressEvent(QMouseEvent *me)
{
    if( me->x()> 0 && me->x()<width()+1)
    {
        in = me->x();
        in = scroll + in*echelle;
        out = in;
        emit sendLimits(in,out);
        update();
    }
}

void Traceur::mouseMoveEvent(QMouseEvent *me)
{
    if( me->x()> 0 && me->x()<width()+1)
    {
        out = me->x();
        out = scroll + out*echelle;
        emit sendLimits(in,out);
        update();
    }
}

void Traceur::getLimits(int inf, int sup)
{
    in = inf;
    out = sup;
    update();
}
