#ifndef __PINT_TREE_H
#define __PINT_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#define as(t, v) std::dynamic_pointer_cast<t>(v)

namespace pint {
	/**
	 * Nodeession.
	 */
	class Node {
		public:
			virtual ~ Node() {}
	};


	class ListNode: public Node {
		public:
			std::vector<std::shared_ptr<Node>> list;

			ListNode() {
				this->list = std::vector<std::shared_ptr<Node>>{};
			}

			ListNode(std::shared_ptr<Node> item) {
				this->list = std::vector<std::shared_ptr<Node>>{item};
			}
	};

	class IdentifierNode: public Node {
		public:
			std::string name;

			IdentifierNode(std::string name) {
				this->name = name;
			}
	};

	class UnitNode: public Node {
		public:
			std::shared_ptr<ListNode> path;

			UnitNode(std::shared_ptr<ListNode> path) {
				this->path = path;
			}
	};

	class UsesNode: public Node {
		public:
			std::shared_ptr<ListNode> imports;

			UsesNode(std::shared_ptr<ListNode> imports) {
				this->imports = imports;
			}
	};

	class ImportNode: public Node {
		public:
			std::shared_ptr<UnitNode> unit;
			std::shared_ptr<IdentifierNode> module;
			std::shared_ptr<IdentifierNode> alias;

			ImportNode(std::shared_ptr<UnitNode> unit) {
				this->unit  = unit;
				this->alias = as(IdentifierNode, unit->path->list.back());
			}

			ImportNode(std::shared_ptr<UnitNode> unit, std::shared_ptr<IdentifierNode> module) {
				this->unit   = unit;
				this->module = module;
				this->alias  = module;
			}

      void setModule(std::shared_ptr<IdentifierNode> module) {
        this->module = module;
      }

			void setAlias(std::shared_ptr<IdentifierNode> alias) {
				this->alias = alias;
			}
	};
}

#endif
