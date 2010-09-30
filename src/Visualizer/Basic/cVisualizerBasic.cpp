/*
 *  cVisualizerBasic.cpp
 *  Visor
 *
 *  Created by mb on 5/29/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cVisualizerBasic.h"
#include <Visualizer/Basic/cVisualizerBasicSprite.h>
using namespace Visor;

/**********************************************************************/
cVisualizerBasic::cVisualizerBasic( Ogre::SceneManager *iScene )
: mScene( iScene )
/**********************************************************************/
{
//    cDebug::Notify( "Basic visualizer created" );
    
    mBillboards = mScene->createBillboardSet( "Components" );
    mBillboards->setMaterialName( "ComponentMaterial" );
    mBillboards->setBillboardType( Ogre::BBT_POINT );
    mBillboards->setDefaultDimensions( 1, 1 );
    mBillboards->setPointRenderingEnabled( true );
    mBillboards->setAutoextend( true );
    mBillboards->setAutoUpdate( true );
    mBillboards->setVisible( true );
    
    mScene->getRootSceneNode()->attachObject( mBillboards );
}

/**********************************************************************/
void cVisualizerBasic::OnComponentCreated( IComponent *iComponent )
/**********************************************************************/
{
    printf( "[cVisualizerBasic::OnComponentCreated] 0x%08x\n", (unsigned int)iComponent );
    new cVisualizerBasicSprite( this, iComponent );
}

/**********************************************************************/
void cVisualizerBasic::OnComponentDestroyed( IComponent *iComponent )
/**********************************************************************/
{
    printf( "[cVisualizerBasic::OnComponentDestroyed] 0x%08x\n", (unsigned int)iComponent );
}
