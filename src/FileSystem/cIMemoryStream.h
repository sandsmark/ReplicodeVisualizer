/*
 *  cIMemoryStream.h
 *  Visor
 *
 *  Created by mb on 8/26/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

namespace Visor
{
    class cIMemoryStream : public std::basic_streambuf<unsigned char>
    {
    public:
        cIMemoryStream( const unsigned char *iData, size_t iSize )
        {
            setg( iData, iData, iData + Size );
        }
    };
}
