#include <map>
#include <functional>
#include <var/var.hpp>

class Lisp
{
	private:
		std::map <var, std::function<var(var&)> > functions;

	public:
		Lisp();

		var set(const var& indexes, var& value);
		var eval(var& trunk);
		var get(const var& indexes);

	private:
		var stack;

		static var setVariable(var& trunk, var& indexes, var& value, var position = 0);
		static var getVariable(var& trunk, var& indexes, var position = 0);
};
