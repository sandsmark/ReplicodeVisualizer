/*
 *  cComponentRegistry.cpp
 *  Visor
 *
 *  Created by mb on 5/31/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cComponentRegistry.h"
#include <Component/Events/IListenerComponent.h>
#include <Event/IEventDispatcher.h>
using namespace Visor;

/**********************************************************************/
void cComponentRegistry::RegisterComponent( IComponent *iComponent )
/**********************************************************************/
{
    Dispatch( &IListenerComponent::OnComponentCreated, iComponent );
}

/**********************************************************************/
void cComponentRegistry::DeregisterComponent( IComponent *iComponent )
/**********************************************************************/
{
    Dispatch( &IListenerComponent::OnComponentDestroyed, iComponent );
}

/**********************************************************************/
void cComponentRegistry::OnComponentInitialized( IComponent *iComponent )
/**********************************************************************/
{
    Dispatch( &IListenerComponent::OnComponentInitialized, iComponent );
}

