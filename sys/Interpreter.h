#ifndef _Interpreter_h_
#define _Interpreter_h_
/* Interpreter.h
 *
 * Copyright (C) 1993-2011,2013,2014,2015 Paul Boersma
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Collection.h"
#include "Gui.h"
#include "Formula.h"

#if defined (macintosh) && useCarbon
	#define USE_HASH  0
#else
	#define USE_HASH  1
#endif

#if USE_HASH
#include <string>
#include <unordered_map>
#endif

Thing_define (InterpreterVariable, SimpleString) {
	char32 *stringValue;
	double numericValue;
	struct Formula_NumericArray numericArrayValue;

	void v_destroy ()
		override;
};

#define Interpreter_MAXNUM_PARAMETERS  400
#define Interpreter_MAXNUM_LABELS  1000
#define Interpreter_MAX_CALL_DEPTH  50

#define Interpreter_MAX_LABEL_LENGTH  99

Thing_declare (UiForm);
Thing_declare (Editor);

Thing_define (Interpreter, Thing) {
	char32 *environmentName;
	ClassInfo editorClass;
	int numberOfParameters, numberOfLabels, callDepth;
	char32 parameters [1+Interpreter_MAXNUM_PARAMETERS] [100];
	int types [1+Interpreter_MAXNUM_PARAMETERS];
	char32 *arguments [1+Interpreter_MAXNUM_PARAMETERS];
	char32 choiceArguments [1+Interpreter_MAXNUM_PARAMETERS] [100];
	char32 labelNames [1+Interpreter_MAXNUM_LABELS] [1+Interpreter_MAX_LABEL_LENGTH];
	long labelLines [1+Interpreter_MAXNUM_LABELS];
	char32 dialogTitle [1+100], procedureNames [1+Interpreter_MAX_CALL_DEPTH] [100];
	#if USE_HASH
	std::unordered_map <std::u32string, InterpreterVariable> *variablesMap;
	#else
	autoSortedSetOfString variables;
	#endif
	bool running, stopped;

	void v_destroy ()
		override;
};

Interpreter Interpreter_create (char32 *environmentName, ClassInfo editorClass);
Interpreter Interpreter_createFromEnvironment (Editor editor);

void Melder_includeIncludeFiles (char32 **text);
long Interpreter_readParameters (Interpreter me, char32 *text);
Thing_declare (UiForm);
UiForm Interpreter_createForm (Interpreter me, GuiWindow parent, const char32 *fileName,
	void (*okCallback) (UiForm sendingForm, int narg, Stackel args, const char32 *sendingString, Interpreter interpreter, const char32 *invokingButtonTitle, bool modified, void *closure), void *okClosure,
	bool selectionOnly);
void Interpreter_getArgumentsFromDialog (Interpreter me, UiForm dialog);
void Interpreter_getArgumentsFromString (Interpreter me, const char32 *arguments);
void Interpreter_getArgumentsFromArgs (Interpreter me, int nargs, Stackel args);
void Interpreter_run (Interpreter me, char32 *text);   // destroys 'text'
void Interpreter_stop (Interpreter me);   // can be called from any procedure called deep-down by the interpreter; will stop before next line

void Interpreter_voidExpression (Interpreter me, const char32 *expression);
void Interpreter_numericExpression (Interpreter me, const char32 *expression, double *value);
void Interpreter_stringExpression (Interpreter me, const char32 *expression, char32 **value);
void Interpreter_numericArrayExpression (Interpreter me, const char32 *expression, struct Formula_NumericArray *value);
void Interpreter_anyExpression (Interpreter me, const char32 *expression, struct Formula_Result *result);

InterpreterVariable Interpreter_hasVariable (Interpreter me, const char32 *key);
InterpreterVariable Interpreter_lookUpVariable (Interpreter me, const char32 *key);

/* End of file Interpreter.h */
#endif
