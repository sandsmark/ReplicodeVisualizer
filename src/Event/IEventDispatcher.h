/*
 *  IEventDispatcher.h
 *  Visor
 *
 *  Created by mb on 5/30/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <vector>
#include <Event/IEventListener.h>

namespace Visor
{
    class IComponent;
    
    template< typename iType >
    class IEventDispatcher
    {
    protected:
        std::vector<iType*> mListeners;
        
        template< typename iParamType1 >
        class cTypeWrapper1
        {
        public:
            typedef void(iType::*tFunction)( iParamType1 );
        };
        
    public:
        void AddListener( iType &iListener ) { mListeners.push_back( &iListener ); }
        
#if 0
//        template< void*(iType::*iFunction)(IComponent*), typename iType1 >
        void Dispatch( iType1 vParam1 )
        {
            for ( size_t i=0; i<mListeners.size(); i++ )
            {
                iType *vListener = mListeners[i];
//                cDelegateWrapper<i::Action( vParam1 );
//                (vListener->*iFunction)( vParam1 );
            }
        }
#endif
        
        template< typename iParamType1 >
        void Dispatch( typename cTypeWrapper1<iParamType1>::tFunction iFunction, iParamType1 vParam1 )
        {
            for ( size_t i=0; i<mListeners.size(); i++ )
            {
                iType *vListener = mListeners[i];
                (vListener->*iFunction)( vParam1 );
            }
        }
        
    };
}
