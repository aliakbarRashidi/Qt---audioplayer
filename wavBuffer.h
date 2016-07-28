#ifndef WAVBUFFER_H
#define WAVBUFFER_H

#include <QString>
#include <QBuffer>

//
// Cette classe implemente une source sonore (codage WAV)
// et permet également un accès aux données pour d'autres
// buts que la creation sonore (ex: visualisation)
//
class wavBuffer : public QBuffer
{
    Q_OBJECT
    private:
        QString filename_ ; // Nom du fichier chargé
        const char *err_ ;  // Message decrivant la dernière erreur rencontré
        int getu8(int N) ;  //renvoie l'octet N du buffer comme un int
        static QByteArray makeInt32(int x) ; // fabrique un buffer a partir d'un int

    public:
        wavBuffer() ;
        ~wavBuffer() ;

        // charge la totalite d'un fichier WAV
        bool loadWav(const char* wavefilename) ;

        // supprime un bloc du buffer allant des frames
        // "b" a "e" inclus
        bool cutBlock(int b,int e) ;

        // les fct suivantes recuperent (ou modifient)
        // des donnees dans l'en-tete WAV
        int nbChannels() ;
        int bitsPerSample() ;
        int bytesPerSample() ;
        int bytesPerFrame() ;
        int nbFrames();
        int framesPerSecond() ;
        int soundSize() ; // taille du morceau (en frames)
        void setSoundSize(int nbframes);

        // renvoie les echantillons droit et gauche d'un frame
        bool getFrame(int frameNumber, int& left, int& right) ;

        // renvoie le nom du fichier associé au buffer
        QString getFileName() ;

        // fournit le message d'erreur correspondant
        // a la derniere erreur rencontree
        // (la consultation efface le message)
        const char *errorText() ;

        // block duration in milliseconds for nbFrames frames
        int blockDuration(int nbFrames) ;

        // convert frame number to time (in ms)
        qint64 frame2Time(int frame) ;

    signals:
        // emis si chargement OK
        void fileLoaded(wavBuffer *wavManager) ;
        // emis  a la destruction
        void managerDestroyed(wavBuffer *wavManager) ;
} ;

#endif
