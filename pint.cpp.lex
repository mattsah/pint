//
//
//

%lex

%%

\s+						%empty
[0-9]					INT
[0-9]+\.[0-9]+			FLOAT
[a-zA-Z_][a-zA-Z0-9_]*	IDENTIFIER

/lex %{

	#include <string>
	#include <initializer_list>

	/**
	* Expression type.
	*/
	enum class ExpType {
		Number,
		String,
		Symbol,
		List,
	};

	/**
	* Expression.
	*/
	class Exp {
		public:
		ExpType type;

		int number;
		std::string string;
		std::vector<std::shared_ptr<Exp>> list;

		// Numbers:
		Exp(int number) : type(ExpType::Number), number(number) {}

		// Strings, Symbols:
		Exp(std::string& strVal) {
			if (strVal[0] == '"') {
				type = ExpType::String;
				string = strVal.substr(1, strVal.size() - 2);
			} else {
				type = ExpType::Symbol;
				string = strVal;
			}
		}

		// Lists:
		Exp(std::vector<std::shared_ptr<Exp>> list) : type(ExpType::List), list(list) {}

		// List init:
		Exp(const std::initializer_list<std::shared_ptr<Exp>> &list) : type(ExpType::List), list(list) {}

		~Exp() {}
	};

	using Value = std::shared_ptr<Exp>;
%}

%%

list
		:
;