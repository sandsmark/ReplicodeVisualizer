/*
 *  cListenerSourceView.h
 *  Visor
 *
 *  Created by mb on 7/21/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Event/IEventListener.h>

namespace Visor
{
    class IComponent;
    class cQSourceView;
    
    class IListenerSourceView : public IEventListener
    {
    public:
        virtual void OnSourceViewClosed( cQSourceView *iSourceView ) = 0;
    };
}
