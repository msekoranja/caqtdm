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

#include <QtGui>
#include "limitsStripplotDialog.h"

limitsStripplotDialog::limitsStripplotDialog(caStripPlot *w, MutexKnobData *data, const QString &title, QWidget *parent) : QDialog(parent)
{

    StripPlot = w;
    monData = data;
    QGridLayout *Layout = new QGridLayout;
    QString text = StripPlot->getPVS();

    vars = text.split(";", QString::SkipEmptyParts);

    for(int i=0; i< vars.size(); i++) {
        QString pv = vars.at(i).trimmed();
        if(pv.size() > 0) {
            QLabel *channelLabel = new QLabel(pv);

            minComboBox[i] = new QComboBox;
            minComboBox[i]->addItem("channel");
            minComboBox[i]->addItem("user");
            if(StripPlot->getYscalingMin(i) == caStripPlot::Channel) minComboBox[i]->setCurrentIndex(0);
            else minComboBox[i]->setCurrentIndex(1);

            double minY = StripPlot->getYaxisLimitsMin(i);
            text = text.setNum(minY);
            minLineEdit[i] = new QLineEdit(text);

            maxComboBox[i] = new QComboBox;
            maxComboBox[i]->addItem("channel");
            maxComboBox[i]->addItem("user");
            if(StripPlot->getYscalingMax(i) == caStripPlot::Channel) maxComboBox[i]->setCurrentIndex(0);
            else maxComboBox[i]->setCurrentIndex(1);

            double maxY = StripPlot->getYaxisLimitsMax(i);
            text = text.setNum(maxY);
            maxLineEdit[i] = new QLineEdit(text);

            Layout->addWidget(channelLabel,    i, 0);
            Layout->addWidget(minComboBox[i],  i, 1);
            Layout->addWidget(minLineEdit[i],  i, 2);
            Layout->addWidget(maxComboBox[i],  i, 3);
            Layout->addWidget(maxLineEdit[i],  i, 4);
        }
    }

    QDialogButtonBox *box = new QDialogButtonBox( Qt::Horizontal );
    QPushButton *button = new QPushButton( "Return" );
    connect( button, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
    box->addButton(button, QDialogButtonBox::RejectRole );

    button = new QPushButton( "Apply" );
    connect( button, SIGNAL(clicked()), this, SLOT(applyClicked()) );
    box->addButton(button, QDialogButtonBox::ApplyRole );

    Layout->addWidget(box, vars.size(), 0);

    setLayout(Layout);

    setWindowTitle(title);

}

void limitsStripplotDialog::cancelClicked()
{
    reject();
}

void limitsStripplotDialog::applyClicked()
{
    bool ok;
    QVariant var=StripPlot->property("MonitorList");
    QVariantList list = var.toList();
    int nbMonitors = list.at(0).toInt();

    for(int i=0; i< qMin(vars.size(), nbMonitors); i++) {
        QString pv = vars.at(i).trimmed();
        if(pv.size() > 0) {

            knobData *ptr = monData->GetMutexKnobDataPtr(list.at(i+1).toInt());
            if(ptr == (knobData *) 0) break;

            int indx = minComboBox[i]->currentIndex();
            QString text = minLineEdit[i]->text();
            double minY = text.toDouble(&ok);
            if(ok) {
                if(indx == 1) { // user
                        StripPlot->setYaxisLimitsMin(i, minY);
                        StripPlot->setYscalingMin(i, caStripPlot::User);
                } else {

                        StripPlot->setYscalingMin(i, caStripPlot::Channel);
                        StripPlot->setYaxisLimitsMin(i, ptr->edata.lower_disp_limit);
                }
            }

            indx = maxComboBox[i]->currentIndex();
            text = maxLineEdit[i]->text();
            double maxY = text.toDouble(&ok);
            if(ok) {
                if(indx == 1) { // user
                        StripPlot->setYaxisLimitsMax(i, maxY);
                        StripPlot->setYscalingMax(i, caStripPlot::User);

                } else {
                        StripPlot->setYscalingMax(i, caStripPlot::Channel);
                        StripPlot->setYaxisLimitsMax(i, ptr->edata.upper_disp_limit);
                }
            }
            if(i==0) {
                double ymin = StripPlot->getYaxisLimitsMin(0);
                double ymax = StripPlot->getYaxisLimitsMax(0);
                StripPlot->setYscale(ymin, ymax);
            }
        }
    }

    // force a resize to reinitialize the plot
    StripPlot->resize(StripPlot->width()+1, StripPlot->height()+1);
    StripPlot->resize(StripPlot->width()-1, StripPlot->height()-1);
}

