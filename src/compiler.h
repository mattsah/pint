#include "tree.h"
#include "parser.h"

namespace pint {
	class Compiler {
		public:
			Compiler() {
				parser = std::make_unique<syntax::parser>();
			}

			/**
			 *
			 */
			void parse(const std::string &code) {
				parser->parse(code);
			}


		private:
			/**
			 *
			 */
			std::unique_ptr<syntax::parser> parser;

			/**
			 *
			 */
			void exec(const Expr &node) {

			}
	};
}