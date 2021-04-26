#include <iostream>
#include <conio.h>



/// ////////////////////////////////////////////////////////////
int read(bool& inv)
{
	char stop;
	int value = 0;
	stop = _getch();
	bool sign = 0;
	if (stop == '-')
	{
		std::cout << "-";
		sign = 1;
	}
	else if ((int)stop >= 48 && (int)stop <= 57)
	{
		std::cout << stop;
		value = value * 10 + (stop - '0');
	}
	while (1)
	{
		stop = _getch();
		if ((int)stop >= 48 && (int)stop <= 57)
		{
			std::cout << stop;
			value = value * 10 + (stop - '0');
		}
		else if ((int)stop == 13)
		{
			inv = 1;
			break;
		}
		else if ((int)stop == 8)
		{
			std::cout << "\b \b";
			value = (value - value % 10) / 10;
		}
		else if ((int)stop == 32)
		{
			std::cout << " ";
			break;
		}
	}
	if (sign)
		return -value;
	else
		return value;
}
////////////////////////////////////////////////////////////////




class Tree
{
private:
	int elem[3];
	bool full;//сколько элементов содержит: 0 -> 1 элемент, 1 -> 2 элемента 
	Tree* left;
	Tree* middle;
	Tree* right;
	Tree* extra;
	Tree* parent;
	int size;//к-во детей
	void sort()//сортировка элементов
	{
		if (full == false)
		{
			if (elem[0] > elem[1])
			{
				std::swap(elem[0], elem[1]);
			}
		}
		else
		{
			if (elem[0] > elem[1])
				std::swap(elem[0], elem[1]);
			if (elem[1] > elem[2])
				std::swap(elem[1], elem[2]);
			if (elem[0] > elem[1])
				std::swap(elem[0], elem[1]);
		}
	}
	void chi_sort()// Сортировка связанных деревьев. WARNING! При одинаковых числах могут быть troubles
	{
		if (size == 2)
		{
			//2 ребенка
			//TODO: через swap
			if (middle)
			{
				if (left)
					right = middle;
				else
					left = middle;
				middle = 0;
			}
			if (extra)
			{
				if (left)
					right = extra;
				else
					left = extra;
				extra = 0;
			}
			if (left->elem[0] > right->elem[0])
			{
				std::swap(left, right);
			}
		}
		else if (size == 3)
		{
			//3ребенка
			if (extra)
			{
				if (!right)
				{
					std::swap(right, extra);
				}
				else if (!left)
				{
					std::swap(left, extra);
				}
				else
				{
					std::swap(middle, extra);
				}
				//теперь имеем значения в left, middle, right

				if (left->elem[0] > middle->elem[0])
					std::swap(left, middle);
				if (middle->elem[0] > right->elem[0])
					std::swap(middle, right);
				if (left->elem[0] > middle->elem[0])
					std::swap(left, middle);
			}
		}
		else if (size == 4)
		{
			//4 ребенка
			if (left->elem[0] > middle->elem[0])
				std::swap(left, right);
			if (middle->elem[0] > right->elem[0])
				std::swap(middle, right);
			if (right->elem[0] > extra->elem[0])
				std::swap(right, extra);

			if (left->elem[0] > middle->elem[0])
				std::swap(left, middle);
			if (middle->elem[0] > right->elem[0])
				std::swap(middle, right);
			if (left->elem[0] > middle->elem[0])
				std::swap(left, middle);
		}
	}
public:

	Tree(Tree* parent, int value)
	{
		this->parent = parent;
		elem[0] = value;
		left = 0;
		middle = 0;
		right = 0;
		extra = 0;
		size = 0;
		full = 0;
	}
	void show()
	{
		if (full)
		{
			std::cout << elem[0] << " " << elem[1] << " (" << size << ")     ";
		}
		else
		{
			std::cout << elem[0] << " (" << size << ")     ";
		}
	}

	friend Tree* search(Tree* node, int value);
	friend void div(Tree*& root, Tree*& node);
	friend void add(Tree*& root, int value);
	friend void show_all(Tree* root);
	friend void div(Tree*& root, Tree*& node);
};

Tree* search(Tree* node, int value)//поиск места для вставки
{
	//теоретически можно сократить код, но так понятнее, поэтому не буду
	if (node->left && node->right)//пока не лист
	{
		if (node->full)//если это 2-элемент
		{
			if (node->elem[0] > value)
				search(node->left, value);
			else if (value >= node->elem[1])
				search(node->right, value);
			else
				search(node = node->middle, value);
		}
		else//если это 1-элемент
		{
			if (node->elem[0] > value)
				search(node->left, value);
			else
				search(node->right, value);
		}
	}
	else
		return node;
}
void div(Tree*& root, Tree*& node)
{
	if (node->parent)
	{
		node->sort();
		node->parent->extra = new Tree(node->parent, node->elem[2]);
		node->parent->size++;

		if (node->left)
		{
			node->parent->extra->size = 2;
		}

		if (node->parent->size > 3)//если у родитея больше 3 детей
		{
			node->parent->elem[2] = node->elem[1];
			node->parent->sort();
			if (node->left)//если были дети - перераспределим
			{
				
				node->full = false;
				std::cout << "Ура! Ты болван! Перераспределяем! (В дурку)\n";
				node->chi_sort();
				node->parent->extra->left = node->right;
				node->parent->extra->right = node->extra;
				node->right = node->middle;
				node->middle = 0;
				node->extra = 0;
				node->size = 2;
				node->parent->extra->left->parent = node->parent->extra;
				node->parent->extra->right->parent = node->parent->extra;
			}
			else
			{
				std::cout << "У элемента нет детей, перераспределять не надо\n";
				node->size = 0;
				node->full = false;
			}
			div(root, node->parent);
		}
		else//тут просто добавляем родителю middle
		{
			std::cout << "Просто добавляем родителю middle\n";
			node->parent->elem[1] = node->elem[1];
			node->parent->sort();
			node->parent->size = 3;
			node->parent->full = true;
			node->full = false;
			if (node->left)
			{
				node->chi_sort();
				node->parent->extra->left = node->right;
				node->parent->extra->right = node->extra;
				node->right = node->middle;
				node->size = 2;
				node->parent->extra->left->parent = node->parent->extra;
				node->parent->extra->right->parent = node->parent->extra;
			}
			node->parent->chi_sort();
			
		}
	}
	else//создаем нового родителя
	{
		std::cout << "+родитель\n";
		node->sort();
		node->parent = new Tree(0, node->elem[1]);
		root = node->parent;
		root->size = 2;
		root->right = new Tree(root, node->elem[2]);

		root->left = node;//!!!

		if (node->left)
		{
			root->right->size = 2;
			node->size = 4;
			node->chi_sort();
			
			root->right->left = root->left->right;
			root->right->right = root->left->extra;

			root->left->right = root->left->middle;


			root->right->right->parent = root->right;
			root->right->left->parent = root->right->right->parent;

			root->left->size = 2;

			node->extra = 0;
			node->middle = 0;
		}
		root->left->full = false;
	}
}
void add(Tree*& root, int value)
{
	std::cout << "\nAdd " << value << "\n";
	Tree* node = search(root, value);
	if (!node->full)
	{
		node->elem[1] = value;
		node->sort();
		node->full = 1;
		std::cout << "Узел неполный, добавляю в лист \n";
	}
	else
	{
		node->elem[2] = value;
		node->full = true;
		node->sort();
		div(root, node);
	}
}
Tree* add_n()
{
	int n;
	std::cin >> n;
	int k;
	std::cin >> k;
	Tree* root = new Tree(0, k);
	for (int i = 1; i < n; i++)
	{
		std::cin >> k;
		add(root, k);
	}
	return root;
}


//////////////////////////////////////
struct Queue
{
	Queue(Tree* unit, int count)
	{
		this->unit = unit;
		rétrograder = count;
		Queue* next = 0;
	}
	Tree* unit;

	int rétrograder;
	Queue* next;
};
void show_all(Tree* root)
{
	if (root)
	{
		if (root->left)
		{
			Queue* reg = new Queue(root, 1);
			Queue* head = reg;
			Queue* fullness = reg;

			int count = root->size;
			int n_count = 0;
			int i = 0;
			while (47)
			{
				std::cout << count << "\n";
				if (reg->unit->left)
				{
					fullness->next = new Queue(reg->unit->left, count);
					n_count += reg->unit->left->size;
					i++;
					fullness = fullness->next;

					if (reg->unit->size == 3)
					{
						fullness->next = new Queue(reg->unit->middle, count);
						fullness = fullness->next;
						i++;
						n_count += reg->unit->middle->size;
					}
					fullness->next = new Queue(reg->unit->right, count);
					n_count += reg->unit->right->size;
					fullness = fullness->next;
					i++;
					reg = reg->next;

					if (i == count)
					{
						count = n_count;
						n_count = 0;
						i = 0;
					}
				}
				else
				{
					fullness->next = 0;
					break;
				}
			}

			std::cout << "\n\n";

			fullness = head;
			count = 1;
			n_count = 0;
			while (fullness)
			{
				fullness->unit->show();
				n_count++;
				if (n_count == fullness->rétrograder)
				{
					std::cout << "\n";
					n_count = 0;
				}
				fullness = fullness->next;
				delete head->unit;
				delete head;
				head = fullness;
			}
		}
		else
		{
			root->show();
			delete root;
		}
		root = 0;
	}
}
//////////////////////////////////////





int main()
{
	setlocale(LC_ALL, "rus");
	Tree* root = add_n();
	std::cout << std::endl;
	show_all(root);
	root = nullptr;
	return 0;
}