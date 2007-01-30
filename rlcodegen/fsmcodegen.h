/*
 *  Copyright 2001-2006 Adrian Thurston <thurston@cs.queensu.ca>
 *            2004 Eric Ocean <eric.ocean@ampede.com>
 *            2005 Alan West <alan@alanz.com>
 */

/*  This file is part of Ragel.
 *
 *  Ragel is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 * 
 *  Ragel is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with Ragel; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef _FSMCODEGEN_H
#define _FSMCODEGEN_H

#include <iostream>
#include <string>
#include <stdio.h>
#include "common.h"

using std::string;
using std::ostream;

/* Integer array line length. */
#define IALL 8

/* Forwards. */
struct RedFsmAp;
struct RedStateAp;
struct CodeGenData;
struct Action;
struct NameInst;
struct InlineItem;
struct InlineList;
struct RedAction;
struct LongestMatch;
struct LongestMatchPart;

inline string itoa( int i )
{
	char buf[16];
	sprintf( buf, "%i", i );
	return buf;
}

/*
 * class FsmCodeGen
 */
class FsmCodeGen
{
public:
	FsmCodeGen( ostream &out );
	virtual ~FsmCodeGen() {}

	virtual void writeOutData() = 0;
	virtual void writeOutInit();
	virtual void writeOutExec() = 0;
	virtual void writeOutEOF() = 0;
	
	/* Gather various info on the machine. */
	void analyzeAction( Action *act, InlineList *inlineList );
	void analyzeActionList( RedAction *redAct, InlineList *inlineList );
	void analyzeMachine();

protected:
	friend struct CodeGenData;

	string FSM_NAME();
	string START_STATE_ID();
	ostream &ACTIONS_ARRAY();
	string GET_WIDE_KEY();
	string GET_WIDE_KEY( RedStateAp *state );
	string TABS( int level );
	string KEY( Key key );
	string LDIR_PATH( char *path );
	void ACTION( ostream &ret, Action *action, int targState, bool inFinish );
	void CONDITION( ostream &ret, Action *condition );
	string ALPH_TYPE();
	string WIDE_ALPH_TYPE();
	string ARRAY_TYPE( unsigned long maxVal );

	virtual string ARR_OFF( string ptr, string offset ) = 0;
	virtual string CAST( string type ) = 0;
	virtual string UINT() = 0;
	virtual string NULL_ITEM() = 0;
	virtual string POINTER() = 0;
	virtual string GET_KEY();
	virtual ostream &SWITCH_DEFAULT() = 0;

	string P() { return "p"; }
	string PE() { return "pe"; }

	string ACCESS();
	string CS();
	string STACK() { return ACCESS() + "stack"; }
	string TOP() { return ACCESS() + "top"; }
	string TOKSTART() { return ACCESS() + "tokstart"; }
	string TOKEND() { return ACCESS() + "tokend"; }
	string ACT() { return ACCESS() + "act"; }

	string DATA_PREFIX();
	string PM() { return "_" + DATA_PREFIX() + "partition_map"; }
	string C() { return "_" + DATA_PREFIX() + "cond_spaces"; }
	string CK() { return "_" + DATA_PREFIX() + "cond_keys"; }
	string K() { return "_" + DATA_PREFIX() + "trans_keys"; }
	string I() { return "_" + DATA_PREFIX() + "indicies"; }
	string CO() { return "_" + DATA_PREFIX() + "cond_offsets"; }
	string KO() { return "_" + DATA_PREFIX() + "key_offsets"; }
	string IO() { return "_" + DATA_PREFIX() + "index_offsets"; }
	string CL() { return "_" + DATA_PREFIX() + "cond_lengths"; }
	string SL() { return "_" + DATA_PREFIX() + "single_lengths"; }
	string RL() { return "_" + DATA_PREFIX() + "range_lengths"; }
	string A() { return "_" + DATA_PREFIX() + "actions"; }
	string TA() { return "_" + DATA_PREFIX() + "trans_actions_wi"; }
	string TT() { return "_" + DATA_PREFIX() + "trans_targs_wi"; }
	string TSA() { return "_" + DATA_PREFIX() + "to_state_actions"; }
	string FSA() { return "_" + DATA_PREFIX() + "from_state_actions"; }
	string EA() { return "_" + DATA_PREFIX() + "eof_actions"; }
	string SP() { return "_" + DATA_PREFIX() + "key_spans"; }
	string CSP() { return "_" + DATA_PREFIX() + "cond_key_spans"; }
	string START() { return DATA_PREFIX() + "start"; }
	string ERROR() { return DATA_PREFIX() + "error"; }
	string FIRST_FINAL() { return DATA_PREFIX() + "first_final"; }
	string CTXDATA() { return DATA_PREFIX() + "ctxdata"; }

	void INLINE_LIST( ostream &ret, InlineList *inlineList, int targState, bool inFinish );
	virtual void GOTO( ostream &ret, int gotoDest, bool inFinish ) = 0;
	virtual void CALL( ostream &ret, int callDest, int targState, bool inFinish ) = 0;
	virtual void NEXT( ostream &ret, int nextDest, bool inFinish ) = 0;
	virtual void GOTO_EXPR( ostream &ret, InlineItem *ilItem, bool inFinish ) = 0;
	virtual void NEXT_EXPR( ostream &ret, InlineItem *ilItem, bool inFinish ) = 0;
	virtual void CALL_EXPR( ostream &ret, InlineItem *ilItem, 
			int targState, bool inFinish ) = 0;
	virtual void RET( ostream &ret, bool inFinish ) = 0;
	virtual void BREAK( ostream &ret, int targState ) = 0;
	virtual void CURS( ostream &ret, bool inFinish ) = 0;
	virtual void TARGS( ostream &ret, bool inFinish, int targState ) = 0;
	void EXEC( ostream &ret, InlineItem *item, int targState, int inFinish );
	void EXECTE( ostream &ret, InlineItem *item, int targState, int inFinish );
	void LM_SWITCH( ostream &ret, InlineItem *item, int targState, int inFinish );
	void SET_ACT( ostream &ret, InlineItem *item );
	void INIT_TOKSTART( ostream &ret, InlineItem *item );
	void INIT_ACT( ostream &ret, InlineItem *item );
	void SET_TOKSTART( ostream &ret, InlineItem *item );
	void SET_TOKEND( ostream &ret, InlineItem *item );
	void GET_TOKEND( ostream &ret, InlineItem *item );
	void SUB_ACTION( ostream &ret, InlineItem *item, 
			int targState, bool inFinish );

	string ERROR_STATE();
	string FIRST_FINAL_STATE();

	virtual string PTR_CONST() = 0;
	virtual ostream &OPEN_ARRAY( string type, string name ) = 0;
	virtual ostream &CLOSE_ARRAY() = 0;
	virtual ostream &STATIC_VAR( string type, string name ) = 0;

	virtual string CTRL_FLOW() = 0;

	unsigned int arrayTypeSize( unsigned long maxVal );

	bool anyActions();
	bool anyToStateActions()        { return bAnyToStateActions; }
	bool anyFromStateActions()      { return bAnyFromStateActions; }
	bool anyRegActions()            { return bAnyRegActions; }
	bool anyEofActions()            { return bAnyEofActions; }
	bool anyActionGotos()           { return bAnyActionGotos; }
	bool anyActionCalls()           { return bAnyActionCalls; }
	bool anyActionRets()            { return bAnyActionRets; }
	bool anyRegActionRets()         { return bAnyRegActionRets; }
	bool anyRegActionByValControl() { return bAnyRegActionByValControl; }
	bool anyRegNextStmt()           { return bAnyRegNextStmt; }
	bool anyRegCurStateRef()        { return bAnyRegCurStateRef; }
	bool anyRegBreak()              { return bAnyRegBreak; }
	bool anyLmSwitchError()         { return bAnyLmSwitchError; }
	bool anyConditions()            { return bAnyConditions; }

	/* Set up labelNeeded flag for each state. Differs for each goto style so
	 * is virtual. */
	virtual void setLabelsNeeded() {}

	/* Determine if we should use indicies. */
	virtual void calcIndexSize() {}

	void findFinalActionRefs();
	void assignActionIds();
	void setValueLimits();

	/* Are there any regular transition functions, any out transition functions. */
	char *fsmName;
	CodeGenData *cgd;
	RedFsmAp *redFsm;

	bool outLabelUsed;
	bool againLabelUsed;

protected:
	ostream &out;

	bool bAnyToStateActions;
	bool bAnyFromStateActions;
	bool bAnyRegActions;
	bool bAnyEofActions;
	bool bAnyActionGotos;
	bool bAnyActionCalls;
	bool bAnyActionRets;
	bool bAnyRegActionRets;
	bool bAnyRegActionByValControl;
	bool bAnyRegNextStmt;
	bool bAnyRegCurStateRef;
	bool bAnyRegBreak;
	bool bAnyLmSwitchError;
	bool bAnyConditions;

	int maxState;
	int maxSingleLen;
	int maxRangeLen;
	int maxKeyOffset;
	int maxIndexOffset;
	int maxIndex;
	int maxActListId;
	int maxActionLoc;
	int maxActArrItem;
	unsigned long long maxSpan;
	unsigned long long maxCondSpan;
	int maxFlatIndexOffset;
	Key maxKey;
	int maxCondOffset;
	int maxCondLen;
	int maxCondSpaceId;
	int maxCondIndexOffset;
	int maxCond;

	bool useIndicies;
};

class CCodeGen : virtual public FsmCodeGen
{
public:
	CCodeGen( ostream &out ) : FsmCodeGen(out) {}

	virtual string NULL_ITEM();
	virtual string POINTER();
	virtual ostream &SWITCH_DEFAULT();
	virtual ostream &OPEN_ARRAY( string type, string name );
	virtual ostream &CLOSE_ARRAY();
	virtual ostream &STATIC_VAR( string type, string name );
	virtual string ARR_OFF( string ptr, string offset );
	virtual string CAST( string type );
	virtual string UINT();
	virtual string PTR_CONST();
	virtual string CTRL_FLOW();
};

class DCodeGen : virtual public FsmCodeGen
{
public:
	DCodeGen( ostream &out ) : FsmCodeGen(out) {}

	virtual string NULL_ITEM();
	virtual string POINTER();
	virtual ostream &SWITCH_DEFAULT();
	virtual ostream &OPEN_ARRAY( string type, string name );
	virtual ostream &CLOSE_ARRAY();
	virtual ostream &STATIC_VAR( string type, string name );
	virtual string ARR_OFF( string ptr, string offset );
	virtual string CAST( string type );
	virtual string UINT();
	virtual string PTR_CONST();
	virtual string CTRL_FLOW();
};

class JavaCodeGen : virtual public FsmCodeGen
{
public:
	JavaCodeGen( ostream &out ) : FsmCodeGen(out) {}

	virtual string NULL_ITEM();
	virtual string POINTER();
	virtual ostream &SWITCH_DEFAULT();
	virtual ostream &OPEN_ARRAY( string type, string name );
	virtual ostream &CLOSE_ARRAY();
	virtual ostream &STATIC_VAR( string type, string name );
	virtual string ARR_OFF( string ptr, string offset );
	virtual string CAST( string type );
	virtual string UINT();
	virtual string PTR_CONST();
	virtual string GET_KEY();
	virtual string CTRL_FLOW();
};

#endif /* _FSMCODEGEN_H */
