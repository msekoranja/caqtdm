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

#ifndef limitsCartesianplotDialog_H
#define limitsCartesianplotDialog_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QList>
#include <QPair>
#include <QString>
#include <QLabel>
#include "cacartesianplot.h"
#include "dbrString.h"
#include "knobDefines.h"
#include "knobData.h"
#include "mutexKnobData.h"

 class QCheckBox;
 class QDialogButtonBox;
 class QLabel;
 class QLineEdit;
 class QTableWidget;
 class QTextEdit;
 class QWidget;

 class limitsCartesianplotDialog : public QDialog
{
     Q_OBJECT

 public:

     limitsCartesianplotDialog(caCartesianPlot *w, MutexKnobData *data, const QString &title, QWidget *parent);

 public slots:
     void cancelClicked();
     void applyClicked();

 private:
     QComboBox *xComboBox;
     QComboBox *yComboBox;
     QCheckBox *xCheckBox;
     QCheckBox *yCheckBox;
     QLineEdit *xminLineEdit;
     QLineEdit *xmaxLineEdit;
     QLineEdit *yminLineEdit;
     QLineEdit *ymaxLineEdit;
     caCartesianPlot *CartesianPlot;
     MutexKnobData *monData;
 };


 #endif
