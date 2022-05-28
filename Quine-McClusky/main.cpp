#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <iomanip> // cout format

using namespace std;

typedef struct Node {
	int decimals[16];	// ���յ� ����� 10����
	int bits[4];		// 2����
	struct Node* next;	// ���� ���
}Node;


int numArr[16] = { 0 };


/*		�Լ� ����� ����		*/

void PrintColumn(Node* column);	// Print Column
void ChoiceNum();				// ���� �Է�
void Grouping(Node* column);
void InitTable(Node* table);
void Merge(Node* column1, Node* column2);
int CountBit(Node* node);
void cls();

/*		�Լ� ����� ��		*/	


void main() {
	
	Node* table = new Node[sizeof(Node)];
	Node* column1 = new Node[sizeof(Node)];
	Node* column2 = new Node[sizeof(Node)];

	table->next = NULL;
	column1->next = NULL;
	column2->next = NULL;
	ChoiceNum();
	//InitTable(table);
	Grouping(table);
	PrintColumn(table);
	Merge(table, column1);

	cout << "\n\n\n";
	PrintColumn(column1);

	Merge(column1, column2);

	cout << "\n\n\n";
	PrintColumn(column2);
}


/*		�Լ� ������ ����		*/

/*
* @brief	���ڷ� ���� column�� ���
* @return	void
* @date		2022-05-27
* @params
*	column : ����� column
*/
void PrintColumn(Node* column) {
	Node* next = column->next;

	while (next != NULL) {
		// ���� ���
		// [1 ,] 0 0 0 1 

		cout << "[";
		int last = 0;
		for(int i = 0; i < 16; i++) 
			if (next->decimals[i] == 1) 
				last = i;
			

		for (int i = 0; i < 16; i++) //group ���
		{
			// left << setw(2) : 2ĭ ���� ���� ���
			if (last == i) cout << left << setw(2) << i;
			else if (next->decimals[i] == 1) cout << left << setw(2) << i << ", ";
		}
		cout << "] ";
		for (int i = 0; i < 4; i++) //bit ���
		{
			if (next->bits[i] != -1) cout << left << setw(2) << next->bits[i];
			else cout << "- ";
		}
		cout << endl;
		next = next->next;
	}
}

/*
* @breif	�Է����� �����ϴ� �Լ�
* @date		2022-05-27
* @return	void
* @param	void
*/
void ChoiceNum() {
	int temp;
	for (int i = 0; i < 16;) {
		cin >> temp;
		if (temp == 0 || temp == 1) {
			numArr[i] = temp;
			i++;
		}
		else i--;
	}
}

/*
* @breif	���� ���̺� ����
* @date		2022-05-27
* @return	void
* @params
*	table : ���� ������ ���̺�
*/
void InitTable(Node* table) {
	Node* curPtr = table;		// ����Ű�� �ִ� ���
	Node* next = curPtr->next;	// ������° ���

	int temp;

	// ��� ����� ���̺� ����
	for (int i = 0; i < 16; i++)
	{
		Node* newdata = new Node[sizeof(Node)];
		newdata->decimals[0] = i; //�׷�ѹ� �Է�

		for (int j = 1; j < 16; j++)
		{
			newdata->decimals[j] = -1;
		}
		temp = i;
		for (int j = 3; j >= 0; j--)
		{
			newdata->bits[j] = temp % 2; //2������ �Է�
			temp /= 2;
		}
		newdata->next = next;  //��忬��
		curPtr->next = newdata;

		curPtr = newdata;    //����̵�
	}
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
					Node* newNode = new Node[sizeof(Node)];
					newNode->next = NULL;
				
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

	for (int i = 0; i < 4; i++) {
		if (node->bits[i] == 1) temp++;
	}

	return temp;
}

void cls() { system("cls"); }

/*		�Լ� ������ ��		*/