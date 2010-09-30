/*
 *  cVisualizerBasicSprite.cpp
 *  Visor
 *
 *  Created by mb on 5/31/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cVisualizerBasicSprite.h"
#include <Visualizer/Basic/cVisualizerBasic.h>
using namespace Visor;

/**********************************************************************/
cVisualizerBasicSprite::cVisualizerBasicSprite( cVisualizerBasic *iVisualizer, IComponent *iComponent )
/**********************************************************************/
: mComponent( iComponent ), mVisualizer( iVisualizer )
{
//    cDebug::Notify( "Basic visualizer created for component" );
    
    Ogre::BillboardSet *vBillboards = mVisualizer->GetBillboards();
    
    Ogre::Vector3 vPosition = Ogre::Vector3( Ogre::Math::RangeRandom( -5, 5 ), Ogre::Math::RangeRandom( -5, 5 ), Ogre::Math::RangeRandom( -5, 5 ) );
    Ogre::Billboard *vBillboard = vBillboards->createBillboard( vPosition );
    vBillboard->setColour( Ogre::ColourValue( Ogre::Math::RangeRandom( 0, 1 ), Ogre::Math::RangeRandom( 0, 1 ), Ogre::Math::RangeRandom( 0, 1 ), Ogre::Math::RangeRandom( 0, 1 ) ) );
    float vSize = Ogre::Math::RangeRandom( 1, 5 );
    vBillboard->setDimensions( vSize, vSize );
}

/**********************************************************************/
void cVisualizerBasicSprite::OnComponentDestroyed( IComponent *iComponent )
/**********************************************************************/
{
}
