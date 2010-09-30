/*
 *  IComponent.cpp
 *  Visor
 *
 *  Created by mb on 5/31/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "IComponent.h"
#include <Component/cComponentRegistry.h>
#include <Component/Events/IListenerComponent.h>
using namespace Visor;

/**********************************************************************/
IComponent::IComponent()
/**********************************************************************/
{
    GetRegistry().RegisterComponent( this );
}

/**********************************************************************/
IComponent::~IComponent()
/**********************************************************************/
{
    GetRegistry().DeregisterComponent( this );
    Dispatch( &IListenerComponent::OnComponentDestroyed, this );
}
