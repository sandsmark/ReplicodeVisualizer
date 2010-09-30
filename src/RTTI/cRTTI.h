/*
 *  cRTTI.h
 *  Visor
 *
 *  Created by mb on 5/29/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

namespace Visor
{
    template< typename iType >
    class cTypeNameHelper
    {
    public:
        static const char *GetName( void ) { return "Unknown type"; }
    };
    
    template< typename iType >
    class cRTTI
    {
    protected:
        const char *mName;
        
    public:
        cRTTI()
        {
            mName = cTypeNameHelper<iType>::GetName();
        }
        
        const char *GetName( void ) const { return mName; }
        iType *Create( void ) const { return new iType; }
    };

    template< typename iType >
    class cTypedClass
    {
    public:
        static const cRTTI< iType > &GetTypeInfo( void )
        {
            static cRTTI<iType> vRTTI;
            
            return vRTTI;
        }
        
        virtual const cRTTI< iType > &GetRTTI( void )
        {
            return iType::GetTypeInfo();
        }
    };
}

#define DECLARE_CLASS( iClass ) \
    class iClass; \
    template<>  \
    class cTypeNameHelper<iClass>  \
    {   \
    public: \
        static const char *GetName( void ) { return #iClass; }  \
    };   \
    class iClass : public cTypedClass< iClass >   

#define DECLARE_CLASS_PARENT( iClass, iParent ) \
    class iClass; \
    template<>  \
    class cTypeNameHelper<iClass>  \
    {   \
    public: \
        static const char *GetName( void ) { return #iClass; }  \
    };   \
    class iClass : public cTypedClass< iClass >, public iParent   
