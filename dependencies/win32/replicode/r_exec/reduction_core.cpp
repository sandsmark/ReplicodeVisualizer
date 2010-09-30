//	reduction_core.cpp
//
//	Author: Eric Nivel
//
//	BSD license:
//	Copyright (c) 2008, Eric Nivel
//	All rights reserved.
//	Redistribution and use in source and binary forms, with or without
//	modification, are permitted provided that the following conditions are met:
//
//   - Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   - Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   - Neither the name of Eric Nivel nor the
//     names of their contributors may be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
//	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//	DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
//	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include	"reduction_core.h"
#include	"mem.h"
#include	"init.h"


namespace	r_exec{

	thread_ret thread_function_call	ReductionCore::Run(void	*args){

		ReductionCore	*_this=((ReductionCore	*)args);

		std::cout<<"Reduction Core created.\n";

		while(1){

			ReductionJob	j=_this->mem->popReductionJob();
			if(j.overlay->is_alive()){

				uint64	now=Now();
				if(j.deadline>now)	//	in case of an input-less pgm or an |pgm, signaling jobs will inject productions if any.
					j.overlay->reduce(j.input,_this->mem);
			}else
				j.overlay=NULL;
		}

		thread_ret_val(0);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ReductionCore::ReductionCore(_Mem	*m):Thread(),mem(m){
	}

	ReductionCore::~ReductionCore(){
	}
}