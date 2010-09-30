//	time_job.cpp
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

#include	"time_job.h"
#include	"mem.h"


namespace	r_exec{

	bool	_TimeJob::is_alive()	const{

		return	true;
	}

	////////////////////////////////////////////////////////////

	TimeJob::TimeJob(){
	}

	TimeJob::TimeJob(_TimeJob	*j,uint64	ijt):target_time(ijt){

		job=j;
	}

	bool	TimeJob::is_alive()	const{

		return	job->is_alive();
	}

	////////////////////////////////////////////////////////////

	UpdateJob::UpdateJob(Group	*g){

		group=g;
	}

	void	UpdateJob::update(_Mem	*m){

		m->update(this);
	}

	////////////////////////////////////////////////////////////

	SignalingJob::SignalingJob(IPGMController	*o){

		controller=o;
	}

	bool	SignalingJob::is_alive()	const{

		return	controller->is_alive();
	}

	////////////////////////////////////////////////////////////

	AntiPGMSignalingJob::AntiPGMSignalingJob(IPGMController	*o):SignalingJob(o){
	}

	void	AntiPGMSignalingJob::update(_Mem	*m){

		m->update(this);
	}

	////////////////////////////////////////////////////////////

	InputLessPGMSignalingJob::InputLessPGMSignalingJob(IPGMController	*o):SignalingJob(o){
	}

	void	InputLessPGMSignalingJob::update(_Mem	*m){

		m->update(this);
	}

	////////////////////////////////////////////////////////////

	InjectionJob::InjectionJob(View	*v){

		view=v;
	}

	void	InjectionJob::update(_Mem	*m){

		m->update(this);
	}

	////////////////////////////////////////////////////////////

	EInjectionJob::EInjectionJob(View	*v){

		view=v;
	}

	void	EInjectionJob::update(_Mem	*m){

		m->update(this);
	}

	////////////////////////////////////////////////////////////

	GInjectionJob::GInjectionJob(View	*v,Group	*g,Group	*h){

		view=v;
		group=g;
		host=h;
	}

	void	GInjectionJob::update(_Mem	*m){

		m->update(this);
	}

	////////////////////////////////////////////////////////////

	SaliencyPropagationJob::SaliencyPropagationJob(Code	*o,float32	sln_change,float32	source_sln_thr):sln_change(sln_change),source_sln_thr(source_sln_thr){

		object=o;
	}
	
	void	SaliencyPropagationJob::update(_Mem	*m){

		m->update(this);
	}
}