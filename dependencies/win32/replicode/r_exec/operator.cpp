//	operator.cpp
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

#include	"operator.h"
#include	"mem.h"
#include	"init.h"
#include	"opcodes.h"
#include	"group.h"
#include	"../CoreLibrary/utils.h"
#include	"../r_code/utils.h"
#include	<math.h>


namespace	r_exec{

	r_code::vector<Operator>	Operator::Operators;

	void	Operator::Register(uint16	opcode,bool	(*o)(const	Context	&,uint16	&index)){

		if(Operators[opcode]._operator)
			Operators[opcode].setOverload(o);
		else
			Operators[opcode]=Operator(o);
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	now(const	Context	&context,uint16	&index){

		index=context.setTimestampResult(Now());
		return	true;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	equ(const	Context	&context,uint16	&index){

		bool	r=*context.getChild(1)==*context.getChild(2);
		index=context.setAtomicResult(Atom::Boolean(r));
		return	r;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	neq(const	Context	&context,uint16	&index){

		bool	r=*context.getChild(1)!=*context.getChild(2);
		index=context.setAtomicResult(Atom::Boolean(r));
		return	r;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	gtr(const	Context	&context,uint16	&index){

		Context	lhs=*context.getChild(1);
		Context	rhs=*context.getChild(2);

		if(lhs[0].isFloat()){

			if(rhs[0].isFloat()){

				bool	r=lhs[0].asFloat()>rhs[0].asFloat();
				index=context.setAtomicResult(Atom::Boolean(r));
				return	r;
			}
		}else	if(lhs[0].getDescriptor()==Atom::TIMESTAMP){

			if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				bool	r=Timestamp::Get(&lhs[0])>Timestamp::Get(&rhs[0]);
				index=context.setAtomicResult(Atom::Boolean(r));
				return	r;
			}
		}

		index=context.setAtomicResult(Atom::UndefinedBoolean());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	lsr(const	Context	&context,uint16	&index){

		Context	lhs=*context.getChild(1);
		Context	rhs=*context.getChild(2);

		if(lhs[0].isFloat()){

			if(rhs[0].isFloat()){

				bool	r=lhs[0].asFloat()<rhs[0].asFloat();
				index=context.setAtomicResult(Atom::Boolean(r));
				return	r;
			}
		}else	if(lhs[0].getDescriptor()==Atom::TIMESTAMP){

			if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				bool	r=Timestamp::Get(&lhs[0])<Timestamp::Get(&rhs[0]);
				index=context.setAtomicResult(Atom::Boolean(r));
				return	r;
			}
		}

		index=context.setAtomicResult(Atom::UndefinedBoolean());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	gte(const	Context	&context,uint16	&index){

		Context	lhs=*context.getChild(1);
		Context	rhs=*context.getChild(2);

		if(lhs[0].isFloat()){

			if(rhs[0].isFloat()){

				bool	r=lhs[0].asFloat()>=rhs[0].asFloat();
				index=context.setAtomicResult(Atom::Boolean(r));
				return	r;
			}
		}else	if(lhs[0].getDescriptor()==Atom::TIMESTAMP){

			if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				bool	r=Timestamp::Get(&lhs[0])>=Timestamp::Get(&rhs[0]);
				index=context.setAtomicResult(Atom::Boolean(r));
				return	r;
			}
		}

		index=context.setAtomicResult(Atom::UndefinedBoolean());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	lse(const	Context	&context,uint16	&index){

		Context	lhs=*context.getChild(1);
		Context	rhs=*context.getChild(2);

		if(lhs[0].isFloat()){

			if(rhs[0].isFloat()){

				bool	r=lhs[0].asFloat()<=rhs[0].asFloat();
				index=context.setAtomicResult(Atom::Boolean(r));
				return	r;
			}
		}else	if(lhs[0].getDescriptor()==Atom::TIMESTAMP){

			if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				bool	r=Timestamp::Get(&lhs[0])<=Timestamp::Get(&rhs[0]);
				index=context.setAtomicResult(Atom::Boolean(r));
				return	r;
			}
		}

		index=context.setAtomicResult(Atom::UndefinedBoolean());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	add(const	Context	&context,uint16	&index){

		Context	lhs=*context.getChild(1);
		Context	rhs=*context.getChild(2);

		if(lhs[0].isFloat()){

			if(rhs[0].isFloat()){

				if(lhs[0]==Atom::PlusInfinity()){

					index=context.setAtomicResult(Atom::PlusInfinity());
					return	true;
				}

				if(rhs[0]==Atom::PlusInfinity()){

					index=context.setAtomicResult(Atom::PlusInfinity());
					return	true;
				}

				index=context.setAtomicResult(Atom::Float(lhs[0].asFloat()+rhs[0].asFloat()));
				return	true;
			}
		}else	if(lhs[0].getDescriptor()==Atom::TIMESTAMP){

			if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				index=context.setTimestampResult(Timestamp::Get(&lhs[0])+Timestamp::Get(&rhs[0]));
				return	true;
			}
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	sub(const	Context	&context,uint16	&index){

		Context	lhs=*context.getChild(1);
		Context	rhs=*context.getChild(2);

		if(lhs[0].isFloat()){

			if(rhs[0].isFloat()){

				if(lhs[0]==Atom::PlusInfinity()){

					index=context.setAtomicResult(Atom::PlusInfinity());
					return	true;
				}

				if(rhs[0]==Atom::PlusInfinity()){

					index=context.setAtomicResult(Atom::Float(0));
					return	true;
				}

				index=context.setAtomicResult(Atom::Float(lhs[0].asFloat()-rhs[0].asFloat()));
				return	true;
			}
		}else	if(lhs[0].getDescriptor()==Atom::TIMESTAMP){

			if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				index=context.setTimestampResult(Timestamp::Get(&lhs[0])-Timestamp::Get(&rhs[0]));
				return	true;
			}
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	mul(const	Context	&context,uint16	&index){

		Context	lhs=*context.getChild(1);
		Context	rhs=*context.getChild(2);

		if(lhs[0].isFloat()){

			if(rhs[0].isFloat()){

				if(lhs[0]==Atom::PlusInfinity()){
					
					if(rhs[0]==Atom::PlusInfinity()){

						index=context.setAtomicResult(Atom::PlusInfinity());
						return	true;
					}

					if(rhs[0].asFloat()>0){

						index=context.setAtomicResult(Atom::PlusInfinity());
						return	true;
					}

					if(rhs[0].asFloat()<=0){

						index=context.setAtomicResult(Atom::Float(0));
						return	true;
					}
				}

				if(rhs[0]==Atom::PlusInfinity()){

					if(lhs[0].asFloat()>0){

						index=context.setAtomicResult(Atom::PlusInfinity());
						return	true;
					}

					if(lhs[0].asFloat()<=0){

						index=context.setAtomicResult(Atom::Float(0));
						return	true;
					}
				}

				index=context.setAtomicResult(Atom::Float(lhs[0].asFloat()*rhs[0].asFloat()));
				return	true;
			}else	if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				index=context.setAtomicResult(Atom::Float(Timestamp::Get(&rhs[0])*lhs[0].asFloat()));
				return	true;
			}
		}else	if(lhs[0].getDescriptor()==Atom::TIMESTAMP){

			if(rhs[0].isFloat()){

				index=context.setTimestampResult(Timestamp::Get(&lhs[0])*rhs[0].asFloat());
				return	true;
			}else	if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				index=context.setAtomicResult(Atom::Float(Timestamp::Get(&lhs[0])*Timestamp::Get(&lhs[0])));
				return	true;
			}
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	div(const	Context	&context,uint16	&index){

		Context	lhs=*context.getChild(1);
		Context	rhs=*context.getChild(2);

		if(lhs[0].isFloat()){

			if(rhs[0].isFloat()){
				
				if(rhs[0].asFloat()!=0){

					if(lhs[0]==Atom::PlusInfinity()){

						if(rhs[0]==Atom::PlusInfinity()){

							index=context.setAtomicResult(Atom::PlusInfinity());
							return	true;
						}

						if(rhs[0].asFloat()>0){

							index=context.setAtomicResult(Atom::PlusInfinity());
							return	true;
						}

						if(rhs[0].asFloat()<=0){

							index=context.setAtomicResult(Atom::Float(0));
							return	true;
						}
					}

					if(rhs[0]==Atom::PlusInfinity()){

						if(lhs[0].asFloat()>0){

							index=context.setAtomicResult(Atom::PlusInfinity());
							return	true;
						}

						if(lhs[0].asFloat()<=0){

							index=context.setAtomicResult(Atom::Float(0));
							return	true;
						}
					}

					index=context.setAtomicResult(Atom::Float(lhs[0].asFloat()/rhs[0].asFloat()));
					return	true;
				}
			}else	if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				float64	rhs_t=(float64)Timestamp::Get(&rhs[0]);
				if(rhs_t!=0){

					index=context.setAtomicResult(Atom::Float(lhs[0].asFloat()/rhs_t));
					return	true;
				}
			}
		}else	if(lhs[0].getDescriptor()==Atom::TIMESTAMP){

			if(rhs[0].isFloat()){
				
				if(rhs[0].asFloat()!=0){

					float64	lhs_t=(float64)Timestamp::Get(&lhs[0]);
					index=context.setTimestampResult(lhs_t/rhs[0].asFloat());
					return	true;
				}
			}else	if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				float64	rhs_t=(float64)Timestamp::Get(&rhs[0]);
				if(rhs_t!=0){

					float64	lhs_t=(float64)Timestamp::Get(&lhs[0]);
					index=context.setAtomicResult(Atom::Float(lhs_t/rhs_t));
					return	true;
				}
			}
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	dis(const	Context	&context,uint16	&index){	

		Context	lhs=*context.getChild(1);
		Context	rhs=*context.getChild(2);

		if(lhs[0].isFloat()){

			if(rhs[0].isFloat()){

				index=context.setAtomicResult(Atom::Float(abs(lhs[0].asFloat()-rhs[0].asFloat())));
				return	true;
			}
		}else	if(lhs[0].getDescriptor()==Atom::TIMESTAMP){

			if(rhs[0].getDescriptor()==Atom::TIMESTAMP){

				uint64	lhs_t=Timestamp::Get(&lhs[0]);
				uint64	rhs_t=Timestamp::Get(&rhs[0]);
				index=context.setTimestampResult(abs((float64)(lhs_t-rhs_t)));
				return	true;
			}
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	ln(const	Context	&context,uint16	&index){

		Context	arg=*context.getChild(1);
		
		if(arg[0].isFloat()){

			if(arg[0].asFloat()!=0){
				
				index=context.setAtomicResult(Atom::Float(::log(arg[0].asFloat())));
				return	true;
			}
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	exp(const	Context	&context,uint16	&index){

		Context	arg=*context.getChild(1);
		
		if(arg[0].isFloat()){

			index=context.setAtomicResult(Atom::Float(::exp(arg[0].asFloat())));
			return	true;
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	log(const	Context	&context,uint16	&index){

		Context	arg=*context.getChild(1);
		
		if(arg[0].isFloat()){

			if(arg[0].asFloat()!=0){
				
				index=context.setAtomicResult(Atom::Float(log10(arg[0].asFloat())));
				return	true;
			}
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	e10(const	Context	&context,uint16	&index){

		Context	arg=*context.getChild(1);
		
		if(arg[0].isFloat()){

			index=context.setAtomicResult(Atom::Float(pow(10,arg[0].asFloat())));
			return	true;
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	syn(const	Context	&context,uint16	&index){

		return	false;	//	TODO.
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	ins(const	Context	&context,uint16	&index){

		Context	object=*context.getChild(1);
		Context	args=*context.getChild(2);

		Code	*_object=object.getObject();
		if(_object->code(0).asOpcode()!=Opcodes::PGM	&&	_object->code(0).asOpcode()!=Opcodes::AntiPGM	&&		
			_object->code(0).asOpcode()!=Opcodes::Goal	&&	_object->code(0).asOpcode()!=Opcodes::AntiGoal){

			context.setAtomicResult(Atom::Nil());
			return	false;
		}

		if(_object	&&	args[0].getDescriptor()==Atom::SET){

			uint16	pattern_set_index=_object->code(PGM_TPL_ARGS).asIndex();	//	same index for goals.
			uint16	arg_count=args[0].getAtomCount();
			if(_object->code(pattern_set_index).getAtomCount()!=arg_count){

				context.setAtomicResult(Atom::Nil());
				return	false;
			}
			
			//	match args with the tpl patterns in _object.
			Context	pattern_set(_object,pattern_set_index);
			for(uint16	i=1;i<=arg_count;++i){

				Context	arg=*args.getChild(i);
				Context	skel=*(*pattern_set.getChild(i)).getChild(1);
				if(!skel.match(arg)){

					context.setAtomicResult(Atom::Nil());
					return	false;
				}
			}

			//	create an ipgm or an igol in the production array.
			Code	*instantiated_object=context.buildObject(_object->code(0));
			uint16	write_index=0;
			if(_object->code(0).asOpcode()==Opcodes::PGM	||	_object->code(0).asOpcode()==Opcodes::AntiPGM)			
				instantiated_object->code(write_index++)=Atom::InstantiatedProgram(Opcodes::IPGM,IPGM_IGOL_ARITY);
			else	if(_object->code(0).asOpcode()==Opcodes::Goal	||	_object->code(0).asOpcode()==Opcodes::AntiGoal)
				instantiated_object->code(write_index++)=Atom::Object(Opcodes::IGoal,IPGM_IGOL_ARITY);
			instantiated_object->code(write_index++)=Atom::RPointer(0);	//	points to the pgm/gol object.
			instantiated_object->code(write_index++)=Atom::IPointer(IPGM_IGOL_ARITY+1);
			instantiated_object->code(write_index++)=Atom::Float(1);	//	psln_thr.
			args.copy(instantiated_object,write_index);					//	writes after psln_thr.
			instantiated_object->set_reference(0,_object);
			context.setAtomicResult(Atom::ProductionPointer(context.addProduction(instantiated_object)));
			return	true;
		}
		
		context.setAtomicResult(Atom::Nil());
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	match(const	Context	&input,const	Context	&pattern,const	Context	&productions,std::vector<uint16>	&production_indices){

		Context	pattern_skeleton=*pattern.getChild(1);

		if(pattern[0].asOpcode()==Opcodes::PTN){
			
			if(!pattern_skeleton.match(input))
				return	false;

			//	patch the pattern with an iptr to the input, i.e. with input.index.
			pattern.patch_input_code(pattern.getIndex(),input.getIndex());

			//	evaluate the set of guards.
			Context	guard_set=*pattern.getChild(2);
			uint16	guard_count=guard_set.getChildrenCount();
			uint16	unused_index;
			for(uint16	i=1;i<=guard_count;++i){

				if(!(*guard_set.getChild(i)).evaluate_no_dereference(unused_index))	//	WARNING: no check for duplicates.
					return	false;
			}

			goto	build_productions;
			return	true;
		}

		if(pattern[0].asOpcode()==Opcodes::AntiPTN){
			
			if(pattern_skeleton.match(input))
				return	false;

			//	evaluate the set of guards.
			Context	guard_set=*pattern.getChild(2);
			uint16	guard_count=guard_set.getChildrenCount();
			uint16	unused_index;
			for(uint16	i=1;i<=guard_count;++i){

				if((*guard_set.getChild(i)).evaluate_no_dereference(unused_index))	//	WARNING: no check for duplicates.
					return	false;
			}

			goto	build_productions;
			return	true;
		}

		return	false;

build_productions:
		//	compute all productions for this input.
		uint16	production_count=productions.getChildrenCount();
		uint16	production_index;
		for(uint16	i=1;i<production_count;++i){

			(*productions.getChild(i)).evaluate_no_dereference(production_index);
			production_indices.push_back(production_index);
		}

		return	true;
	}

	void	reduce(const	Context	&context,const	Context	&input_set,const	Context	&section,std::vector<uint16>	&input_indices,std::vector<uint16>	&production_indices){

		Context	patterns=*section.getChild(1);
		if(patterns[0].getDescriptor()!=Atom::SET)
			return;

		Context	productions=*section.getChild(2);
		if(productions[0].getDescriptor()!=Atom::SET)
			return;

		uint16	pattern_count=patterns.getChildrenCount();
		if(!pattern_count)
			return;

		uint16	production_count=productions.getChildrenCount();
		if(!production_count)
			return;

		std::vector<uint16>::iterator	i;
		for(i=input_indices.begin();i!=input_indices.end();){	//	to be successful, at least one input must match all the patterns.

			bool	failure=false;
			for(uint16	j=0;j<pattern_count;++j){

				if(!match(*input_set.getChild(*i),patterns.getChild(j),productions,production_indices)){

					failure=true;
					context.rollback();
					break;
				}
			}

			if(failure)
				++i;
			else	//	all patterns matched: remove the input from the todo list.
				i=input_indices.erase(i);
		}
	}

	bool	red(const	Context	&context,uint16	&index){	//	reminder: all inputs have already been evaluated (but not the patterns).

		Context	input_set=*context.getChild(1);

		//	a section is a set of 2 sets: 8a) a set of patterns and, (b) a set of productions.
		Context	positive_section=*context.getChild(2);
		Context	negative_section=*context.getChild(3);

		std::vector<uint16>	input_indices;		//	todo list of inputs to match.
		for(uint16	i=1;i<=input_set.getChildrenCount();++i)
			input_indices.push_back(i);

		std::vector<uint16>	production_indices;	//	list of productions built upon successful matches.

		if(input_set[0].getDescriptor()!=Atom::SET	||
			input_set[0].getDescriptor()!=Atom::S_SET	||
			positive_section[0].getDescriptor()!=Atom::SET	||
			negative_section[0].getDescriptor()!=Atom::SET)
			goto	failure;

		uint16	input_count=input_set.getChildrenCount();
		if(!input_count)
			goto	failure;

		reduce(context,input_set,positive_section,input_indices,production_indices);	//	input_indices now filled only with the inputs that did not match all positive patterns.
		reduce(context,input_set,negative_section,input_indices,production_indices);	//	input_indices now filled only with the inputs that did not match all positive nor all negative patterns.
		if(production_indices.size()){

			index=context.setCompoundResultHead(Atom::Set(production_indices.size()));
			for(uint16	i=0;i<production_indices.size();++i)	//	fill the result set with iptrs to productions.
				context.addCompoundResultPart(Atom::IPointer(i));

			context.commit();
			return	true;
		}
failure:
		index=context.setCompoundResultHead(Atom::Set(0));
		return	false;
	}

	////////////////////////////////////////////////////////////////////////////////
	
	bool	com(const	Context	&context,uint16	&index){

		return	false;	//	TODO.
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	spl(const	Context	&context,uint16	&index){

		return	false;	//	TODO.
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	mrg(const	Context	&context,uint16	&index){

		return	false;	//	TODO.
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	ptc(const	Context	&context,uint16	&index){

		return	false;	//	TODO.
	}

	////////////////////////////////////////////////////////////////////////////////

	bool	fvw(const	Context	&context,uint16	&index){

		Context	object=*context.getChild(1);
		Context	group=*context.getChild(2);

		Code	*_object=object.getObject();
		Group	*_group=(Group	*)group.getObject();
		if(!_object	||	!_group){

			context.setAtomicResult(Atom::Nil());
			return	false;
		}

		View	*v=(View	*)_object->find_view(_group,true);	//	returns a copy of the view, if any.
		if(v){	//	copy the view in the value array: code on VIEW_CODE_MAX_SIZE followed by 2 atoms holding raw pointers to grp and org.

			index=context.setCompoundResultHead(v->code(0));
			for(uint16	i=1;i<VIEW_CODE_MAX_SIZE;++i)
				context.addCompoundResultPart(v->code(i));
			context.addCompoundResultPart(Atom((uint32)v->references[0]));
			context.addCompoundResultPart(Atom((uint32)v->references[1]));
			delete	v;
			return	true;
		}

		index=context.setAtomicResult(Atom::Nil());
		return	false;
	}
}