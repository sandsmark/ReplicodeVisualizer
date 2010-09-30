//	view.h
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

#ifndef	view_h
#define	view_h

#include	"../r_code/object.h"
#include	"pgm_overlay.h"
#include	"dll.h"


namespace	r_exec{

	class	Group;
	class	LObject;

	//	OID is hidden at _code[VIEW_OID].
	//	Shared resources:
	//		none: all mod/set operations are pushed on the group and executed at update time.
	class	r_exec_dll	View:
	public	r_code::View{
	private:
		static	uint32	LastOID;
		static	uint32	GetOID();

		//	Ctrl values.
		uint32	sln_changes;
		float32	acc_sln;
		uint32	act_vis_changes;
		float32	acc_act_vis;
		uint32	res_changes;
		float32	acc_res;
		void	reset_ctrl_values();

		//	Monitoring
		float32	initial_sln;
		float32	initial_act;
	protected:
		void	reset_init_sln();
		void	reset_init_act();
	public:
		static	uint16	ViewOpcode;

		P<IPGMController>	controller;	//	built upon injection of the view (if the object is an ipgm).

		static	float32	MorphValue(float32	value,float32	source_thr,float32	destination_thr);
		static	float32	MorphChange(float32	change,float32	source_thr,float32	destination_thr);

		uint32	periods_at_low_sln;
		uint32	periods_at_high_sln;
		uint32	periods_at_low_act;
		uint32	periods_at_high_act;

		View();
		View(r_code::SysView	*source,r_code::Code	*object);
		View(View	*view,Group	*group);	//	copy the view and assigns it to the group (used for cov); morph ctrl values.
		View(const	View	*view);			//	simple copy.
		~View();

		void	set_object(r_code::Code	*object);

		uint32	getOID()	const;

		virtual	bool	isNotification()	const;

		Group	*get_host();

		uint64	get_ijt()	const;

		float32	get_res();
		float32	get_sln();
		float32	get_act_vis();
		bool	get_cov();

		void	mod_res(float32	value);
		void	set_res(float32	value);
		void	mod_sln(float32	value);
		void	set_sln(float32	value);
		void	mod_act_vis(float32	value);
		void	set_act_vis(float32	value);

		float32	update_res();
		float32	update_sln(float32	low,float32	high);
		float32	update_act(float32	low,float32	high);
		float32	update_vis();

		float32	update_sln_delta();
		float32	update_act_delta();

		//	Target res, sln, act, vis.
		void	mod(uint16	member_index,float32	value);
		void	set(uint16	member_index,float32	value);
	};

	class	r_exec_dll	NotificationView:
	public	View{
	public:
		NotificationView(Group	*origin,Group	*destination,Code	*marker);	//	res=1, sln=1.

		bool	isNotification()	const;
	};
}


#include	"view.inline.cpp"


#endif