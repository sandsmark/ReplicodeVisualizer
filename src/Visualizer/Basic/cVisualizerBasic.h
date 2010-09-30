/*
 *  cVisualizerBasic.h
 *  Visor
 *
 *  Created by mb on 5/29/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Visualizer/IVisualizer.h>

namespace Visor
{
    class IComponent;
    
    class cVisualizerBasic : public IVisualizer, public IListenerComponent
    {
    protected:
        Ogre::SceneManager      *mScene;
        Ogre::BillboardSet *mBillboards;
        
    public:
        cVisualizerBasic( Ogre::SceneManager *iScene );
        
        Ogre::BillboardSet *GetBillboards( void ) const { return mBillboards; }
        
        // IListenerComponent methods
        virtual void OnComponentCreated  ( IComponent *iComponent );
        virtual void OnComponentDestroyed( IComponent *iComponent );        
    };
}
