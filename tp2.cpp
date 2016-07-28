#include "tp2.h"
#include "wavBuffer.h"
#include "Traceur.h"
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <Phonon/VolumeSlider>
#include <iostream>

using namespace std;

Tp2::Tp2() : QMainWindow(0)
{
    song = new wavBuffer;
    this->temps = 0;

    // Creation du controleur multimedia
    musique = new Phonon::MediaObject;
    musique->setTickInterval(1);

    // Creation de la source son
    sortie = new Phonon::AudioOutput(Phonon::MusicCategory);


    // Interface graphique =================================================

    QWidget *g = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;
    g->setLayout(vbox);

    // Parametres
    QGroupBox *groupBox1 = new QGroupBox("Parametres");
    QGridLayout *g1 = new QGridLayout;
    g1->setSpacing(4);
    groupBox1->setLayout(g1);

    QLabel *nom = new QLabel(groupBox1);
    nom->setText("Fichier:");
    fichier = new QLineEdit(groupBox1);
    fichier->insert("...");
    fichier->setReadOnly(true);

    QLabel *can = new QLabel(groupBox1);
    can->setText("Canaux:");
    canaux = new QLineEdit(groupBox1);
    canaux->setReadOnly(true);

    QLabel *echantillons = new QLabel(groupBox1);
    echantillons->setText("Echantillons [bits]:");
    ech = new QLineEdit(groupBox1);
    ech->setReadOnly(true);

    QLabel *frequence = new QLabel(groupBox1);
    frequence->setText("Frequence [Hz]:");
    freq = new QLineEdit(groupBox1);
    freq->setReadOnly(true);

    g1->addWidget(nom, 0,0);
    g1->addWidget(fichier, 0,1, 1,5);
    g1->addWidget(can, 1,0);
    g1->addWidget(canaux, 1,1);
    g1->addWidget(echantillons, 1,2);
    g1->addWidget(ech, 1,3);
    g1->addWidget(frequence, 1,4);
    g1->addWidget(freq, 1,5);

    // Echantillons
    QGroupBox *groupBox2 = new QGroupBox("Echantillons");
    QGridLayout *g2 = new QGridLayout;
    g2->setSpacing(4);
    groupBox2->setLayout(g2);

    QLabel *echelle = new QLabel(groupBox1);
    echelle->setText("Echelle [echantillons/pixel]:");
    sld = new QSlider(Qt::Horizontal, groupBox2); // avant this
    sld->setEnabled(false);
    sld->setMaximum(500);
    sld->setMinimum(1);
    sld->setTickPosition(QSlider::TicksBothSides);
    epp = new QSpinBox(groupBox2);
    epp->setEnabled(false);
    epp->setMaximum(500);
    epp->setMinimum(1);
    connect(sld,SIGNAL(valueChanged(int)),epp,SLOT(setValue(int)));
    connect (sld, SIGNAL(valueChanged(int)), this, SLOT(changerEchelle(int)));
    connect (sld, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPas(int)));
    connect(epp,SIGNAL(valueChanged(int)),sld,SLOT(setValue(int)));

    Phonon::VolumeSlider *vol = new Phonon::VolumeSlider;
    vol->setParent(groupBox2);
    vol->setAudioOutput(sortie);
    vol->setOrientation(Qt::Vertical);
    vol->setTracking(true);

    track = new QScrollBar(Qt::Horizontal, groupBox2);
    track->setEnabled(false);
    connect (track, SIGNAL(valueChanged(int)), this, SLOT(changerScroll(int)));
    Itime = new QLineEdit(groupBox2);

    right = new Traceur(groupBox2);
    left = new Traceur(groupBox2);

    g2->addWidget(echelle, 0,0, 1,1);
    g2->addWidget(sld, 0,1, 1,4);
    g2->addWidget(epp, 0,5, 1,1);
    g2->addWidget(right, 1,0, 1,5);
    g2->addWidget(left, 2,0, 1,5);
    g2->addWidget(vol, 1,5, 2,1);
    g2->addWidget(track,3,0, 1,4);
    g2->addWidget(Itime,3,4, 1,2);

    vbox->addWidget(groupBox1);
    vbox->addWidget(groupBox2);

    setCentralWidget(g);

    // a) Creer les actions desirees ======================================
    //Chargement du fichier
    open = new QAction(QIcon(QPixmap(":/open.png")), "&Open", this);
    open->setShortcut(QKeySequence("CTRL+O"));

    //Sauvegarde du fichier
    save = new QAction(QIcon(QPixmap(":/save.png")), "&Save As", this);
    save->setShortcut(QKeySequence("CTRL+S"));
    //save->setEnabled(false);

    //Sortie du programme
    exit = new QAction(QIcon(QPixmap(":/on.png")), "&Exit", this);
    exit->setShortcut(QKeySequence("CTRL+E"));

    //Suivant
    end = new QAction(QIcon(QPixmap(":/forward.png")), "&End", this);
    //end->setShortcut(QKeySequence("CTRL+N"));

    //Play
    play = new QAction(QIcon(QPixmap(":/play.png")), "&Play", this);
    play->setShortcut(QKeySequence("CTRL+R"));

    //Pause
    pause = new QAction(QIcon(QPixmap(":/pause.png")), "&Pause", this);
    pause->setShortcut(QKeySequence("CTRL+P"));

    //Stop
    stop = new QAction(QIcon(QPixmap(":/stop.png")), "&Stop", this);
    stop->setShortcut(QKeySequence("CTRL+A"));

    //Cut
    cut = new QAction(QIcon(QPixmap(":/cut.png")), "&Cut", this);
    cut->setShortcut(QKeySequence("CTRL+X"));

    // b) Conecter ces actions a des slots de traitement ==================
    connect(save, SIGNAL(activated()), this, SLOT(saveFile()));
    connect(open, SIGNAL(activated()), this, SLOT(loadFile()));
    connect(exit, SIGNAL(activated()), this, SLOT(quit()));
    connect(cut, SIGNAL(activated()), this, SLOT(cutFile()));
    connect(play, SIGNAL(activated()), musique, SLOT(play()));
    connect(pause, SIGNAL(activated()), musique, SLOT(pause()));
    connect(stop, SIGNAL(activated()), musique, SLOT(stop()));
    connect(musique, SIGNAL(tick(qint64)), this, SLOT(setTime(qint64)));
    connect(musique, SIGNAL(tick(qint64)), this, SLOT(refresh(qint64)));
    connect(left,SIGNAL(sendLimits(int,int)),right,SLOT(getLimits(int,int)));
    connect(right,SIGNAL(sendLimits(int,int)),left,SLOT(getLimits(int,int)));

    // c) Integrer les actions dans un menu ===============================
    QMenu *fichierMenu = new QMenu("&Fichier", this);
    menuBar()->addMenu(fichierMenu);
    fichierMenu->addAction(save);
    fichierMenu->addAction(open);
    fichierMenu->addAction(exit);

    QMenu *sonMenu = new QMenu("&Son", this);
    menuBar()->addMenu(sonMenu);
    sonMenu->addAction(end);
    sonMenu->addAction(play);
    sonMenu->addAction(pause);
    sonMenu->addAction(stop);
    sonMenu->addAction(cut);


    // d) Integrer dans la toolbar ========================================
    QToolBar *tools = new QToolBar("File and Sound operations", this);
    tools->addAction(open);
    tools->addAction(save);
    tools->addAction(exit);
    tools->addAction(end);
    tools->addAction(play);
    tools->addAction(pause);
    tools->addAction(stop);
    tools->addAction(cut);
    addToolBar(tools);

}

void Tp2::loadFile()
{
    QString aux;
    // Affiche le dialogue de selection d'un fichier
    QString name = QFileDialog::getOpenFileName();
    const char* nom = name.toStdString().c_str();

    if(name == "") return;

    // Titre de la fenetre
    setWindowTitle(name);
    song->loadWav(nom);

    track->setEnabled(true);
    sld->setEnabled(true);
    epp->setEnabled(true);
    sld->setValue(150);


    //Pour setter les valeurs initiales des limites du QScrollBar
    track->setMinimum(0);
    //track->setMaximum(song->nbFrames()/(1*(right->width()-1)));
    track->setMaximum(song->nbFrames());
    track->setPageStep(sld->value());

    /*Setter les valeurs d'echelle et de l'echantillon courant lors du
    chargement du fichier*/
    right->setEchelle(sld->value());
    left->setEchelle(sld->value());
    right->setScroll(track->value());
    left->setScroll(track->value());

    // Lors du chargement du fichier on appelle le Traceur
    right->setPoints(song, 0);
    left->setPoints(song, 1);

    // Affichage de l'information du fichier dans l'interface
    fichier->setText(name);
    aux = song->getFileName();
    int nc = song->nbChannels();
    canaux->setText(QString::number(nc));
    int nech = song->bitsPerSample();
    ech->setText(QString::number(nech));
    int f = song->framesPerSecond();
    freq->setText(QString::number(f));

    // La source sera le fichier wav chargé
    source = new Phonon::MediaSource(song);
    musique->setCurrentSource(*source);
    Phonon::createPath(musique, sortie);
}

void Tp2::saveFile()
{
    // Enregistrement du fichier...
}

void Tp2::setTime(qint64 time)
{
    QTime *ct = new QTime;
    *ct = ct->addMSecs(time);
    Itime->setText(ct->toString(" TEMPS [ mm : ss ]"));
}

void Tp2::quit()
{
    qApp->exit();
}

void Tp2::changerEchelle(int valeur)
{
    epp->setValue(valeur);
    right->setEchelle(valeur);
    right->setPoints(song, 0);

    left->setEchelle(valeur);
    left->setPoints(song, 1);
}

void Tp2::refresh(qint64 time)
{
    currentFrame = (time/1000.) * song->framesPerSecond();
    track->setValue(currentFrame);
}

void Tp2::changerScroll(int valeur)
{
    right->setScroll(valeur);
    right->setPoints(song, 0);

    left->setScroll(valeur);
    left->setPoints(song, 1);
}

void Tp2::scrollBarPas(int valeur)
{
    track->setPageStep(valeur);
}

void Tp2::cutFile()
{
    if(right->in < right->out)
        song->cutBlock( right->in, right->out );
    if(right->in > right->out)
        song->cutBlock( right->out, right->in );

    track->setMaximum(song->nbFrames());
    right->setPoints(song, 0);
    left->setPoints(song, 0);
}
