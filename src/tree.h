#ifndef __PINT_TREE_H
#define __PINT_TREE_H

#include <string>
#include <vector>
#include <memory>

namespace pint {
	class Expr {
	};

	class UnitExpr: public Expr {
		public:
			UnitExpr(std::string name) {
				std::cout << "Unit found with name: " << name << "\n";
			}

			std::string name;
	};

	class IdentifierExpr: public Expr {
		public:
			IdentifierExpr(std::string name) {

			}

			std::string name;
	};

	class IdentifierListExpr: public Expr {
		public:
			IdentifierListExpr(std::shared_ptr<IdentifierExpr> identifier) {
				list.push_back(identifier);
			}

			std::vector<std::shared_ptr<IdentifierExpr>> list;
	};


	class UsesExpr: public Expr {
		public:
			UsesExpr(std::shared_ptr<IdentifierListExpr> identifiers) {

			}

			std::shared_ptr<IdentifierListExpr> identifiers;
	};

}

#endif