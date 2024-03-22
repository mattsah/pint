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
				std::cout << "Unit path length: " << this->path->list.size() << std::endl;
			}
	};

	class UsesNode: public Node {
		public:
			std::shared_ptr<ListNode> imports;

			UsesNode(std::shared_ptr<ListNode> imports) {
				this->imports = imports;
				std::cout << "Total imports: " << this->imports->list.size() << std::endl;
			}
	};

	class PathAsNode: public Node {
		public:
			std::shared_ptr<ListNode> path;
			std::shared_ptr<IdentifierNode> member;
			std::shared_ptr<IdentifierNode> alias;

			PathAsNode(std::shared_ptr<ListNode> path) {
				this->path  = path;
				this->alias = as(IdentifierNode, path->list.back());
			}

			PathAsNode(std::shared_ptr<ListNode> path, std::shared_ptr<IdentifierNode> member) {
				this->path   = path;
				this->member = member;
				this->alias  = member;
			}

			void setAlias(std::shared_ptr<IdentifierNode> alias) {
				this->alias = alias;
			}
	};
}

#endif
