/*
 *  cQSettingsDialog.cpp
 *  Visor
 *
 *  Created by mb on 9/23/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQSettingsDialog.h"
#include <Replicode/Hypertree/cVisualizerReplicodeSettings.h>
using namespace Visor;

char *gColorNames[]=
{
    "Default node",
    "Default line",
    "Reduction node",
    "Reduction member",
    "Reduction input",
    "Reduction product",
    "Code injection",
    "Group visibility",
    "Saliency",
    "Activation",
    "Background",
};

/**********************************************************************/
cQSettingsDialog::cQSettingsDialog()
/**********************************************************************/
{
    setFeatures( QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar );
        
    mContainer.setLayout( &mLayout );
    mLayout.addLayout( &mGrid );
    
    int vIndex = 0;
    cVisualizerReplicodeSettings &vSettings = cVisualizerReplicodeSettings::Get();
    for ( int i=0; i<cVisualizerReplicodeSettings::COLOR_MAX; i++ )
    {
        AddColor( vIndex++, gColorNames[i], vSettings.GetColor( (cVisualizerReplicodeSettings::eColor)i ), mGrid );
    }
    
    setWidget( &mContainer );        
}

/**********************************************************************/
void cQSettingsDialog::AddColor( int iRow, const QString &iLabel, const Ogre::ColourValue &iColor, QGridLayout &iGrid )
/**********************************************************************/
{
    QLabel *vLabel = new QLabel( iLabel );
    vLabel->setAlignment( Qt::AlignRight );
    iGrid.addWidget( vLabel, iRow, 0 );
    

    QPixmap vPixmap( 24, 16 );
    vPixmap.fill( QColor( iColor.r * 255, iColor.g * 255, iColor.b * 255 ) );
    QPushButton *vColor = new QPushButton( QIcon( vPixmap ), "" );
    vColor->setProperty( "OgreColor", (int)&iColor );
    vColor->setFixedSize( 26, 18 );
    iGrid.addWidget( vColor, iRow, 1 );
    connect( vColor, SIGNAL( clicked(bool) ), this, SLOT( OnColorClicked(bool) ) );
}

/**********************************************************************/
void cQSettingsDialog::OnColorClicked( bool iChecked )
/**********************************************************************/
{
    QPushButton *vSender = (QPushButton*)sender();
    Ogre::ColourValue *iColor = (Ogre::ColourValue*)(vSender->property("OgreColor").toInt());
    QColorDialog vDialog;
    vDialog.setCurrentColor( QColor( iColor->r * 255, iColor->g * 255, iColor->b * 255 ) );
    if ( !vDialog.exec() ) return;

    QColor vColor = vDialog.currentColor();
    iColor->r = vColor.red()   / 255.0f;
    iColor->g = vColor.green() / 255.0f;
    iColor->b = vColor.blue()  / 255.0f;
    
    QPixmap vPixmap( 24, 16 );
    vPixmap.fill( vColor );
    vSender->setIcon( QIcon( vPixmap ) );
}
