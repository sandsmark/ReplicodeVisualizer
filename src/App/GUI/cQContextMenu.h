/*
 *  cQContextMenu.h
 *  Visor
 *
 *  Created by mb on 6/27/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

namespace Visor
{
    class cVisualizerHypertreeNode;
    class cQVisor;
    
    class cQContextMenu : public QMenu
    {
        Q_OBJECT
        
    protected:
        cVisualizerHypertreeNode *mNode;
        cQVisor *mVisor;
        
    public:
        cQContextMenu( cQVisor *iVisor, cVisualizerHypertreeNode *iNode );
        
    public slots:
        void OnViewSource();
        void OnSetName();
        void OnFind();
        void OnResetCamera();
    };

}
