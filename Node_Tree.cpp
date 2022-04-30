#include<vector>
#include<algorithm>
#include<iostream>
using namespace std;

class Tree {
	class Branch;
	class Node {
	public:
		int value;
		Branch* sub_branch;
		bool isLast;


		Node() {
			value = INT32_MAX;
			isLast = false;
		}
		Node(int new_val) {
			value = new_val;
		}

		void SetBranch(Branch* br) {
			sub_branch = br;
		}

		void MakeLast() {
			isLast = true;
		}

		friend bool operator==(Node lhs, Node rhs) {
			if (lhs.isLast || rhs.isLast) return false;
			return lhs.value == rhs.value;
		}

		friend bool operator<(Node lhs, Node rhs) {
			if (lhs.isLast) return false;
			if (rhs.isLast) return true;
			return lhs.value < rhs.value;
		}

		friend ostream& operator<<(ostream& stream, Node& node) {
			return stream << node.value;
		}
	};

	class Branch {
	public:
		vector<Node> nodes;
		Branch* prev_branch;

		Branch() {
			Node last_node;
			last_node.MakeLast();
			nodes.push_back(last_node);
		}


		Branch(vector<Node> new_nodes, Branch* new_prev_branch) {
			nodes = new_nodes;
			prev_branch = new_prev_branch;
			if (new_nodes[new_nodes.size() - 1].isLast) return;
			Node last_node;
			last_node.MakeLast();
			nodes.push_back(last_node);
		}

		Node& operator[](int index) {
			return nodes[index];
		}

		int GetSize() {
			return nodes.size();
		}

		friend ostream& operator<<(ostream& stream, Branch branch) {
			stream << "[  ";
			for (auto n : branch.nodes) {
				if (!n.isLast)
					stream << n << "  ";
			}
			return stream << "]";
		}

		void ChangePrevBranches(Branch& br) {
			if (br.nodes[0].sub_branch != nullptr) {
				if (br.nodes[0].sub_branch->nodes[0].sub_branch == nullptr)
					for (int i = 0; i < br.nodes.size(); ++i) {
						br.nodes[i].sub_branch->prev_branch = &br;
					}
				else {
					for (int i = 0; i < br.nodes.size(); ++i) {
						ChangePrevBranches(*br.nodes[i].sub_branch);
					}
				}
			}
		}

		void PutUp() {
			/*
			vector<Node> new_branch;
			for (int i = nodes.size() / 2 + 2; i < nodes.size(); ++i) {
				new_branch.push_back(nodes[i]);
			}

			Node mid = nodes[nodes.size() / 2 + 1];
			nodes.resize(nodes.size() / 2);
			prev_branch->Add(mid);

			Branch *br = new Branch(new_branch, prev_branch);
			(*prev_branch)[prev_branch->GetSize()-1].SetBranch(br);*/

			vector<Node> first_vec, second_vec;
			Node mid = nodes[nodes.size() / 2 - 1];
			for (int i = 0; i < nodes.size() / 2 - 1; ++i) {
				first_vec.push_back(nodes[i]);
			}
			for (int i = nodes.size() / 2; i < nodes.size(); ++i) {
				second_vec.push_back(nodes[i]);
			}

			if (prev_branch == nullptr) {
				auto old_nodes = nodes;
				nodes.clear();
				Branch first_branch(first_vec, this), second_branch(second_vec, this);

				{
					/*
					Node last_node;
					last_node.MakeLast();
					last_node.SetBranch();*/
					first_branch.nodes[first_branch.GetSize() - 1].SetBranch(mid.sub_branch);
				}

				mid.SetBranch(new Branch(first_branch));
				nodes.push_back(mid);

				{
					if (!second_branch[second_branch.GetSize() - 1].isLast) {
						Node last_node;
						last_node.MakeLast();
						second_branch.nodes.push_back(last_node);
					}
				}

				{
					Node last_node;
					last_node.MakeLast();
					last_node.SetBranch(new Branch(second_branch));
					nodes.push_back(last_node);
				}
			}
			else {
				Branch first_branch(first_vec, prev_branch), second_branch(second_vec, prev_branch);

				{
					first_branch.nodes[first_branch.GetSize() - 1].SetBranch(mid.sub_branch);
				}

				for (int i = 0; i < (*prev_branch).nodes.size(); ++i) {
					if ((*prev_branch).nodes[i].sub_branch == this) {
						(*prev_branch).nodes[i].SetBranch(new Branch(second_branch));
						break;
					}
				}

				mid.SetBranch(new Branch(first_branch));
				(*prev_branch).Add(mid, true);
			}
			ChangePrevBranches(*this);
		}

		void Add(Node& node, bool modif = false) {
			int position = -1;
			for (int i = 0; i < nodes.size(); ++i) {
				if (!nodes[i].isLast) {
					if (node < nodes[i] || node == nodes[i]) {
						position = i;
						break;
					}
				}
			}
			if (modif == false) {
				if (nodes[nodes.size() - 1].sub_branch != nullptr) {
					nodes[position < 0 ? nodes.size() - 1 : position].sub_branch->Add(node);
					return;
				}
			}
			nodes.resize(nodes.size() + 1);
			if (position == -1) {
				if (nodes[nodes.size() - 2].isLast) {
					nodes[nodes.size() - 1] = nodes[nodes.size() - 2];
					nodes[nodes.size() - 2] = node;
				}
				else {
					nodes[nodes.size() - 1] = node;
				}
			}
			else {
				for (int i = (int)nodes.size() - 1; i > position; --i) {
					nodes[i] = nodes[i - 1];
				}
				nodes[position] = node;
			}
			int size_for_check = nodes[nodes.size() - 1].isLast ? nodes.size() - 1 : nodes.size();
			if (size_for_check > 2 * n) {
				PutUp();
			}
		}

		void MiniMerge(vector<Node> add_array) {
		}

		void Rebuild(Branch& br) {
			/*if (nodes.size() < n) {
				int pos_of_prev_node = -1;
				for (int i = 0; i < br.prev_branch->GetSize(); ++i) {
					if (br.prev_branch->nodes[i].sub_branch == this) {
						pos_of_prev_node = i;
						break;
					}
				}
				int choosen_prev_node;
				if (pos_of_prev_node < br.prev_branch->GetSize() - 1) {
					if (pos_of_prev_node > 0) {
						choosen_prev_node = br.prev_branch->nodes[pos_of_prev_node - 1].sub_branch->GetSize()
				> br.prev_branch->nodes[pos_of_prev_node + 1].sub_branch->GetSize() ? pos_of_prev_node - 1 : pos_of_prev_node + 1;
					}
					else {
						choosen_prev_node = pos_of_prev_node + 1;
					}
				}
				else {
					choosen_prev_node = pos_of_prev_node - 1;
				}
				vector<Node> old_nodes = br.prev_branch->nodes[pos_of_prev_node].sub_branch->nodes;
				for (int i = pos_of_prev_node; i < br.prev_branch->nodes.size() - 1; ++i) {
					br.prev_branch->nodes[i] = br.prev_branch->nodes[i + 1];
				}
				prev_branch->nodes.resize(nodes.size() - 1);
				for (int i = 0; i < old_nodes.size() - 1; ++i) {
					prev_branch->nodes[position].sub_branch->nodes.push_back(old_nodes[i]);
				}
				sort(nodes[position].sub_branch->nodes.begin(), nodes[position].sub_branch->nodes.end());
				nodes[position].sub_branch->Add(old_nodes[old_nodes.size() - 1], true);
			}*/
			for (int i = 0; i < br.GetSize(); ++i) {
				if (br[i].sub_branch->GetSize() - 1 < n) {
					int choosen_pos;
					if (i > 0) {
						if (i < br.GetSize() - 1) {
							choosen_pos = br[i - 1].sub_branch->GetSize() > br[i + 1].sub_branch->GetSize() ? -1 : 1;
						}
						else {
							choosen_pos = -1;
						}
					}
					else {
						choosen_pos = 1;
					}
					vector<Node> old_nodes = br[i].sub_branch->nodes;
					Node n(br[i].value);
					old_nodes.push_back(n);
					if (!br[i].isLast) {
						for (int j = i; j < br.GetSize() - 1; ++j) {
							br[j] = br[j + 1];
						}
						br.nodes.resize(br.nodes.size() - 1);
						if (choosen_pos > 0) {
							for (int j = 0; j < old_nodes.size() - 1; ++j) {
								br[i].sub_branch->nodes.push_back(old_nodes[j]);
							}
							sort(br[i].sub_branch->nodes.begin(), br[i].sub_branch->nodes.end());
							if (br[i].sub_branch->nodes[br[i].sub_branch->nodes.size() - 2].isLast) {
								br[i].sub_branch->nodes.resize(br[i].sub_branch->nodes.size() - 1);
							}
							br[i].sub_branch->Add(old_nodes[old_nodes.size() - 1], true);
						}
						else {
							for (int j = 0; j < old_nodes.size() - 1; ++j) {
								br[i - 1].sub_branch->nodes.push_back(old_nodes[j]);
							}
							sort(br[i - 1].sub_branch->nodes.begin(), br[i - 1].sub_branch->nodes.end());
							br[i - 1].sub_branch->Add(old_nodes[old_nodes.size() - 1], true);
						}
						if (br.prev_branch != nullptr) Rebuild(*br.prev_branch);
						return;
					}
					else {
						old_nodes = br[i - 1].sub_branch->nodes;
						Node n(br[i - 1].value);
						old_nodes.push_back(n);
						sort(old_nodes.begin(), old_nodes.end());
						br[i - 1] = br[i];
						br.nodes.resize(br.nodes.size() - 1);
						for (int k = 0; k < old_nodes.size() - 1; ++k) {
							br[i - 1].sub_branch->nodes.push_back(old_nodes[k]);
						}
						sort(br[i - 1].sub_branch->nodes.begin(), br[i - 1].sub_branch->nodes.end());
						br[i - 1].sub_branch->Add(old_nodes[old_nodes.size() - 1], true);
						return;
					}
				}
			}
		}

		void MergeArrays(vector<Node> add_array, int position) {
			if (nodes[position].sub_branch != nullptr) {
				if (nodes[position].sub_branch->nodes[0].sub_branch != nullptr) {
					if (nodes[position].sub_branch->nodes[0].sub_branch->nodes[0].sub_branch != nullptr) {
						nodes[position].sub_branch->nodes[0].sub_branch->MergeArrays(add_array[add_array.size() - 1].sub_branch->nodes, 0);
					}
					if (add_array[add_array.size() - 1].sub_branch->GetSize() > 0) {
						for (int i = 0; i < add_array[add_array.size() - 1].sub_branch->GetSize() - 2; ++i) {
							nodes[position].sub_branch->nodes[0].sub_branch->nodes.push_back(add_array[add_array.size() - 1].sub_branch->nodes[i]);
						}
						sort(nodes[position].sub_branch->nodes[0].sub_branch->nodes.begin(),
							nodes[position].sub_branch->nodes[0].sub_branch->nodes.end());
						nodes[position].sub_branch->nodes[0].sub_branch->Add(add_array[add_array.size() - 1].sub_branch->nodes[add_array[add_array.size() - 1].sub_branch->GetSize() - 2]);
					}
				}
			}
		}

		bool Delete(Node node) {
			int position = -1;
			for (int i = 0; i < nodes.size(); ++i) {
				if (node == nodes[i]) {
					position = i;
					break;
				}
			}
			if (position == -1) {
				for (int i = 0; i < nodes.size(); ++i) {
					if (nodes[i].sub_branch == nullptr) return false;
					if (nodes[i].sub_branch->Delete(node)) return true;
				}
			}
			else {
				if (nodes[position].sub_branch != nullptr) {
					vector<Node> old_nodes = nodes[position].sub_branch->nodes;
					for (int i = position; i < nodes.size() - 1; ++i) {
						nodes[i] = nodes[i + 1];
					}
					nodes.resize(nodes.size() - 1);
					MergeArrays(old_nodes, position);
					for (int i = 0; i < old_nodes.size() - 2; ++i) {
						nodes[position].sub_branch->nodes.push_back(old_nodes[i]);
					}
					sort(nodes[position].sub_branch->nodes.begin(), nodes[position].sub_branch->nodes.end());
					nodes[position].sub_branch->Add(old_nodes[old_nodes.size() - 2], true);
				}
				else {
					for (int i = position; i < nodes.size() - 1; ++i) {
						nodes[i] = nodes[i + 1];
					}
					nodes.resize(nodes.size() - 1);

				}
				if (prev_branch != nullptr) {
					Rebuild(*prev_branch);
				}
				return true;
			}
		}
	};

public:
	Branch first_branch;
	const static int n = 2;

	Tree() {
	}

	void AddValues(vector<int> values) {
		for (auto& val : values) {

			Node n(val);
			first_branch.Add(n);
		}
	}

	void Delete(int val) {
		Node v(val);
		first_branch.Delete(v);
	}

	friend ostream& operator<<(ostream& stream, Tree& tree) {
		stream << tree.first_branch << endl;
		if (tree.first_branch[0].sub_branch != nullptr) {
			for (int i = 0; i < tree.first_branch.GetSize(); ++i) {
				stream << *tree.first_branch[i].sub_branch << "\t";
			}
		}
		stream << endl;
		if (tree.first_branch[0].sub_branch->nodes[0].sub_branch != nullptr) {
			for (int i = 0; i < tree.first_branch.GetSize(); ++i) {
				for (int j = 0; j < tree.first_branch[i].sub_branch->nodes.size(); ++j) {
					stream << *tree.first_branch[i].sub_branch->nodes[j].sub_branch << "\t";
				}
			}
		}
		return stream << endl;
	}
};

int main() {
	Tree tree;
	vector<int> v;
	for (int i = 0; i < 30; ++i) {
		v.push_back(rand() % 30);
	}
	tree.AddValues(v);
	vector<int> v1{ 5,6 };

	tree.AddValues(v1);
	cout << tree << endl;
	cout << endl;
	tree.Delete(4);
	cout << tree;
}