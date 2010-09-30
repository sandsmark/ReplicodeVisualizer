/*
 *  cVisualizerBasicSprite.h
 *  Visor
 *
 *  Created by mb on 5/31/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Component/Events/IListenerComponent.h>

namespace Visor
{
    class IComponent;
    class cVisualizerBasic;
    
    class cVisualizerBasicSprite : public IListenerComponent
    {
    protected:
        IComponent        *mComponent;
        cVisualizerBasic *mVisualizer;
        
    public:
        cVisualizerBasicSprite( cVisualizerBasic *iVisualizer, IComponent *iComponent );
        
        // IListenerComponent methods
        virtual void OnComponentCreated  ( IComponent *iComponent ) {}
        virtual void OnComponentDestroyed( IComponent *iComponent );
        virtual void OnComponentInitialized( IComponent *iComponent ) {}
    };
}
