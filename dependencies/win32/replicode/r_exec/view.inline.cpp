//	view.inline.cpp
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

#include	"../r_code/utils.h"


namespace	r_exec{

	inline	View::View():r_code::View(){

		_code[VIEW_OID].atom=GetOID();

		reset_ctrl_values();
	}

	inline	View::View(r_code::SysView	*source,r_code::Code	*object):r_code::View(source,object){

		_code[VIEW_OID].atom=GetOID();

		reset_ctrl_values();
		reset_init_sln();
		reset_init_act();
	}

	inline	View::View(const	View	*view):r_code::View(){

		object=view->object;
		memcpy(_code,view->_code,VIEW_CODE_MAX_SIZE*sizeof(Atom)+2*sizeof(r_code::Object	*));	//	reference_set is contiguous to code; memcpy in one go.

		reset_ctrl_values();
		reset_init_sln();
		reset_init_act();
	}

	inline	View::~View(){
	}

	inline	uint32	View::getOID()	const{

		return	_code[VIEW_OID].atom;
	}

	inline	bool	View::isNotification()	const{

		return	false;
	}

	inline	Group	*View::get_host(){

		uint32	host_reference=code(VIEW_HOST).asIndex();
		return	(Group	*)references[host_reference];
	}

	inline	uint64	View::get_ijt()	const{

		return	Timestamp::Get(_code+_code[VIEW_IJT].asIndex());
	}

	inline	float32	View::get_res(){

		return	code(VIEW_RES).asFloat();
	}

	inline	float32	View::get_sln(){

		return	code(VIEW_SLN).asFloat();
	}

	inline	float32	View::get_act_vis(){

		return	code(VIEW_ACT_VIS).asFloat();
	}

	inline	bool	View::get_cov(){

		return	code(GRP_VIEW_COV).asFloat()==1;
	}

	inline	void	View::mod_res(float32	value){

		if(code(VIEW_RES)==Atom::PlusInfinity())
			return;
		acc_res+=value;
		++res_changes;
	}

	inline	void	View::set_res(float32	value){

		if(code(VIEW_RES)==Atom::PlusInfinity())
			return;
		acc_res+=value-get_res();
		++res_changes;
	}

	inline	void	View::mod_sln(float32	value){

		acc_sln+=value;
		++sln_changes;
	}

	inline	void	View::set_sln(float32	value){

		acc_sln+=value-get_sln();
		++sln_changes;
	}

	inline	void	View::mod_act_vis(float32	value){

		acc_act_vis+=value;
		++act_vis_changes;
	}

	inline	void	View::set_act_vis(float32	value){

		acc_act_vis+=value-get_act_vis();
		++act_vis_changes;
	}

	inline	float32	View::update_sln_delta(){

		float32	delta=get_sln()-initial_sln;
		initial_sln=get_sln();
		return	delta;
	}

	inline	float32	View::update_act_delta(){

		float32	delta=get_act_vis()-initial_act;
		initial_act=get_act_vis();
		return	delta;
	}

	inline	void	View::mod(uint16	member_index,float32	value){

		switch(member_index){
		case	VIEW_SLN:
			mod_sln(value);
			break;
		case	VIEW_RES:
			mod_res(value);
			break;
		case	VIEW_ACT_VIS:
			mod_act_vis(value);
			break;
		}
	}

	inline	void	View::set(uint16	member_index,float32	value){

		switch(member_index){
		case	VIEW_SLN:
			set_sln(value);
			break;
		case	VIEW_RES:
			set_res(value);
			break;
		case	VIEW_ACT_VIS:
			set_act_vis(value);
			break;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline	bool	NotificationView::isNotification()	const{

		return	true;
	}
}