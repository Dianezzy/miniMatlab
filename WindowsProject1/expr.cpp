#include "expr.h"
#include <sstream>
#include <stack>

int	Operator::NOperandsTable[128];
int Operator::PrecedenceTable[128];
char Operator::UserDefinedOperators[10];
int	Operator::FreeOperators;
std::string Operator::OperatorToFunc[128];

std::map<std::string, Matrix> Command::VarList;
std::map<std::string, Function> Command::FuncList;

void Operator::Initialize() {
	for (auto &it : PrecedenceTable)
		it = -1;// default precedence for non-opeartors
	for (auto &it : NOperandsTable)
		it = 0;
	PrecedenceTable['\''] = 4;
	NOperandsTable['\''] = 1;
	PrecedenceTable['!'] = 4;
	NOperandsTable['!'] = 1;
	PrecedenceTable['^'] = 4;
	NOperandsTable['^'] = 2;
	PrecedenceTable['*'] = 3;
	NOperandsTable['*'] = 2;
	PrecedenceTable['/'] = 3;
	NOperandsTable['/'] = 2;
	PrecedenceTable['+'] = 2;
	NOperandsTable['+'] = 2;
	PrecedenceTable['-'] = 2;
	NOperandsTable['-'] = 2;
	PrecedenceTable[')'] = 1;
	PrecedenceTable['('] = 1;
	PrecedenceTable['\0'] = 0;

	UserDefinedOperators[0] = '`';
	UserDefinedOperators[1] = '~';
	UserDefinedOperators[2] = '@';
	UserDefinedOperators[3] = '#';
	UserDefinedOperators[4] = '$';
	UserDefinedOperators[5] = '&';
	UserDefinedOperators[6] = '_';
	UserDefinedOperators[7] = '|';
	UserDefinedOperators[8] = '?';
	UserDefinedOperators[9] = ':';
	FreeOperators = 10;
}

int Operator::PrecedenceLevel(bool LeftHandSide) const {
	if (Name == '(' && LeftHandSide) return 5;
	return PrecedenceTable[Name];
}

bool IsOperator(char _c) {
	return Operator(_c).PrecedenceLevel() >= 0;
}

int Operator::NOperands() {
	return NOperandsTable[Name];
}

char Operator::GetName() {
	return Name;
}

Matrix Operator::Calculate(Matrix m1, Matrix m2) {
	if (Name == '+') return m1 + m2;
	else if (Name == '-') return m1 - m2;
	else if (Name == '*') {
		if (m1.GetRow() == 1 && m1.GetCol() == 1) return m2 * m1.GetData(0);
		if (m2.GetRow() == 1 && m2.GetCol() == 1) return m1 * m2.GetData(0);
		return m1 * m2;
	}
	else if (Name == '/') {
		if (m2.GetRow() == 1 && m2.GetCol() == 1) return m1 / m2.GetData(0);
		return m1 / m2;
	}
	else if (Name == '^') {
		if (m2.GetRow() == 1 && m2.GetCol() == 1) return m1 ^ m2.GetData(0);
		else error("Expected an operand after operator ^");
	}
	else if (Name == '!') return m1.Inv();
	else if (Name == '\'') return m1.Trans();
	else {
		Function f = Command::FuncList[OperatorToFunc[Name]];
		return f.Calculate(m1);
	}
	return Matrix();
}

bool Operator::operator>(const Operator & op) const {
	return PrecedenceLevel(true) > op.PrecedenceLevel(false);
}

void Operator::CreateOperator(std::string FuncName){
	if(FreeOperators == 0) {
		error("You can create 10 Functions at most. No operators Left.");
	}
	char op = UserDefinedOperators[--FreeOperators];
	NOperandsTable[op] = 1;
	PrecedenceTable[op] = 3;
	OperatorToFunc[op] = FuncName;
}

char Operator::FindOperator(std::string FuncName){
	for(auto op: UserDefinedOperators){
		if(OperatorToFunc[op] == FuncName) return op;
	}
	return 0;
}

void Command::Load(std::string _str) {
	std::string str;
	for (auto c : _str) {
		// omit whitespaces
		if (c != ' ' && c != '\t' && c != '\n') str += c;
	}
	if (str.find('=') != str.npos) {
		VarName = str.substr(0, str.find('='));
		Expr = str.substr(str.find('=') + 1);
	}
	else {
		Expr = str;
		VarName = "NoVarNameInput";
	}
}

Matrix Command::EvaluateExprDir() {
	// square-bracket-defined matrix, direct assignment (e.g. [1,2;3,4])
	int row = 1, col = 0;
	double x;
	char c;
	std::stringstream buf(Expr);
	buf >> c;// '['
	if (Expr.find(']') == Expr.npos || !(Expr[1] >= '0' && Expr[1] <= '9'))
		error("The matrix is ill-formed. It must contain a ] and at least 1 element.");
	// count the columns
	do {
		col++;
		buf >> x >> c;
	} while (c == ',');
	// count the rows
	while (c != ']') {
		for (int i = 1; i <= col; i++) {
			buf >> x >> c;
			if (i != col && c != ',') error("The matrix is ill-formed. Num of elements in row " +
				std::to_string(row + 1) + " doesn't match with its Col.");
		}
		row++;
	}
	// assignment
	Matrix ret = Matrix(row, col);
	std::string ExprNoComma = Expr;
	for (auto &c : ExprNoComma)
		if (c < '0' || c > '9') c = ' ';
	std::stringstream(ExprNoComma) >> ret;
	return ret;
}

Matrix Command::EvaluateExpr() {
	std::stack<Matrix> operands;
	std::stack<Operator> operators;
	std::string ExprReversePolish = "";
	std::string curVar = "";
	operators.push(Operator('\0'));
	for (int i = 0; i <= Expr.length(); i++) {// c may be \0 - that's what we need
		char c = Expr[i];
		if (!IsOperator(c)) {
			// reading a var Name
			curVar += c;
			if (IsOperator(Expr[i + 1])) {
				// end of a var Name
				if (curVar[0] >= '0' && curVar[0] <= '9') {
					// an immediate number (an 1-by-1 matrix)
					Matrix num(1, 1);
					std::stringstream(curVar) >> num;
					operands.push(num);
				}
				else if (VarList.find(curVar) == VarList.end()) {
					error("Variable " + curVar + " doen't exist.");
				}
				else {
					operands.push(VarList[curVar]);
				}
				curVar = "";
			}
		}
		else {
			while (!(operators.empty() || Operator(c) > operators.top())) {
				Operator op = operators.top();
				operators.pop();
				if (c == ')' && op.GetName() == '(') break;
				if (operands.size() < op.NOperands())
					error(std::string("Insufficient operands for operator ") + op.GetName());
				if (op.NOperands() == 2) {
					Matrix m2 = operands.top();
					operands.pop();
					Matrix m1 = operands.top();
					operands.pop();
					operands.push(op.Calculate(m1, m2));
				}
				else if (op.NOperands() == 1) {
					Matrix m1 = operands.top();
					operands.pop();
					operands.push(op.Calculate(m1));
				}
			}
			if (c != ')') operators.push(Operator(c));
		}
	}
	// expected: there is exactly 1 operand Left
	if (operands.size() != 1)
		error(std::to_string(operands.size()) + " operands Left after evluation, expected 1");
	return operands.top();
}

bool Command::Execute(Matrix &ret) {
	// check the variable Name
	for (auto c : VarName) {
		if (IsOperator(c)) {
			error("Invalid identifier: " + VarName);
		}
	}
	// create a new var if the variable Name hasn't been occupied
	if (VarList.find(VarName) == VarList.end()) {
		VarList[VarName] = Matrix();
	}
	Matrix& tagVar = VarList[VarName];
	// evalute the Expression
	if (VarName.find("[") != VarName.npos && VarName.find("]") != VarName.npos) {
		// define a Function
		std::string FuncName = VarName.substr(0, VarName.find("["));
		std::string paraName = VarName.substr(VarName.find("[") + 1, VarName.size() - VarName.find("[") - 2);
		if(FuncList.find(FuncName) == FuncList.end()){
			// assign an operator for this Function
			Operator::CreateOperator(FuncName);
		}
		Function f(paraName, Expr);
		FuncList[FuncName] = f;
		// actually not an error - it's a feedback on the assigned operator of the Function
		error(std::string("Function created. The assigned operator is ") + Operator::FindOperator(FuncName));
	}
	else if (VarName == "Integrate" || VarName == "Solve"){
		if(Expr.find(",") == Expr.npos)
			error("Format error. Expected [Function], [lower bound], [upperbound]");
		std::string FuncName = Expr.substr(0, Expr.find(","));
		Expr = Expr.substr(Expr.find(",") + 1);
		if(Expr.find(",") == Expr.npos)
			error("Format error. Expected [Function], [lower bound], [upperbound]");
		double l = std::stod(Expr.substr(0, Expr.find(",")));
		Expr = Expr.substr(Expr.find(",") + 1);
		double r = std::stod(Expr);
		if(FuncList.find(FuncName) == FuncList.end())
			error("Function " + FuncName + " doesn't exist");
		Function f = FuncList[FuncName];
		if(VarName == "Integrate"){
			double result = Integrate(
				[&](double x)->double{
					return f.Calculate(x);
				},
			l, r);
			error(std::to_string(result));
		}
		else{
			tagVar = EquationSolve(
				[&](double x)->double{
					return f.Calculate(x);
				},
			l,r);
		}
	}
	else if (Expr[0] == '[') {
		tagVar = EvaluateExprDir();
	}
	else {
		tagVar = EvaluateExpr();
	}
	// feed back
	ret = tagVar;
	return true;
}

void Command::debug() {
	std::cout << "VarName:[" << VarName << "] Expr:[" << Expr << "]" << std::endl;
}

void error(std::string errMsg) {
	throw(errMsg);
}

std::string Function::GetParamName(){
	return VarName;
}

Matrix Function::Calculate(Matrix _input){
	Matrix ret(_input.GetRow(), _input.GetCol());
	// Load the input matrix
	Matrix tmp = Command::VarList[VarName];
	Command::VarList[VarName] = _input;
	// Calculate
	Command c;
	c.Load(Expr);
	c.Execute(ret);
	// restore
	Command::VarList[VarName] = tmp;
	return ret;
}

double Function::Calculate(double _input){
	Matrix m(1,1);
	std::stringstream buf(std::to_string(_input));
	buf >> m;
	return Calculate(m).GetData(0);
}

Function::Function(std::string _VarName, std::string _expr){
	VarName = _VarName;
	Expr = _expr;
}

Function::Function() {
}
