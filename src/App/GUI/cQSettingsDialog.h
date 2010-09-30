/*
 *  cQSettingsDialog.h
 *  Visor
 *
 *  Created by mb on 9/23/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

namespace Visor
{
    
    class cQSettingsDialog : public QDockWidget
    {
        Q_OBJECT
     
    protected:
        
    protected:
        QWidget mContainer;
        QVBoxLayout mLayout;
        QGridLayout mGrid;
        
        void AddColor( int iRow, const QString &iLabel, const Ogre::ColourValue &iColor, QGridLayout &iGrid );
        
    public:
        cQSettingsDialog();
        
    public slots:
        void OnColorClicked( bool iChecked );
        
    };

}
