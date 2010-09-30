/*
 *  cQContextMenu.cpp
 *  Visor
 *
 *  Created by mb on 6/27/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cQContextMenu.h"
#include <Replicode/Hypertree/cVisualizerReplicode.h>
#include <Replicode/Hypertree/cVisualizerReplicodeNode.h>
#include <App/GUI/cQNameDialog.h>
#include <App/GUI/cQVisor.h>
#include <App/GUI/cQMainWindow.h>
#include <Replicode/Component/cComponentReplicode.h>
using namespace Visor;

/**********************************************************************/
cQContextMenu::cQContextMenu( cQVisor *iVisor, cVisualizerHypertreeNode *iNode )
: QMenu( (QWidget*)iVisor ), mNode( iNode ), mVisor( iVisor )
/**********************************************************************/
{
    if ( mNode )
    {
        addAction( "View source", this, SLOT( OnViewSource() ) );
        addAction( "Set name", this, SLOT( OnSetName() ) );
    }

    addAction( "Find", this, SLOT( OnFind() ) );
    addAction( "Reset Camera", this, SLOT( OnResetCamera() ) );
}

/**********************************************************************/
void cQContextMenu::OnViewSource()
/**********************************************************************/
{
//    mVisor->GetMainWindow().ShowSource( ((cComponentReplicode*)mNode->GetComponent())->GetSource().c_str(), mNode->GetName().c_str() );
    mVisor->GetMainWindow().ShowSource( ((cComponentReplicode*)mNode->GetComponent()) );
}

/**********************************************************************/
void cQContextMenu::OnSetName()
/**********************************************************************/
{
    cQNameDialog vDialog( mNode->GetName().c_str() );
    if ( !vDialog.exec() ) return;
    
	mNode->SetName( Ogre::String( vDialog.GetName().toLocal8Bit() ) );
}

/**********************************************************************/
void cQContextMenu::OnFind()
/**********************************************************************/
{
    cQNameDialog vDialog( "" );
    if ( !vDialog.exec() ) return;
    
    cVisualizerReplicodeNode *vNode = (cVisualizerReplicodeNode*)mVisor->GetVisualizer()->FindNodeByName( Ogre::String( vDialog.GetName().toLocal8Bit() ) );
    if ( vNode )
    {
        mVisor->TrackNode( vNode );
    }
}

/**********************************************************************/
void cQContextMenu::OnResetCamera()
/**********************************************************************/
{
    mVisor->TrackNode( (cVisualizerReplicodeNode*)mVisor->GetVisualizer()->FindNodeByComponent( mVisor->GetVisualizer()->GetFocus() ) );
}

