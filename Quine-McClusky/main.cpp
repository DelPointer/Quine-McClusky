#include <iostream>
#include <Windows.h>	// system()
#include <conio.h>		// _getch()
#include <iomanip>		// cout format
#include <algorithm>	// sort(), copy()
#include <random>		
using namespace std;

typedef struct Node {
	int decimals[16];	// ���յ� ����� 10����
	int bits[4];		// 2����
	bool used;			// ���տ� ����� �ƴ°�?
	struct Node* next;	// ���� ���
}Node;

int numArr[16] = { 0 };	// �Է���
int circuitRow = 0;		// ȸ�ε� ����

/*		�Լ� ����� ����		*/
void PrintColumn(Node* column, bool cdt);	// Į�� ���
void PrintCircuit(int** graph, Node* PI);	// ȸ�ε� ���
void PrintFx(Node* EPI);					// ������ ���
void ChoiceNum();							// �Է���
void Grouping(Node* column);				// �׷�����
void Merge(Node* column1, Node* column2);	// �պ��Ѱ� ����
void DeleteOverlap(Node* column);			// �պ��߿� �ߺ��ִ°� ����
void PI(Node* column1, Node* column2, Node* PI); // PI����
void EPI(Node* PI, int** graph, Node* EPI);	// EPI����
int** CurcuitDiagram(Node* PI);				// ȸ�ν�
void Free(Node* column);					// Į���� �ִ� ������ ������
int  CountBit(Node* node);					// ��Ʈ ī����
void cls();									// ��ũ�� Ŭ����
void gotoxy(int x, int y);					// gotoxy()
/*		�Լ� ����� ��		*/	

void main() {
	
	Node* table	  = new Node[sizeof(Node)];	// ���� ���̺�
	Node* column1 = new Node[sizeof(Node)];	// 1�� �з�
	Node* column2 = new Node[sizeof(Node)]; // 2�� �з�
	Node* column3 = new Node[sizeof(Node)]; // 3�� �з�
	Node* column4 = new Node[sizeof(Node)]; // 4�� �з�	�ִ�
	Node* pi	  = new Node[sizeof(Node)]; // PI 
	Node* epi	  = new Node[sizeof(Node)];	// EPI

	int level   = 0;	// ���� ����� ���� ��
	int page	= 0;	// ���� �ܰ�
	int key		= 0;
	int result  = 0;
	int** graph_1 = NULL;		// PI ȸ�ε�
	int** graph_2 = NULL;		// EPI ȸ�ε�

	table->next   = NULL;
	column1->next = NULL;
	column2->next = NULL;
	column3->next = NULL;
	column4->next = NULL;
	pi->next	  = NULL;
	epi->next	  = NULL;
	
	ChoiceNum();
	Grouping(table);

	Merge(table, column1);		// 1�� ���� ���
	Merge(column1, column2);	// 2�� ���� ���
	Merge(column2, column3);	// 3�� ���� ���
	Merge(column3, column4);	// 4�� ���� ���
	
	while (key != 27) {
		cls();
		if (page == 0) { // ���� ���̺�
			PrintColumn(table, true);
			cout << "���� ���̺�";
		}
		else if (page == 1) {	// 1�� Į��
			if (column1->next != NULL) {
				PrintColumn(column1, false);
				cout << "1�� ����";
			}
			
			else {
				cout << "���յ� Column�� �����ϴ�. ���α׷��� �����մϴ�.";
				return;
			}
		}
		else if (page == 2) {	// 2�� Į��
			if (column2->next != NULL) {
				PrintColumn(column2, false);
				cout << "2�� ����";
				result = 1;
			}
			else page = 5;
		}
		else if (page == 3) {	// 3�� Į��	
			if (column3->next != NULL) {
				PrintColumn(column3, false);
				cout << "3�� ����";
				result = 2;
			}
			else {
				page = 5;
				result = 1;
			}
		}
		else if (page == 4) {	// 4�� Į��
			if (column4->next != NULL) {
				PrintColumn(column4, false);
				cout << "4�� ����";
				result = 3;
			}
			else {
				page = 5;
				result = 2;
			}
		}
		if (page == 5) {	// pi
			if (result == 1 && pi->next == NULL) PI(column1, column2, pi);
			if (result == 2 && pi->next == NULL) PI(column2, column3, pi);
			if (result == 3 && pi->next == NULL) PI(column3, column4, pi);
			if (graph_1 == NULL && graph_2 == NULL) {
				graph_1 = CurcuitDiagram(pi);
				graph_2 = new int* [sizeof(int) * circuitRow];
				for (int i = 0; i < circuitRow; i++) {
					graph_2[i] = new int[sizeof(int) * 16];
					copy(graph_1[i], graph_1[i] + 16, graph_2[i]);
				}
			}

			PrintColumn(pi, false);
			cout << "PI ����";
		}
		else if (page == 6) {	// graph_1
			PrintCircuit(graph_1, pi);
			cout << "�׷���_1";
		}
		else if (page == 7) {	// graph_2
			if (epi->next == NULL) EPI(pi, graph_2, epi);
			PrintCircuit(graph_2, pi);
			cout << "�׷���_2";
		}
		else if (page == 8) {	// �����
			
			PrintFx(epi);
		}

		key = _getch();
		if (key == 72 && page > 0) {
			if (page == 5) {
				if (result == 1) page = 2;
				else if (result == 2) page = 3;
				else if (result == 3) page = 4;
			}
			else page--;
		}
		if (key == 80 && page < 8) page++;
	}
}

/*		�Լ� ���Ǻ� ����		*/
/*
* @brief	���ڷ� ���� column�� ���
* @return	void
* @date		
*	2022-05-27
*	2022-05-29
* @params
*	column : ����� column						_IN_
*	cdt	   : ����� �� ��Ʈ 1�� ������ ǥ�� ����	_IN_
*/
void PrintColumn(Node* column, bool cdt) {
	Node* next = column->next;
	int bit = -1;
	int curbit = -1;
	while (next != NULL) {
		// ���� ���
		// [1 ,] 0 0 0 1 
		
		bit = CountBit(next);
		if (cdt == true) {
			if (bit != curbit) {
				cout << left << setw(2) << bit << "| ";
				curbit = bit;
			}
			else {
				cout << "  | ";
			}
		}

		cout << "[";
		int last = 0;
		for(int i = 0; i < 16; i++) 
			if (next->decimals[i] == 1) 
				last = i;
			
		for (int i = 0; i < 16; i++) //group ���
		{
			// left << setw(2) : 2ĭ ���� ���� ���
			if (last == i) cout << left << setw(2) << i;
			else if (next->decimals[i] == 1) cout << left << setw(2) << i << ",";
		}
		cout << "] ";
		for (int i = 0; i < 4; i++) //bit ���
		{
			if (next->bits[i] != -1) cout << left << setw(2) << next->bits[i];
			else cout << "- ";
		}
		//if (next->used) cout << "V";
		cout << endl;

		next = next->next;
	}
}

/*
* @brief	ȸ�ε� ���
* @date		2022-05-29
* @return	void
* @params
*	graph : ����� ȸ�ε�
*	PI : ������ ����� PI
*/
void PrintCircuit(int** graph, Node* PI) {
	Node* cur = PI->next;

	gotoxy(17, 0);
	for (int i = 0; i < 16; i++) {
		cout << left << setw(3) << i;
	}
	cout << endl;

	for (int i = 0; i < circuitRow; i++) {
		int arr[16], count = 0;
		cout << "[";
		for (int j = 0; j < 16; j++) {
			if (cur->decimals[j] == 1) {
				arr[count++] = j;
			}
		}
		for (int j = 0; j < count; j++) {
			cout << left << setw(2) << arr[j] << ", ";
		}
		gotoxy(15, i + 1);
		cout << "]";
		gotoxy(17, i + 1);
		for (int j = 0; j < 16; j++) {
			if (graph[i][j] == 1) {
				cout << "X, ";
			}
			else cout << " , ";
		}
		cout << endl;

		cur = cur->next;
	}
}

/*
* @brief	EPI�� �̿��� ���� ����
* @date		2022-05-30
* @return	void
* @params
*	EPI : Essential PI
*/
void PrintFx(Node* EPI) {
	Node* cur = EPI->next;
	Node* next = cur->next;
	cout << "F(x) = ";
	
	while (cur != NULL) {
		for (int i = 0; i < 4; i++) {
			if (cur->bits[i] == -1) continue;
			cout << (char)(i + 'A');
			if (cur->bits[i] == 0) cout << "'";
		}
		cur = cur->next;
		if (next != NULL) {
			next = cur->next;
			cout << " + ";
		}
		
	}
}

/*
* @breif	���� �Է�
* @date	
*	2022-05-27
*	2022-05-30
* @return	void
* @param	void
*/
void ChoiceNum() {
	int mode;
	int count;
	int key = -1;
	cout << "�Է��� ����(0: �ڵ�, 1: ����) "; cin >> mode;
	cout << "�Է��� ���� ����(�ִ� 16��) "; cin >> count;

	if (mode == 0) { // �ڵ�
		// �õ尪�� ��� ���� random_device ����.
		std::random_device rd;

		// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
		std::mt19937 gen(rd());

		// 0 ���� 99 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
		std::uniform_int_distribution<int> dis(0, 15);

		for (int i = 0; i < count;) {
			int random = dis(gen);
			if (numArr[random] == 0) {
				numArr[random] = 1;
				++i;
			}
		}
		for (int i = 0; i < 16; i++) {
			if(numArr[i] == 1)cout << i << endl;
		}
		
	}
	else if (mode == 1) {
		for (int i = 0; i < count;) {
			int temp = -1;
			cin >> temp;
			if (0 <= temp && temp <= 15) {
				numArr[temp] = 1;
				++i;
			}
		}
	}
	
	_getch();

}

/*
* @brief	column1���� ��¡ �۾��� �� ������� column2�� ����
* @date		2022-05-29
* @return	void
* @params
*	column1 : Merge�� column
* 	column2 : Merge �۾����� ������ column
*/
void Merge(Node* column1, Node* column2) {
	Node* ptr1 = column1->next;	// ��¡ �۾��� ��� ������ 1
	Node* ptr2;	// ��¡ �۾��� ��� ������ 2
	Node* head = column2;		// ��¡ �۾��� ������ Į��
	Node* tail = head->next;

	/*
	* ptr1 != NULL, ptr2 != NULL�� ���� �����ͱ��� Į�� ���� �Ѿ�� �ʰ� �ϱ� ����
	* CountBit(ptr1) - CountBit(ptr2) >= -1 �� ��Ʈ 1�� ������ ������������ ���ĵ� Į������
	* �� �������� 1��Ʈ �� ���̰� -1 ~ -3 ���� �� �� �ִµ� �� �� ��Ʈ1 ���̰� 1������ ã�� ����.
	*/

	while (ptr1 != NULL) {
		ptr2 = ptr1->next;
		while (ptr2 != NULL) {
			if (CountBit(ptr1) - CountBit(ptr2) >= -1) {
				int count = 0;	// ���� �ٸ� ��Ʈ ��

				for (int i = 0; i < 4; i++)
					count += ptr1->bits[i] != ptr2->bits[i] ? 1 : 0;
			
				// ���� �ٸ� ��Ʈ�� 1�� �϶���
				if (count == 1) {
					ptr1->used = true;
					ptr2->used = true;
					Node* newNode = new Node[sizeof(Node)];
					newNode->next = NULL;
					newNode->used = false;
				
					// ��Ʈ ��¡
					for (int i = 0; i < 4; i++) 
						newNode->bits[i] = ptr1->bits[i] == ptr2->bits[i] ? ptr1->bits[i] : -1;

					// �׷�ѹ� �ʱ�ȭ
					memset(newNode->decimals, 0, sizeof(int) * 16);

					// �׷�ѹ� ����
					for (int i = 0; i < 16; i++) 
						if (ptr1->decimals[i] || ptr2->decimals[i]) newNode->decimals[i] = 1;
					
					head->next = newNode;
					head = head->next;
				}
			}
			ptr2 = ptr2->next;
		}
		ptr1 = ptr1->next;
	}

	DeleteOverlap(column2);
}

/*
* @brief	column���� �ߺ��� node ����
* @date		2022-05-29
* @return	void
* @params
*	column : �ߺ��� node�� ������ column
*/
void DeleteOverlap(Node* column) {
	if (column->next == NULL) return;

	Node* temp = column->next;
	Node* cur = column->next;
	Node* next = cur->next;


	while (cur != NULL) {
		while (next != NULL) {
			int count = 0;
			for (int i = 0; i < 4; i++) {
				count += cur->bits[i] == next->bits[i] ? 1 : 0;
			}
			
			// �ߺ���
			if (count == 4) {
				temp->next = next->next;
				delete[] next;
				next = temp->next;
			}
			else
			{
				temp = temp->next;
				if (temp != NULL) next = temp->next;
			}
			count = 0;
		}	
		temp = cur->next;
		cur = cur->next;
		if (cur != NULL) next = cur->next;
	}
}

/*
* @brief	PI����
* @date		2022-05-29
* @return	void
* @params
*	column1 : ������ Į�� �� Į��		_IN_
*	column2 : ������ Į��			_IN_
*	PI		: ������ PI ��ũ�帮��Ʈ	_OUT_
*/
void PI(Node* column1, Node* column2, Node* PI) {
	Node* ptr1	 = column1->next;
	Node* ptr2	 = column2->next;
	Node* pi	 = PI;
	Node* piTail = pi->next;

	while (ptr1 != NULL) {
		if (ptr1->used == false) {
			Node* newNode = new Node[sizeof(Node)];
			
			// ����
			newNode->next = pi->next;
			pi->next = newNode;
			pi = pi->next;

			for (int i = 0; i < 4; i++) {
				newNode->bits[i] = ptr1->bits[i];
			}

			for (int i = 0; i < 16; i++) {
				newNode->decimals[i] = ptr1->decimals[i];
			}

			newNode->used = false;
		}
		// ���� ����
		ptr1 = ptr1->next;
	}
	while (ptr2 != NULL) {
		if (ptr2->used == false) {
			Node* newNode = new Node[sizeof(Node)];

			// ����
			newNode->next = pi->next;
			pi->next = newNode;
			pi = pi->next;

			for (int i = 0; i < 4; i++) {
				newNode->bits[i] = ptr2->bits[i];
			}

			for (int i = 0; i < 16; i++) {
				newNode->decimals[i] = ptr2->decimals[i];
			}

			newNode->used = false;
		}
		// ���� ����
		ptr2 = ptr2->next;
	}
}

/*
* @brief	ȸ�ε����� EPI����
* @date		2022-05-29
* @return	void
* @params
*	PI : Prime Implicants	_IN_
*	graph : ȸ�ε�(������Ʈ)	_IN_
*	EPI : ������ EPI			_OUT_
*/
void EPI(Node* PI, int** graph, Node* EPI) {
	Node* ptr  = PI->next;
	Node* cur  = EPI;


	// �ʼ����� ã��
	int i, j, row;
	int arr[16][2];

	for (int i = 0; i < 16; i++) {
		memset(&arr[i], -1, sizeof(int) * 2);
	}
	for (i = 0; i < 16; i++) {
		int count = 0;
		row = 0;	// �ʼ������� �ִ� row

		for (j = 0; j < circuitRow; j++) {
			if (graph[j][i] == 1) {
				++count;
				row = j;
			}
		}
		
		// �����ٷ� 2�� �̻��̸� ���� ���� �Ѿ
		if (count != 1) continue;

		arr[i][0] = row;
		arr[i][1] = i;
		// �ʼ����� ����
		int temp = row;
		while (ptr != NULL && temp != 0) {
			ptr = ptr->next;
			--temp;
		}

		Node* newNode = new Node[sizeof(Node)];
		newNode->next = cur->next;
		newNode->used = false;
		cur->next = newNode;
		cur = cur->next;

		for (int j = 0; j < 4; j++) {
			cur->bits[j] = ptr->bits[j];
		}

		for (int j = 0; j < 16; j++) {
			cur->decimals[j] = ptr->decimals[j];
		}
		ptr = PI->next; // ptr �ʱ�ȭ
	} 

	// �࿭ ����
	for (int i = 0; i < 16; i++) {
		int r = arr[i][0];
		int c = arr[i][1];

		if (r != -1 && c != -1) {
			for (int j = 0; j < 16; j++) {
				if (graph[r][j] == 1) {
					for (int k = 0; k < circuitRow; k++) {
						graph[k][j] = 0;
					}
				}
			}
		}
	}

	for (int i = 0; i < 16; i++) {
		int r = arr[i][0];
		int c = arr[i][1];
		if (r != -1 && c != -1) graph[r][c] = 1;

	}
	// ���� epi ����
	while (ptr != NULL) {
		for (int i = 0; i < circuitRow; i++) {
			bool a = true;
			for (int j = 0; j < 16; j++) {
				if (ptr->decimals[j] != graph[i][j]) a = false;
			}

			if (a) {
				Node* newNode = new Node[sizeof(Node)];
				newNode->next = cur->next;
				newNode->used = false;
				cur->next = newNode;
				cur = cur->next;

				for (int j = 0; j < 4; j++) {
					cur->bits[j] = ptr->bits[j];
				}

				for (int j = 0; j < 16; j++) {
					cur->decimals[j] = ptr->decimals[j];
				}
			}

			ptr = ptr->next;
		}
	}

	DeleteOverlap(EPI);
}

/*
* @brief	ȸ�ε� ����
* @date		2022-05-29
* @return	void
* @params
*	PI  : ȸ�ε� ������ ���Ǵ� PI list	_IN_
*	EPI : ������ EPI list				_OUT_
*/
int** CurcuitDiagram(Node* PI) {
	Node* ptr = PI->next;
	int row = 0;
	int** graph;

	// row ���
	while (ptr != NULL) {
		++row;
		ptr = ptr->next;
	}
	ptr = PI->next; // ptr �ʱ�ȭ
	
	// �׷��� ����
	graph = new int* [row];
	for (int i = 0; i < row; i++) {
		graph[i] = new int[16];
		for (int j = 0; j < 16; j++) {
			graph[i][j] = 0;
		}
	}


	circuitRow = row;
	row = 0;
	while (ptr != NULL) {
		for (int i = 0; i < 16; i++) {
			if (ptr->decimals[i] == 1) {
				graph[row][i] = 1;
			}
		}
		ptr = ptr->next;
		++row;
	}

	return graph;
}

/*
* @brief	column�� Node �޸� �Ҵ� ����
* @date		2022-05-29
* @return	void
* @params
*	column : �Ҵ� ������ column
*/
void Free(Node* column) {
	Node* cur = column;
	Node* next = cur->next;
	
	while (next != NULL) {
		cur->next = next->next;
		delete[] next;
		next = cur->next;
	}
}

/**
* @brief	���ڷ� ���� Į���� 1�� ������ ���� �׷���.
* @date		2022-05-27
* @return	����
* @param
*	column : �׷����� Į��
*/
void Grouping(Node* column) {
	Node* head	= column;		// Į�� ���� �������� ���
	Node* tail	= head->next;	// �������� ����� ���� ���
	int temp;

	for (int i = 0; i < 16; i++)
	{
		// �Է��׵鸸
		if (numArr[i] == 1)
		{
			Node* newdata = new Node[sizeof(Node)];
			memset(newdata->decimals, 0, sizeof(int) * 16);
			
			newdata->decimals[i] = 1;

			// ��Ʈ �Է�
			temp = i;
			for (int j = 3; j >= 0; j--)
			{
				newdata->bits[j] = temp % 2; //2������ �Է�
				temp /= 2;
			}

			// 1������ ���� ������ ä������
			// ���� ���� ���ڰ� next�� ���ں���
			while (tail != NULL && CountBit(newdata) >= CountBit(tail))
			{
				head = head->next;
				tail = head->next;
			}
			newdata->next = tail; //��忬��
			head->next = newdata;
			head = column;  //p,s�����ġ �ʱ�ȭ
			tail = head->next;
		}
	}
}

/**
* @brief	1�� ������ ����
* @date		2022-05-27
* @return	int
* @param
*	node : 1�� �� ����
*/
int CountBit(Node* node) {
	int temp = 0;
	for (int i = 0; i < 4; i++) if (node->bits[i] == 1) temp++;
	return temp;
}

void cls() { system("cls"); }

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
/*		�Լ� ���Ǻ� ��		*/