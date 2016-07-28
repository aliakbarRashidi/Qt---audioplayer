#include "wavBuffer.h"
#include <QFile>
#include <iostream>
#include <iomanip>
#include <cstdio>
using namespace std ;


wavBuffer::wavBuffer()
{
    buffer().clear() ;
	filename_ = "" ;
    err_ = "No error ! ";
}

wavBuffer::~wavBuffer()
{
        if(buffer().size() != 0)
	{
		emit managerDestroyed(this) ;
	}
}

bool wavBuffer::loadWav(const char* wavefilename)
{
    // load bytes
    QFile f(wavefilename) ;
    buffer().clear();
    if(f.open(QIODevice::ReadOnly ))
    {
        buffer().append(f.readAll()) ;
    }
    else
    {
        err_ = "WAV file not found" ;
        return false ;
    }
    // analyser le header
    QByteArray& buf = buffer() ;
    if( ! buf.startsWith("RIFF") || (buf.at(0x14) != 1) ||
            (buf.at(8) != 'W') || (buf.at(9) != 'A' )                                                                           ||
            (buf.at(10) != 'V') || (buf.at(11)!= 'E') )
    {
        err_ =  "Bad/Unsupported WAV file format" ;
        return false ;
    }
    filename_ = wavefilename ;
    emit fileLoaded(this) ;
    return true ;
}

int wavBuffer::nbChannels()
{
        return getu8(0x16) ;
}
 int wavBuffer::bitsPerSample()
{
     return getu8(0x22) ;
}
 int wavBuffer::bytesPerSample()
{
        return bitsPerSample() / 8 ;
}
 int wavBuffer::bytesPerFrame()
{
        return nbChannels() * bitsPerSample()  / 8 ;
}
 int wavBuffer::getu8(int offset)
 {
     return (int)(unsigned char)buffer().at(offset) ;
 }

 int wavBuffer::framesPerSecond()
 {
     return     getu8(0x18)
             | (getu8(0x19) << 8)
             | (getu8(0x1A) << 16)
             | (getu8(0x1B) << 24) ;
 }

 int wavBuffer::soundSize()
 {
     return     getu8(0x28)
             | (getu8(0x29) << 8)
             | (getu8(0x2A) << 16)
             | (getu8(0x2B) << 24) ;
 }
 // encodage d'un int dans un QByteArray
 // garantissant l'ordre des octets
 // (octets de poids faible aux index faibles)
 QByteArray wavBuffer::makeInt32(int x)
 {
     QByteArray r ;
     for(int i = 0 ; i < 4 ; ++i, x >>= 8 )
         r.push_back((char)(x&0xFF) );
     return r ;
 }

 void  wavBuffer::setSoundSize(int nbframes)
 {
     buffer().replace(0x28,4, makeInt32(nbframes))  ;
 }

 int wavBuffer::nbFrames()
 {
     int bpf = bytesPerFrame()  ;
     return bpf ? soundSize() / bpf : 0 ;
 }


bool wavBuffer::getFrame(int numF , int& left, int& right)
{
	bool st = true ;

        if( numF >= nbFrames() ) return false ;
        // calcul de l'adresse en octet du frame dans le buffer
        // ATT: les donnees sont precedees d'un  header de 0x2C bytes
        int num = numF * bytesPerFrame() + 0x2C ;
	signed short sl, sr ;
        QByteArray& data = buffer() ;


        if(bitsPerSample() == 16)
	{
                if( nbChannels() == 2)
		{
                        sl = ( short)(unsigned char)data.at(num++) ;
                        sl |= ( short)(unsigned char)data.at(num++) << 8 ;
                        sr = ( short)(unsigned char)data.at(num++)  ;
                        sr |= ( short)(unsigned char)data.at(num) << 8 ;
			left = sl ; right = sr ;
                        //printf("L=%04X  R=%04X \n", left&0xFFFF, right&0xFFFF) ;
		}
		else
		{
                        sl = (short)(unsigned char)data.at(num++) ;
                        sl |= (short)(unsigned char)data.at(num) << 8 ;
			right = 0 ;
			left = sl ;
		}
	}
        else if( bitsPerSample()  == 8)
	{ // echantillons 8 bits , codes en "excedent 127"
                if( nbChannels() == 2)
		{
                        left = ((int)data.at(num++))-127 ;
                        right = ((int)data.at(num))-127 ;
		}
		else
		{
                        left = (int)data.at(num)-127 ;
			right = 0 ;
		}
	}
	else
		st = false ;

	return st ;
}

QString wavBuffer::getFileName()
{
	return filename_ ;
}
const char *wavBuffer::errorText()
{
    const char *p =  err_ ;
    err_ = "no error" ; // reset error message
    return p ;
}

// block duration in milliseconds
int wavBuffer::blockDuration(int nbFrames)
{
	return nbFrames*1000/framesPerSecond() ;
}

qint64 wavBuffer::frame2Time(int frame)
{
    return (qint64)frame*1000/framesPerSecond() ;
}

// remove frames block [start..end]
// return: true if OK
bool wavBuffer::cutBlock(int start,int end)
{
	if( start == -1 || end == -1 || start > end ) return false ;

        // nombre d'octets a effacer et position du premier
        int len = (end-start+1)  * bytesPerFrame() ,
            pos = start *  bytesPerFrame();
        // enlever les octets ATTENTION au header
        // qui decale les données de 0x2C chars!!
        buffer().remove(pos + 0x2C ,len)  ;
        // remettre à jour, la taille (!!! en frames !!! )
        setSoundSize(  soundSize() - (end-start+1) );
	return true ;
}

