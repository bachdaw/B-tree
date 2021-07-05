#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#define MAX_NUMBER_LENGTH 20
#define NOT_FOUND -1
#pragma warning(disable : 4996)

struct Node
{
	bool isLeaf;
	int* keys;
	Node** children;
	int count;
	Node* parent;
};

Node* InitNode(const int t)
{
	const int max_children = 2 * t;
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->keys = (int*)malloc((max_children - 1) * sizeof(int));
	new_node->children = (Node**)malloc(max_children * sizeof(Node*));
	new_node->parent = NULL;
	new_node->count = 0;
	new_node->isLeaf = true;
	for (int i = 0; i < max_children; i++)
		new_node->children[i] = NULL;

	return new_node;
}

int FindIndex(Node* node, const int key)
{
	int i = 0;
	while (i < node->count && key > node->keys[i])
		i++;
	if (key != node->keys[i] && node->isLeaf)
		return NOT_FOUND;
	return i;
}

Node* SplitNode(Node* node, Node*& root, int key, const int t)
{
	if (node->parent == NULL)
	{
		Node* parent_node = InitNode(t);
		Node* right_node = InitNode(t);

		parent_node->keys[0] = node->keys[t - 1];
		parent_node->children[0] = node;
		parent_node->children[1] = right_node;
		parent_node->count = 1;
		parent_node->isLeaf = false;

		for (int i = 0; i < t - 1; i++)
			right_node->keys[i] = node->keys[i + t];
		for (int i = 0; i < t; i++)
		{
			right_node->children[i] = node->children[i + t];
			if (node->children[i + t] != NULL)
			{
				right_node->children[i]->parent = right_node;
				right_node->isLeaf = false;
			}
			node->children[i + t] = NULL;
		}
		right_node->parent = parent_node;
		right_node->count = t - 1;

		node->parent = parent_node;
		node->count = t - 1;

		root = parent_node;

		if (key > root->keys[0]) return right_node;
		else return node;
	}
	else
	{
		Node* right_node = InitNode(t);
		for (int i = 0; i < t - 1; i++)
			right_node->keys[i] = node->keys[i + t];
		for (int i = 0; i < t; i++)
		{
			right_node->children[i] = node->children[i + t];
			if (node->children[i + t] != NULL)
			{
				right_node->children[i]->parent = right_node;
				right_node->isLeaf = false;
			}
			node->children[i + t] = NULL;
		}
		right_node->count = t - 1;
		right_node->parent = node->parent;

		node->count = t - 1;
		int i = 0;
		while (i < node->parent->count && node->keys[t - 1] > node->parent->keys[i])
			i++;
		for (int j = node->parent->count + 1; j > i + 1; j--)
			node->parent->children[j] = node->parent->children[j - 1];

		node->parent->children[i] = node;
		node->parent->children[i + 1] = right_node;
		for (int j = node->parent->count - 1; j >= i; j--)
			node->parent->keys[j + 1] = node->parent->keys[j];

		node->parent->keys[i] = node->keys[t - 1];
		node->parent->count++;

		i = FindIndex(node->parent, key);
		return node->parent->children[i];
	}
}


void SetKey(Node* node, Node*& root, int key, const int t)
{
	const int max_children = 2 * t;
	while (node->count >= max_children - 1)
		node = SplitNode(node, root, key, t);

	int i = 0;
	while (i < node->count && key > node->keys[i])
		i++;

	if (node->isLeaf == true)
	{
		if (node->count >= max_children - 1)
		{
			node = SplitNode(node, root, key, t);
			i = 0;
			while (i < node->count && key > node->keys[i])
				i++;
			for (int j = node->count - 1; j >= i; j--)
				node->keys[j + 1] = node->keys[j];

			node->keys[i] = key;
			node->count++;
		}
		else
		{
			i = 0;
			while (i < node->count && key > node->keys[i])
				i++;

			for (int j = node->count - 1; j >= i; j--)
				node->keys[j + 1] = node->keys[j];

			node->keys[i] = key;
			node->count++;
		}
	}
	else SetKey(node->children[i], root, key, t);
}

void Insert(Node*& root, const int t, const int key)
{
	const int max_children = 2 * t;
	if (root == NULL)
	{
		root = (Node*)malloc(sizeof(Node));
		root->keys = (int*)malloc((max_children - 1) * sizeof(int));
		root->children = (Node**)malloc(max_children * sizeof(Node*));
		root->parent = NULL;
		root->isLeaf = true;
		for (int i = 0; i < max_children; i++)
			root->children[i] = NULL;

		root->count = 1;
		root->keys[0] = key;
	}
	else
	{
		Node* node = root;
		SetKey(node, root, key, t);
	}
}

void PrintNode(Node* node)
{
	if (node == NULL) return;
	int i;
	for (i = 0; i < node->count; i++)
	{
		if (node->isLeaf == false)
			PrintNode(node->children[i]);
		printf("%d ", node->keys[i]);
	}
	if (node->isLeaf == false)
		PrintNode(node->children[i]);
}

Node* Find(Node* node, const int key)
{
	if (node == NULL) return NULL;
	int i = FindIndex(node, key);
	if (i < node->count && node->keys[i] == key)
		return node;
	if (node->isLeaf == true)
		return NULL;

	return Find(node->children[i], key);
}

int ProccesString(Node*& node, Node* parent, std::string& input, const int curr, const int t)
{
	node = InitNode(t);
	node->parent = parent;

	int i = curr;
	int size = input.length();
	while (i < size && input[i] != '\0')
	{
		if (input[i] >= '0' && input[i] <= '9')
		{
			char number_buff[MAX_NUMBER_LENGTH];
			int number_index = 0;
			while (input[i] != ')' && input[i] != '(')
			{
				if (input[i] == ' ')
				{
					node->keys[node->count] = atoi(number_buff);
					node->count++;

					int j = 0;
					while (number_buff[j] >= '0' && number_buff[j] <= '9')
					{
						number_buff[j] = '\n';
						j++;
					}
					number_index = 0;
				}
				else
				{
					number_buff[number_index] = input[i];
					number_index++;
				}
				i++;
			}
		}
		else if (input[i] == '(')
		{
			node->isLeaf = false;
			i++;
			i = ProccesString(node->children[node->count], node, input, i, t);
		}
		else if (input[i] == ' ')
			i++;
		else if (input[i] == ')') return i + 1;
	}
	return input.length();
}

void Load(Node*& root, int& t)
{
	scanf("%d\n", &t);
	std::string* input = new std::string;
	std::getline(std::cin, *input);
	ProccesString(root, NULL, *input, 1, t);
	delete input;
}

void Save(Node* node)
{
	if (node == NULL) return;
	int i;
	printf("( ");
	for (i = 0; i < node->count; i++)
	{
		if (node->isLeaf == false)
			Save(node->children[i]);
		printf("%d ", node->keys[i]);
	}
	if (node->isLeaf == false)
		Save(node->children[i]);
	printf(") ");
}

int GetPredecssor(Node* node, const int t, const bool isCopy)
{
	while (node->isLeaf != true)
		node = node->children[node->count];

	if (isCopy) return node->keys[node->count - 1];
	if (node->count > t - 1)
	{
		int temp = node->keys[node->count - 1];
		node->count--;
		return temp;
	}
	return NOT_FOUND;
}

int GetSuccessor(Node* node, const int t)
{
	while (node->isLeaf != true && node->children[node->count] != NULL)
		node = node->children[0];

	if (node->count > t - 1)
	{
		int temp = node->keys[0];
		for (int i = 0; i < node->count; i++)
			node->keys[i] = node->keys[i + 1];
		node->count--;
		return temp;
	}
	return NOT_FOUND;
}

Node* BorrowRight(Node*& parent, const int curr, const int t)
{
	Node* actual = parent->children[curr];
	Node* right_sibling = parent->children[curr + 1];

	actual->keys[t - 1] = parent->keys[curr];
	actual->children[t] = right_sibling->children[0];
	if (right_sibling->children[0] != NULL) right_sibling->children[0]->parent = actual;
	right_sibling->children[0] = NULL;
	actual->count = t;

	parent->keys[curr] = right_sibling->keys[0];

	right_sibling->count--;
	for (int i = 0; i < right_sibling->count; i++)
		right_sibling->keys[i] = right_sibling->keys[i + 1];
	for (int i = 0; i < right_sibling->count + 1; i++)
		right_sibling->children[i] = right_sibling->children[i + 1];
	right_sibling->children[right_sibling->count + 1] = NULL;
	return actual;
}

Node* BorrowLeft(Node*& parent, const int curr, const int t)
{
	Node* actual = parent->children[curr];
	Node* left_sibling = parent->children[curr - 1];

	for (int i = t - 1; i > 0; i--)
		actual->keys[i] = actual->keys[i - 1];
	for (int i = t; i > 0; i--)
		actual->children[i] = actual->children[i - 1];
	actual->count = t;

	actual->keys[0] = parent->keys[curr - 1];
	parent->keys[curr - 1] = left_sibling->keys[left_sibling->count - 1];

	actual->children[0] = left_sibling->children[left_sibling->count];
	if (left_sibling->children[left_sibling->count] != NULL) left_sibling->children[left_sibling->count]->parent = actual;
	left_sibling->children[left_sibling->count] = NULL;
	left_sibling->count--;
	return actual;
}

Node* MergeSiblings(Node*& parent, const int curr, const int t, bool with_parent_key)
{
	if (curr > 0 && parent->children[curr - 1] != NULL)
	{
		Node* left_sibling = parent->children[curr - 1];
		int shift = 1;
		left_sibling->count = t - 1;
		if (with_parent_key == true)
		{
			left_sibling->keys[left_sibling->count] = parent->keys[curr - 1];
			left_sibling->count++;
			shift = 0;
		}
		for (int i = 0; i < t - 1; i++)
		{
			left_sibling->keys[left_sibling->count] = parent->children[curr]->keys[i];
			left_sibling->children[left_sibling->count + shift] = parent->children[curr]->children[i];
			if (parent->children[curr]->children[i] != NULL) parent->children[curr]->children[i]->parent = left_sibling;
			parent->children[curr]->children[i] = NULL;
			left_sibling->count++;
		}
		left_sibling->children[left_sibling->count] = parent->children[curr]->children[parent->children[curr]->count];
		if (parent->children[curr]->children[parent->children[curr]->count] != NULL)
			parent->children[curr]->children[parent->children[curr]->count]->parent = left_sibling;
		parent->children[curr]->children[parent->children[curr]->count] = NULL;
		parent->children[curr] = NULL;

		for (int i = curr - 1; i < parent->count - 1; i++)
			parent->keys[i] = parent->keys[i + 1];
		for (int i = curr; i < parent->count; i++)
			parent->children[i] = parent->children[i + 1];
		parent->children[parent->count] = NULL;
		parent->count--;
		return left_sibling;
	}
	else
	{
		Node* right_sibling = parent->children[curr + 1];
		Node* current = parent->children[curr];
		int shift = 1;
		current->count = t - 1;
		if (with_parent_key == true)
		{
			current->keys[current->count] = parent->keys[curr];
			current->count++;
			shift = 0;
		}
		for (int i = 0; i < t - 1; i++)
		{
			current->keys[current->count] = right_sibling->keys[i];
			current->children[current->count + shift] = right_sibling->children[i];
			if (right_sibling->children[i] != NULL) right_sibling->children[i]->parent = current;
			right_sibling->children[i] = NULL;
			current->count++;
		}
		current->children[current->count] = right_sibling->children[right_sibling->count];
		if (right_sibling->children[right_sibling->count] != NULL)
			right_sibling->children[right_sibling->count]->parent = current;
		right_sibling->children[right_sibling->count] = NULL;
		parent->children[curr + 1] = NULL;

		for (int i = curr; i < parent->count - 1; i++)
			parent->keys[i] = parent->keys[i + 1];
		for (int i = curr + 1; i < parent->count; i++)
			parent->children[i] = parent->children[i + 1];
		parent->children[parent->count] = NULL;
		parent->count--;
		return current;
	}
}

Node* FillNode(Node*& node, Node*& root, int curr, const int t)
{
	Node* temp = node;
	if (temp->parent == NULL) return temp;
	else
	{
		if (curr > 0 && temp->parent->children[curr - 1]->count > t - 1)
		{
			temp = BorrowLeft(temp->parent, curr, t);
			return temp;
		}
		else if (curr < ((2 * t) - 1) && temp->parent->count > curr && temp->parent->children[curr + 1]->count > t - 1)
		{
			temp = BorrowRight(temp->parent, curr, t);
			return temp;
		}
		else
		{
			temp = MergeSiblings(temp->parent, curr, t, true);
			if (temp->parent->count <= 0)
			{
				free(root->children);
				free(root->keys);
				free(root);
				temp->parent = NULL;
				root = temp;
				return temp;
			}
			return temp;
		}
	}
}

void RemoveFromLeaf(Node*& node, Node*& root, const int key)
{
	int i = FindIndex(node, key);
	for (int j = i; j < node->count - 1; j++)
		node->keys[j] = node->keys[j + 1];
	node->count--;
	if (node == root && node->count == 0)
	{
		free(node->children);
		free(node->keys);
		free(node);
		root = NULL;
	}
}

void RemoveKey(Node*& node, Node*& root, const int t, const int key)
{
	Node* temp = node;
	if (Find(temp, key) == NULL) return;
	else
	{
		const int min_children = t - 1;
		int i = 0;
		int at_parent_index = 0;
		while (key != temp->keys[i])
		{
			i = FindIndex(temp, key);
			if (i == NOT_FOUND)
			{
				if (Find(temp, key) == NULL) return;
				else
				{
					temp = root;
					continue;
				}
			}
			if (temp == root)
			{
				if (key == temp->keys[i]) break;
				else
				{
					temp = temp->children[i];
					continue;
				}
			}
			if (temp->count == min_children)
			{
				at_parent_index = FindIndex(temp->parent, key);
				temp = FillNode(temp, root, at_parent_index, t);
				i = 0;
			}
			else if (key != temp->keys[i] && temp->isLeaf == false) temp = temp->children[i];
		}

		if (temp->count == min_children && temp != root)
		{
			at_parent_index = FindIndex(temp->parent, key);
			temp = FillNode(temp, root, at_parent_index, t);
		}

		if (temp->isLeaf == true)
		{
			RemoveFromLeaf(temp, root, key);
			return;
		}
		else
		{
			i = FindIndex(temp, key);
			int substitute = GetPredecssor(temp->children[i], t, false);
			if (substitute != NOT_FOUND)
			{
				temp->keys[i] = substitute;
				return;
			}
			if (temp->children[i + 1] != NULL)
				substitute = GetSuccessor(temp->children[i + 1], t);
			if (substitute != NOT_FOUND)
			{
				temp->keys[i] = substitute;
				return;
			}
			else
			{
				i = FindIndex(temp, key);
				int precedessor = GetPredecssor(temp->children[i], t, true);
				temp = temp->children[i];
				while (temp->isLeaf != true)
				{
					i = FindIndex(temp, key);
					precedessor = GetPredecssor(temp->children[temp->count], t, true);
					if (temp->count == min_children)
					{
						at_parent_index = FindIndex(temp->parent, precedessor);
						temp = FillNode(temp, root, at_parent_index, t);
						i = 0;
						continue;
					}
					temp = temp->children[temp->count];
				}

				if (temp->count == min_children && temp != root)
				{
					at_parent_index = FindIndex(temp->parent, precedessor);
					FillNode(temp, root, at_parent_index, t);
				}
				RemoveKey(root, root, t, key);
			}
		}
	}
}

void FreeMemory(Node* node, const int t)
{
	if (node == NULL) return;
	const int max_children = 2 * t;
	for (int i = 0; i < node->count + 1; i++)
	{
		if (i > max_children - 1) break;
		if (node->isLeaf == false && node->children[i] != NULL && node->children[i])
			FreeMemory(node->children[i], t);
	}
	free(node->children);
	free(node->keys);
	free(node);
}

bool SearchCache(int*& arr, const int arr_size, const int key)
{
	for (int i = 0; i < arr_size; i++)
	{
		if (arr[i] == key)
			return true;
	}
	for (int i = arr_size - 1; i > 0; i--)
		arr[i] = arr[i - 1];
	arr[0] = key;
	return false;
}

int FindCache(Node* node, const int key)
{
	int counter = 0;
	if (node == NULL) return 0;
	counter++;
	int i = FindIndex(node, key);
	if (i < node->count && node->keys[i] == key)
		return counter;
	if (node->isLeaf == true)
		return counter;
	return counter + FindCache(node->children[i], key);
}

void Cache(Node* node)
{
	std::string* input = new std::string;
	std::getline(std::cin, *input);
	char* ch_input = &(*input)[0];
	char* token = strtok(ch_input, " ");

	const int arr_size = atoi(token);
	int* cache_arr = (int*)malloc(arr_size * sizeof(int));
	unsigned int no_cache = 0;
	unsigned int with_cache = 0;
	unsigned int height = 0;

	for (int i = 0; i < arr_size; i++)
		cache_arr[i] = -1;

	token = strtok(NULL, " ");
	while (token != NULL)
	{
		height = FindCache(node, atoi(token));
		no_cache += height;

		if (!SearchCache(cache_arr, arr_size, atoi(token))) with_cache += height;
		token = strtok(NULL, " ");
	}
	printf("NO CACHE: %u CACHE: %u\n", no_cache, with_cache);
	delete input;
	free(cache_arr);
}


int main()
{
	Node* root = NULL;
	int t;
	char command;
	int key;
	while (scanf("%c", &command) != EOF)
	{
		if (command == 'X') break;
		switch (command)
		{
		case 'I':
			scanf("%d", &t);
			break;
		case 'A':
			scanf("%d", &key);
			Insert(root, t, key);
			break;
		case '?':
			scanf("%d", &key);
			printf("%d ", key);
			if (Find(root, key) == NULL)
				printf("-\n");
			else
				printf("+\n");
			break;
		case 'P':
			PrintNode(root);
			printf("\n");
			break;
		case 'L':
			Load(root, t);
			break;
		case 'S':
			printf("%d\n", t);
			Save(root);
			printf("\n");
			break;
		case 'R':
			scanf("%d", &key);
			RemoveKey(root, root, t, key);
			break;
		case 'C':
			Cache(root);
			break;
		}
	}
	FreeMemory(root, t);
}

