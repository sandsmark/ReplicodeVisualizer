//	opcodes.cpp
//
//	Author: Eric Nivel
//
//	BSD license:
//	Copyright (c) 2010, Eric Nivel
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

#include	"opcodes.h"


namespace	r_exec{

	uint16	Opcodes::View;
	uint16	Opcodes::ReactView;
	uint16	Opcodes::GroupView;

	uint16	Opcodes::Entity;

	uint16	Opcodes::Group;

	uint16	Opcodes::PTN;
	uint16	Opcodes::AntiPTN;

	uint16	Opcodes::IPGM;
	uint16	Opcodes::PGM;
	uint16	Opcodes::AntiPGM;

	uint16	Opcodes::IGoal;
	uint16	Opcodes::Goal;
	uint16	Opcodes::AntiGoal;

	uint16	Opcodes::Cmd;

	uint16	Opcodes::MkRdx;
	uint16	Opcodes::MkAntiRdx;

	uint16	Opcodes::MkNew;

	uint16	Opcodes::MkLowRes;
	uint16	Opcodes::MkLowSln;
	uint16	Opcodes::MkHighSln;
	uint16	Opcodes::MkLowAct;
	uint16	Opcodes::MkHighAct;
	uint16	Opcodes::MkSlnChg;
	uint16	Opcodes::MkActChg;

	uint16	Opcodes::Inject;
	uint16	Opcodes::Eject;
	uint16	Opcodes::Mod;
	uint16	Opcodes::Set;
	uint16	Opcodes::NewClass;
	uint16	Opcodes::DelClass;
	uint16	Opcodes::LDC;
	uint16	Opcodes::Swap;
	uint16	Opcodes::NewDev;
	uint16	Opcodes::DelDev;
	uint16	Opcodes::Suspend;
	uint16	Opcodes::Resume;
	uint16	Opcodes::Stop;
}