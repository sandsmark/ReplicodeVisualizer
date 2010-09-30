//	loader_module.cpp
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

#include "loader_module.h"

#include	<r_exec/init.h>


LOAD_MODULE(Loader)

void	Loader::initialize(){

	image=NULL;
}

void	Loader::finalize(){
}

void	Loader::compile(const	std::string	&filename){

	std::string	error;
	if(r_exec::Compile(filename.c_str(),error)){

		image=r_exec::Seed.serialize<ImageMessage>();
		//image->trace();
	}

	//	TMP: generate an image for Tamas.
	
	ofstream	output("c:/work/replicode/test/test.visualizer.replicode.image",ios::binary|ios::out);
	ImageMessage::Write(image,output);
	output.close();

	ifstream	input("c:/work/replicode/test/test.visualizer.replicode.image",ios::binary|ios::in);
	if(!input.good())
		return;

	ImageMessage	*img=(ImageMessage	*)ImageMessage::Read(input);
	input.close();

	img->trace();

	r_code::vector<Code	*>	objects;
	r_comp::Image			*i=new	r_comp::Image();
	i->load(img);
	i->getObjects<LObject>(objects);
	delete	i;

	delete	img;
}