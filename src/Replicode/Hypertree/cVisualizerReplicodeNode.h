/*
 *  cVisualizerReplicodeNode.h
 *  Visor
 *
 *  Created by mb on 7/15/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Visualizer/Hypertree/cVisualizerHypertreeNode.h>

namespace Visor
{
    class cComponentReplicode;
    
    class cVisualizerReplicodeNode : public cVisualizerHypertreeNode
    {
        friend class cVisualizerReplicode;
        
    protected:
        Ogre::String mCaption;
        Ogre::MovableText *mTextCode;
        
        virtual void Init( void );
        
        virtual const Ogre::ColourValue GetLineColor( const cVisualizerHypertreeNode *iTarget );
        
    public:
        cVisualizerReplicodeNode( cVisualizerHypertree *iVisualizer, cComponentReplicode *iComponent );        
        
        void SetCodeReference( const Ogre::String &iCode );
    };
}
