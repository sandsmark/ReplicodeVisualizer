//	time_job.h
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

#ifndef	time_job_h
#define	time_job_h

#include	"group.h"
#include	"pgm_overlay.h"


namespace	r_exec{

	class	r_exec_dll	_TimeJob:
	public	_Object{
	public:
		virtual	void	update(_Mem	*m)=0;
		virtual	bool	is_alive()	const;
	};
	
	class	r_exec_dll	TimeJob{
	public:
		int64		target_time;	//	0 means ASAP.
		P<_TimeJob>	job;
		TimeJob();
		TimeJob(_TimeJob	*j,uint64	ijt);
		bool	is_alive()	const;
	};

	class	r_exec_dll	UpdateJob:
	public	_TimeJob{
	public:
		P<Group>	group;
		UpdateJob(Group	*g);
		void	update(_Mem	*m);
	};

	class	r_exec_dll	SignalingJob:
	public	_TimeJob{
	protected:
		SignalingJob(IPGMController	*o);
	public:
		P<IPGMController>	controller;
		bool	is_alive()	const;
	};

	class	r_exec_dll	AntiPGMSignalingJob:
	public	SignalingJob{
	public:
		AntiPGMSignalingJob(IPGMController	*o);
		void	update(_Mem	*m);
	};

	class	r_exec_dll	InputLessPGMSignalingJob:
	public	SignalingJob{
	public:
		InputLessPGMSignalingJob(IPGMController	*o);
		void	update(_Mem	*m);
	};

	class	r_exec_dll	InjectionJob:
	public	_TimeJob{
	public:
		P<View>	view;
		InjectionJob(View	*v);
		void	update(_Mem	*m);
	};

	class	r_exec_dll	EInjectionJob:
	public	_TimeJob{
	public:
		P<View>	view;
		EInjectionJob(View	*v);
		void	update(_Mem	*m);
	};

	class	r_exec_dll	GInjectionJob:
	public	_TimeJob{
	public:
		P<View>		view;
		P<Group>	group;
		P<Group>	host;
		GInjectionJob(View	*v,Group	*g,Group	*h);
		void	update(_Mem	*m);
	};

	class	r_exec_dll	SaliencyPropagationJob:
	public	_TimeJob{
	public:
		P<Code>	object;
		float32		sln_change;
		float32		source_sln_thr;
		SaliencyPropagationJob(Code	*o,float32	sln_change,float32	source_sln_thr);
		void	update(_Mem	*m);
	};
}


#endif