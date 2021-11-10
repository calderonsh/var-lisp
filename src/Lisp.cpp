#include "Lisp.hpp"

Lisp::Lisp()
{
	this->functions["++"] = [this](var& that) -> var
	{
		var result = this->eval(that[1]) + 1;
		this->set(that[1], result);

		return result;
	};

	this->functions["--"] = [this](var& that) -> var
	{
		var result = this->eval(that[1]);
		result = result - 1;

		this->set(that[1], result);

		return result;
	};

	this->functions["="] = [this](var& that) -> var
	{
		if (that.size() < 3) {
			return 0;
		}

		return this->set(that[1], that[2]);
	};

	this->functions["+"] = [this](var& that) -> var {
		return this->eval(that[1]) + this->eval(that[2]);
	};

	this->functions["-"] = [this](var& that) -> var {
		return this->eval(that[1]) - this->eval(that[2]);
	};

	this->functions["*"] = [this](var& that) -> var {
		return this->eval(that[1]) * this->eval(that[2]);
	};

	this->functions["/"] = [this](var& that) -> var {
		return this->eval(that[1]) / this->eval(that[2]);
	};

	this->functions["=="] = [this](var& that) -> var {
		return this->eval(that[1]) == this->eval(that[2]);
	};

	this->functions["!="] = [this](var& that) -> var {
		return this->eval(that[1]) != this->eval(that[2]);
	};

	this->functions["and"] = [this](var& that) -> var {
		return this->eval(that[1]) && this->eval(that[2]);
	};

	this->functions["or"] = [this](var& that) -> var {
		return this->eval(that[1]) || this->eval(that[2]);
	};

	this->functions[">"] = [this](var& that) -> var {
		return this->eval(that[1]) > this->eval(that[2]);
	};

	this->functions["<"] = [this](var& that) -> var {
		return this->eval(that[1]) < this->eval(that[2]);
	};

	this->functions["if"] = [this](var& that) -> var
	{
		if (that.size() < 3) {
			return 0;
		}

		var result;
		if (this->eval(that[1])) {
			result = this->eval(that[2]);
		} else
		{
			if (that.size() == 4) {
				result = this->eval(that[3]);
			}
		}

		return 0;
	};

	this->functions["while"] = [this](var& that) -> var
	{
		var result;
		while (this->eval(that[1])) {
			result = this->eval(that[2]);
		}

		return 0;
	};

	this->functions["for"] = [this](var& that) -> var
	{
		var result;
		for (this->eval(that[1]); this->eval(that[2]); this->eval(that[3])) {
			result = this->eval(that[4]);
		}

		return 0;
	};

	this->functions["echo"] = [this](var& that) -> var
	{
		var result = this->eval(that[1]);
		printf("%s\n", (const char*)result);

		return result;
	};

	this->functions["dump"] = [this](var& that) -> var
	{
		printf("%s\n", (const char*)this->stack[this->stack.size()-1].encode().c_str());

		return 0;
	};

	this->stack.decode("[{}]");
}

var Lisp::set(const var& index, var& value)
{
	var indexes = index;
	if (var::type(indexes) == var::string) {
		indexes = indexes.split(".");
	}

	var result = this->eval(value);
	this->setVariable(this->stack[this->stack.size() - 1], indexes, result);

	return result;
}

var Lisp::eval(var& trunk)
{
	if (var::type(trunk) == var::vector)
	{
		if (trunk.size() == 0) {
			return trunk;
		}

		if (var::type(trunk[0]) == var::vector)
		{
			for (var i in trunk) {
				this->eval(trunk[i]);
			}

			return 0;
		}

		if (var::type(trunk[0]) == var::string)
		{
			auto it = this->functions.find(trunk[0]);

			if (it != this->functions.end()) {
				return this->functions[trunk[0]](trunk);
			}
		}
	}

	return this->getVariable(this->stack[this->stack.size()-1], trunk);
}

var Lisp::get(const var& indexes)
{
	var program = indexes;
	return this->eval(program);
}

var Lisp::setVariable(var& trunk, var& indexes, var& value, var position)
{
	if (position + 1 < (var)indexes.size()) {
		return setVariable(trunk[indexes[position]], indexes, value, position + 1);
	} else {
		return trunk[indexes[position]] = value;
	}
}

var Lisp::getVariable(var& stack, var& trunk, var position)
{
	var indexes = trunk;
	if (var::type(indexes) == var::string) {
		indexes = indexes.split(".");
	}

	if (position + 1 < (var)indexes.size())
	{
		for (var i in stack)
		{
			if (i == indexes[position]) {
				return getVariable(stack[indexes[position]], indexes, position + 1);
			}
		}
	}
	else
	{
		for (var i in stack)
		{
			if (i == indexes[position]) {
				return stack[indexes[position]];
			}
		}
	}

	return trunk;
}
