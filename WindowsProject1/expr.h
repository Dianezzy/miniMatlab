#pragma once
#include <map>
#include <vector>
#include <Functional>
#include "numcalc.h"

class Operator {
private:
	char		Name;
	static int	NOperandsTable[128];
	static int	PrecedenceTable[128];
	static char UserDefinedOperators[10];
	static int	FreeOperators;
	int			PrecedenceLevel(bool LeftHandSide = false) const;
	static std::string OperatorToFunc[128];

public:
	static void	Initialize();
	Operator(char _c) : Name(_c) {}
	friend bool	IsOperator(char _c);
	int			NOperands();
	char		GetName();
	Matrix		Calculate(Matrix m1, Matrix	m2 = Matrix());
	bool		operator > (const Operator& op) const;
	static void	CreateOperator(std::string FuncName);
	static char	FindOperator(std::string FuncName);
	/*
		We say op1 > op2 if op1 can be placed on op2 in the operators stack
		otherwise, the stack will be popped, until op1 doesn't > operators.top()
		precedence:
		5. ( when appearing at the Left-hand-side of '>'
		4. !(Inverse), '(Transpose), ^(power)
		3. *, /
		2. +, -
		1. (
		0. '\0'
		Special: ')' won't be pushed onto the operators stack
		when processing ')', the stack will keep popping until a '(' is popped
	*/
};

class Function {
private:
	std::string	VarName;
	std::string	Expr;
public:
	std::string	GetParamName();
	Matrix		Calculate(Matrix _input);
	double		Calculate(double _input);
	Function(std::string _VarName, std::string _expr);
	Function();
};

class Command {
private:
	std::string VarName;
	std::string Expr;
	Matrix		EvaluateExprDir();
	Matrix		EvaluateExpr();

public:
	static		std::map<std::string, Matrix> VarList;
	static		std::map<std::string, Function> FuncList;
	void		Load(std::string _str);
	bool		Execute(Matrix &ret);
	void		debug();
};

void		error(std::string errMsg);