/*
 *  IComponent.h
 *  Visor
 *
 *  Created by mb on 5/29/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <RTTI/cRTTI.h>
#include <Component/cComponentRegistry.h>
#include <Event/IEventDispatcher.h>
#include <vector>

namespace Visor
{
    class IVisualizer;
    class IListenerComponent;
    
//    class IComponent : public cTypedClass<IComponent>
//    DECLARE_CLASS( IComponent )
    DECLARE_CLASS_PARENT( IComponent, IEventDispatcher< IListenerComponent > )
    {
    protected:
        IVisualizer *mVisualizer;
        
        std::vector<const IComponent*> mChildren;
        
    public:
        IComponent();
        virtual ~IComponent();
        
        void SetVisualizer( IVisualizer *iVisualizer ) { mVisualizer = iVisualizer; }
        IVisualizer *GetVisualizer( void ) const { return mVisualizer; }
        
        void AddChild( IComponent *iComponent )
        {
            for ( size_t i=0; i<mChildren.size(); i++ )
            {
                if ( mChildren[i] == iComponent ) return;
            }            
            mChildren.push_back( iComponent );
        }
        size_t GetNumChildren( void ) const { return mChildren.size(); }
        const IComponent *GetChild( size_t iIndex ) const { return mChildren[ iIndex ]; }
        
        // Registry
        static cComponentRegistry &GetRegistry()
        {
            static cComponentRegistry vRegistry;            
            return vRegistry;
        }
    };
}

