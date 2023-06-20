#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <time.h>
using namespace std;


template <typename K, typename U>
struct TreeNode {

	K keyword;
	/*U url;*/
	list<string> url_list;
	TreeNode* lChild;
	TreeNode* rChild;

	TreeNode() {
		this->lChild = this->rChild = nullptr;
	}

	TreeNode(K keyword, U url) {
		this->keyword = keyword;
		/*this->url = url;*/
		this->lChild = this->rChild = nullptr;
		this->url_list.push_back(url);
	}

	bool isLeaf() {
		return !this->lChild && !this->rChild;
	}

};

template <typename K, typename U>
class BinarySearchTree {

private:

	TreeNode<K, U>* root;

	void inorderPrintKeys(TreeNode<K, U>* ptr) {
		if (ptr) {
			inorderPrintKeys(ptr->lChild);
			cout << "Word: " << ptr->keyword << endl;
			//
			cout << "URL List for the Word: " << endl;
			list <string> ::iterator it; //create an iterator according to the data structure
			for (it = ptr->url_list.begin(); it != ptr->url_list.end(); it++) {
				cout << *it << " " << endl;
			}
			//
			inorderPrintKeys(ptr->rChild);
		}
	}

	void insert(K keyword, U url, TreeNode<K, U>*& ptr) {
		if (ptr == nullptr) {
			ptr = new TreeNode<K, U>(keyword, url);
		}
		else if (keyword < ptr->keyword) {
			insert(keyword, url, ptr->lChild);
		}
		else if (keyword > ptr->keyword) {
			insert(keyword, url, ptr->rChild);
		}
	}

	void search(K keyword, TreeNode<K, U>* ptr) {
		if (ptr == nullptr) {
			cout << "The entered Key Word does not exist in the Data Base !" << endl << endl;
		}
		else if (keyword < ptr->keyword) {
			search(keyword, ptr->lChild);
		}
		else if (keyword > ptr->keyword) {
			search(keyword, ptr->rChild);
		}
		else {
			cout << ptr->url_list.size() << " result(s) found" << endl << endl; //insert time here
			list <string> ::iterator it; //create an iterator according to the data structure
			int i = 1;
			for (it = ptr->url_list.begin(); it != ptr->url_list.end(); it++) {
				cout << i << ". " << *it << endl;
				i++;
			}
			cout << endl;
		}
	}

	bool search_append(K keyword, U url, TreeNode<K, U>* ptr) {
		if (ptr == nullptr) {
			return false;
		}
		else if (keyword < ptr->keyword) {
			return this->search_append(keyword, url, ptr->lChild);
		}
		else if (keyword > ptr->keyword) {
			return this->search_append(keyword, url, ptr->rChild);
		}
		else {
			if (url != ptr->url_list.back()) {
				ptr->url_list.push_back(url);
			}
			return true;
		}
	}

	void delete_(K keyword, TreeNode<K, U>*& ptr) {
		if (ptr == nullptr) {
			return;
		}
		else if (keyword < ptr->keyword) {
			delete_(keyword, ptr->lChild);
		}
		else if (keyword > ptr->keyword) {
			delete_(keyword, ptr->rChild);
		}
		else {
			//case 0: leaf node
			if (ptr->isLeaf()) {
				delete ptr;
				ptr = nullptr;
			}
			//case 1.1: only left child exists
			else if (ptr->lChild && !ptr->rChild) {
				TreeNode<K, U>* delNode = ptr;
				ptr = ptr->lChild;
				delete delNode;
			}
			//case 1.2: only right child exists
			else if (!ptr->lChild && ptr->rChild) {
				TreeNode<K, U>* delNode = ptr;
				ptr = ptr->rChild;
				delete delNode;
			}
			//case 2: both children exits
			else {
				TreeNode<K, U>* successor = ptr->rChild;
				while (successor->lChild) {
					successor = successor->lChild;
				}
				ptr->keyword = successor->keyword;
				ptr->url = successor->url;
				delete_(successor->keyword, ptr->rChild);
			}
		}
	}//end of delete

	void deleteAll(TreeNode<K, U>* ptr) {
		if (ptr)
		{
			deleteAll(ptr->lChild);
			deleteAll(ptr->rChild);
			delete ptr;
		}
	}


public:

	BinarySearchTree() {
		this->root = nullptr;
	}

	void inorderPrintKeys() {
		inorderPrintKeys(this->root);
	}

	void insert(K keyword, U url) {
		bool flag = search_append(keyword, url, this->root);
		if (flag == false) {
			insert(keyword, url, this->root);
		}
	}

	void search(K keyword) {
		search(keyword, this->root);
	}

	void delete_(K keyword) {
		delete_(keyword, this->root);
	}

	~BinarySearchTree() {
		this->deleteAll(this->root);
	}

};


int main() {

	cout << ".......... Welcome to Search Engine .........." << endl << endl;

	BinarySearchTree <string, string> SearchEngine;

	string filename;
	string keyword;
	string line;
	string url;
	string input = "";
	int keyword_size = 0;
	int keyword_start_index = 0;
	int urlcount = 0;
	int seconds;
	double time;
	bool flag = true;
	clock_t tStart;

	cout << "Enter the name of the file: ";
	getline(cin, filename);
	cout << endl;

	tStart = clock();

	ifstream in;
	in.open(filename);
	if (in.is_open() == false) {
		cout << "The file could not be opened or does not exist !" << endl << endl;
		flag = false;
	}
	else {
		while (!in.eof()) {
			getline(in, line);
			if (line != "") {
				if (line[2] == 'h' && line[3] == 't' && line[4] == 't' && line[5] == 'p' && line[6] == ':' && line[7] == '/' && line[8] == '/') {
					url = line.substr(2, (line.length() - 2));
					urlcount++;
					/*cout << "URL: " << url << endl;*/
				}
				else if (line[3] == 'h' && line[4] == 't' && line[5] == 't' && line[6] == 'p' && line[7] == ':' && line[8] == '/' && line[9] == '/') {
					url = line.substr(3, (line.length() - 3));
					urlcount++;
					/*cout << "URL: " << url << endl;*/
				}
				else {
					for (int line_index = 0; line_index < line.length(); ) {
						if (line_index == (line.length() - 1)) {
							keyword_size++;
							keyword = line.substr(keyword_start_index, keyword_size);
							SearchEngine.insert(keyword, url);
							line_index++;
							keyword_start_index = 0;
							keyword_size = 0;
							/*cout << keyword;*/
						}
						else if (line[line_index] != ' ') {
							keyword_size++;
							line_index++;
						}
						else if (line[line_index] == ' ') {
							keyword = line.substr(keyword_start_index, keyword_size);
							SearchEngine.insert(keyword, url);
							line_index++;
							keyword_start_index = line_index;
							keyword_size = 0;
							//cout << keyword << " ";
						}
					}
					/*cout << endl;*/
				}
			}
		}
	}
	in.close();

	time = ((double)(clock() - tStart)) / CLOCKS_PER_SEC;
	seconds = time;
	time = time - seconds;

	cout << "File loaded into the BST successfully. " << urlcount << " URLs loaded from the file in " << seconds << " seconds and " << time * 1000000 << " microseconds." << endl << endl; //insert time here

	if (flag == true) {
		while (input != "-") {
			cout << "Please enter a word to search, or enter '-' to exit: ";
			getline(cin, input);
			cout << endl;
			if (input != "-") {
				tStart = clock();
				SearchEngine.search(input);
				time = ((double)(clock() - tStart)) / CLOCKS_PER_SEC;
				seconds = time;
				time = time - seconds;
				cout << "Time taken to find the URLs: " << seconds << " seconds and " << time * 1000000 << " microseconds." << endl << endl; //insert time here
			}
		}
		cout << ".......... Program Terminated .........." << endl << endl;
	}

	/*SearchEngine.inorderPrintKeys();*/

	system("pause");
}