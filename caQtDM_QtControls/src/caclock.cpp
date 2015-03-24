/*
 *  This file is part of the caQtDM Framework, developed at the Paul Scherrer Institut,
 *  Villigen, Switzerland
 *
 *  The caQtDM Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The caQtDM Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the caQtDM Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright (c) 2010 - 2014
 *
 *  Author:
 *    Anton Mezger
 *  Contact details:
 *    anton.mezger@psi.ch
 */

#if defined(_MSC_VER)
#define NOMINMAX
#include <windows.h>
#define QWT_DLL
#endif

#include "caclock.h"

#if defined(_MSC_VER)
#define fmax max
#define fmin min
#endif

caClock::caClock(QWidget *parent) : QwtAnalogClock(parent)
{
    setReadOnly(true);
    setFocusPolicy(Qt::NoFocus);

    thisBaseColor = QColor( Qt::gray);
    thisScaleColor = thisBaseColor.dark( 200 ).light( 800 );
    setScaleDefaultColor(true);

    const QColor knobColor = thisBaseColor.light(130);
    setPalette( colorTheme( thisBaseColor.dark( 150 ) ) );

    scaleDraw()->setPenWidth(3);
    setLineWidth(3);
    setFrameShadow(QwtDial::Sunken);

    scaleDraw()->setTickLength( QwtScaleDiv::MajorTick, 6);
    scaleDraw()->setSpacing(3.0);

    for ( int i = 0; i < QwtAnalogClock::NHands; i++) {
        QColor handColor = thisBaseColor.light(150);
        int width = 5;
        if ( i == QwtAnalogClock::SecondHand ) {
            handColor = Qt::red;
            width = 3;
        }
        QwtDialSimpleNeedle *hand = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow, true, handColor, knobColor);
        hand->setWidth(width);
        setHand((QwtAnalogClock::Hand)i, hand);
    }

    //setTime();
    // update the clock every second
    timer = new QTimer(this);
    timer->connect(timer, SIGNAL(timeout()), this, SLOT(setCurrentTime()));
    connect(this, SIGNAL(updateTime(QTime)), this, SLOT(setClockTime(QTime)));

    setTimeType (InternalTime);
    setColorMode(Static);
}

QPalette caClock::colorTheme( const QColor &base ) const
{
    QPalette palette;
    palette.setColor( QPalette::Base, base );
    palette.setColor( QPalette::Window, base.dark( 150 ) );
    palette.setColor( QPalette::Mid, base.dark( 110 ) );
    palette.setColor( QPalette::Light, base.light( 170 ) );
    palette.setColor( QPalette::Dark, base.dark( 170 ) );

    if(thisScaleDefaultColor) {
       palette.setColor( QPalette::Text, base.dark( 200 ).light( 800 ) );
    } else {
       palette.setColor( QPalette::Text, thisScaleColor );
    }
    palette.setColor( QPalette::WindowText, base.dark( 200 ) );

    return palette;
}

void caClock::setClockTime(QTime time) {
    setTime(time);
}

void caClock::updateClock(QTime time) {
    if(time == prevTime) return;
    prevTime = time;
    emit updateTime(time);
}

void caClock::resizeEvent(QResizeEvent *e)
{

    Q_UNUSED(e);
    int Width = width();
    QFont f = font();
    f.setPointSizeF(Width/20.0);
    setFont(f);
    update();
}

void caClock::setAlarmColors(short status)
{
    QColor c;
    switch (status) {

    case NO_ALARM:
        c=AL_GREEN;
        if(thisColorMode == Static) c= thisBaseColor.dark( 150 );
        break;
    case MINOR_ALARM:
        c=AL_YELLOW;
        if(thisColorMode == Static) c= thisBaseColor.dark( 150 );
        break;
    case MAJOR_ALARM:
        c=AL_RED;
        if(thisColorMode == Static) c= thisBaseColor.dark( 150 );
        break;
    case INVALID_ALARM:
        c=AL_WHITE;
        if(thisColorMode == Static) c= thisBaseColor.dark( 150 );
        break;
    case NOTCONNECTED:
        c=AL_WHITE;
        break;
    default:
        c= thisBaseColor.dark( 150 );
        break;
    }
    if((prevColor != c) || (thisScaleDefaultColor != prevScaleDefaultColor) || (thisScaleColor != prevScaleColor)){
      prevColor = c;
      prevScaleDefaultColor = thisScaleDefaultColor;
      prevScaleColor = thisScaleColor;
      setPalette( colorTheme( c) );
    }
}
