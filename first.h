/**
 * @brief The specification for the static First set-collection object.
 *
 * @file first.h
 * 
 * This class is a deceptively simple one.
 * The First object is really just a collection of functions
 * within an easily modifiable class structure that returns the
 * "First set" of a given Nonterminal.
 *
 * Which is to say, the "first" terminals of a given Nonterminal
 * (or, in our case, function) is returned.
 *
 * @see set.h
 * @see parser.cpp
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to March 1st, 2011
 **/

#ifndef FIRST_H
#define FIRST_H

/** Essentially, everything done in this program relies upon the Set class. */
#include "set.h"

/**
 * @brief A static class that constructs First Sets for given productions/nonterminals.
 * Please refer to the specification for a FIRST SET as well as the PL Language.
 *
 * The First object provides a series of static functions that return the semi-predefined 
 * FIRST sets for any given production rule in the PL language.
 * 
 * These sets are composed using the appropriate set of nonterminals and their first sets, and
 * to that end, their terminals/nonterminals.
 *
 * As such, the functionality of virtually every function in this object is the same, but with
 * tweaked heuristics and unique Set string values per each set.
 */
class First {
	
  private:

  public:
    
    /** Base Constructor. */
    First();
    
    /**
     *
     * The following functions have been chunked according to logical
     * groups, but I avoided re-ordering them at all in case there was 
     * a reason for the ordering - such as following closely to the book's
     * production rules or something like that.
     *
     *For each of the following functions, a base set or sets are created with
     * the appropriate set values corresponding to the First set for the given
     * nonterminal that each function represents (by name).
     * After this, sets are created using the mUnion of the given function set
     * with the following productions' first sets.
     *
     * Finally, the static functions return the given sets and are accessible 
     * without the class being instantiated.
     *
     * In other words, every function here is representative of a Function in the
     * Parser, and delivers their Follow sets upon call.
     *
     */

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