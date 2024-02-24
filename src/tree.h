#ifndef __PINT_TREE_H
#define __PINT_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#define as(t, v) std::dynamic_pointer_cast<t>(v)

namespace pint {
	/**
	 * Expression.
	 */
	class Expr {
		public:
			virtual ~ Expr() {}
	};


	class ListExpr: public Expr {
		public:
			std::vector<std::shared_ptr<Expr>> list;

			ListExpr() {
				this->list = std::vector<std::shared_ptr<Expr>>{};
			}

			ListExpr(std::shared_ptr<Expr> item) {
				this->list = std::vector<std::shared_ptr<Expr>>{item};
			}
	};

	class IdentifierExpr: public Expr {
		public:
			std::string name;

			IdentifierExpr(std::string name) {
				this->name = name;
			}
	};

	class UnitExpr: public Expr {
		public:
			std::shared_ptr<ListExpr> path;

			UnitExpr(std::shared_ptr<ListExpr> path) {
				this->path = path;
				std::cout << "Unit path length: " << this->path->list.size() << std::endl;
			}
	};

	class UsesExpr: public Expr {
		public:
			std::shared_ptr<ListExpr> imports;

			UsesExpr(std::shared_ptr<ListExpr> imports) {
				this->imports = imports;
				std::cout << "Total imports: " << this->imports->list.size() << std::endl;
			}
	};

	class IdentifierPathAsExpr: public Expr {
		public:
			std::shared_ptr<ListExpr> path;
			std::shared_ptr<IdentifierExpr> member;
			std::shared_ptr<IdentifierExpr> alias;

			IdentifierPathAsExpr(std::shared_ptr<ListExpr> path) {
				this->path  = path;
				this->alias = as(IdentifierExpr, path->list.back());
			}

			IdentifierPathAsExpr(std::shared_ptr<ListExpr> path, std::shared_ptr<IdentifierExpr> member) {
				this->path   = path;
				this->member = member;
				this->alias  = member;
			}

			void setAlias(std::shared_ptr<IdentifierExpr> alias) {
				this->alias = alias;
			}
	};
}

#endif