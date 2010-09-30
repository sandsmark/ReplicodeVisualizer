/*
 *  cComponentReplicodeView.h
 *  Visor
 *
 *  Created by mb on 7/16/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Replicode/Component/cComponentReplicode.h>

namespace Visor
{
    class cComponentReplicodeView : public cComponentReplicode
    {
    public:
        cComponentReplicodeView( r_code::View *iCode );

        r_code::View *GetView( void ) const { return mView; }
    };
}
