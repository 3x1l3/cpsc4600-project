#ifndef FIRST_H
#define FIRST_H

#include "set.h"

class First {
	
private:


public:

	First();
	static Set Program();
	static Set Block();
	static Set DefinitionPart();
	static Set Definition();
	static Set ConstantDefinition();
	static Set VariableDefinition();
	static Set VariableDefinitionPart();
	
	
	
	static Set TypeSymbol();
	static Set VariableList();
	static Set ProcedureDefinition();
	static Set StatementPart();
	static Set Statement();
	static Set EmptyStatement();
	static Set ReadStatement();
	static Set VariableAccessList();
	static Set WriteStatement();
	static Set ExpressionList();
	static Set AssignmentStatement();
	static Set ProcedureStatement();
	static Set IfStatement();
	static Set DoStatement();
	static Set GuardedCommandList();
	static Set GuardedCommand();
	static Set Expression();
	static Set PrimaryOperator();
	static Set PrimaryExpression();
	static Set RelationalOperator();
	static Set SimpleExpression();
	static Set AddingOperator();
	static Set Term();
	static Set MultiplyingOperator();
	static Set Factor();
	static Set VariableAccess();
	static Set IndexedSelector();
	static Set Constant();
	static Set Numeral();
	static Set BooleanSymbol();
	static Set ConstantName();
	static Set VariableName();
	static Set ProcedureName();
	static Set FactorName();
	
	
};


#endif