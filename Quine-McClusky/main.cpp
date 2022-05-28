#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <iomanip> // cout format

using namespace std;

typedef struct Node {
	int decimals[16];	// 결합된 노드의 10진수
	int bits[4];		// 2진수
	struct Node* next;	// 다음 노드
}Node;


int numArr[16] = { 0 };


/*		함수 선언부 시작		*/

void PrintColumn(Node* column);	// Print Column
void ChoiceNum();				// 주항 입력
void Grouping(Node* column);
void InitTable(Node* table);
void Merge(Node* column1, Node* column2);
int CountBit(Node* node);
void cls();

/*		함수 선언부 끝		*/	


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


/*		함수 구현부 시작		*/

/*
* @brief	인자로 받은 column을 출력
* @return	void
* @date		2022-05-27
* @params
*	column : 출력할 column
*/
void PrintColumn(Node* column) {
	Node* next = column->next;

	while (next != NULL) {
		// 예상 출력
		// [1 ,] 0 0 0 1 

		cout << "[";
		int last = 0;
		for(int i = 0; i < 16; i++) 
			if (next->decimals[i] == 1) 
				last = i;
			

		for (int i = 0; i < 16; i++) //group 출력
		{
			// left << setw(2) : 2칸 좌측 정렬 출력
			if (last == i) cout << left << setw(2) << i;
			else if (next->decimals[i] == 1) cout << left << setw(2) << i << ", ";
		}
		cout << "] ";
		for (int i = 0; i < 4; i++) //bit 출력
		{
			if (next->bits[i] != -1) cout << left << setw(2) << next->bits[i];
			else cout << "- ";
		}
		cout << endl;
		next = next->next;
	}
}

/*
* @breif	입력항을 선택하는 함수
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
* @breif	최초 테이블 설계
* @date		2022-05-27
* @return	void
* @params
*	table : 최초 설계할 테이블
*/
void InitTable(Node* table) {
	Node* curPtr = table;		// 가르키고 있는 노드
	Node* next = curPtr->next;	// 다음번째 노드

	int temp;

	// 노드 만들고 테이블에 연결
	for (int i = 0; i < 16; i++)
	{
		Node* newdata = new Node[sizeof(Node)];
		newdata->decimals[0] = i; //그룹넘버 입력

		for (int j = 1; j < 16; j++)
		{
			newdata->decimals[j] = -1;
		}
		temp = i;
		for (int j = 3; j >= 0; j--)
		{
			newdata->bits[j] = temp % 2; //2진수로 입력
			temp /= 2;
		}
		newdata->next = next;  //노드연결
		curPtr->next = newdata;

		curPtr = newdata;    //노드이동
	}
}

/*
* @brief	column1에서 머징 작업을 한 결과물을 column2에 저장
* @date		2022-05-29
* @return	void
* @params
*	column1 : Merge할 column
* 	column2 : Merge 작업물을 저장할 column
*/
void Merge(Node* column1, Node* column2) {
	Node* ptr1 = column1->next;	// 머징 작업할 노드 포인터 1
	Node* ptr2;	// 머징 작업할 노드 포인터 2
	Node* head = column2;		// 머징 작업물 삽입할 칼럼
	Node* tail = head->next;

	/*
	* ptr1 != NULL, ptr2 != NULL의 뜻은 포인터까지 칼럼 끝을 넘어가지 않게 하기 위함
	* CountBit(ptr1) - CountBit(ptr2) >= -1 는 비트 1의 개수가 오름차순으로 정렬된 칼럼에서
	* 두 포인터의 1비트 수 차이가 -1 ~ -3 까지 날 수 있는데 이 때 비트1 차이가 1개만을 찾기 위함.
	*/

	while (ptr1 != NULL) {
		ptr2 = ptr1->next;
		while (ptr2 != NULL) {
			if (CountBit(ptr1) - CountBit(ptr2) >= -1) {
				int count = 0;	// 서로 다른 비트 수

				for (int i = 0; i < 4; i++)
					count += ptr1->bits[i] != ptr2->bits[i] ? 1 : 0;
			
				// 서로 다른 비트가 1개 일때만
				if (count == 1) {
					Node* newNode = new Node[sizeof(Node)];
					newNode->next = NULL;
				
					// 비트 머징
					for (int i = 0; i < 4; i++) 
						newNode->bits[i] = ptr1->bits[i] == ptr2->bits[i] ? ptr1->bits[i] : -1;

					// 그룹넘버 초기화
					memset(newNode->decimals, 0, sizeof(int) * 16);

					// 그룹넘버 적기
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
* @brief	인자로 받은 칼럼을 1의 개수에 따라 그룹핑.
* @date		2022-05-27
* @return	없음
* @param
*	column : 그룹핑할 칼럼
*/
void Grouping(Node* column) {
	Node* head	= column;		// 칼럼 현재 참조중인 노드
	Node* tail	= head->next;	// 참조중인 노드의 다음 노드
	int temp;

	for (int i = 0; i < 16; i++)
	{
		// 입력항들만
		if (numArr[i] == 1)
		{
			Node* newdata = new Node[sizeof(Node)];
			memset(newdata->decimals, 0, sizeof(int) * 16);
			
			newdata->decimals[i] = 1;

			// 비트 입력
			temp = i;
			for (int j = 3; j >= 0; j--)
			{
				newdata->bits[j] = temp % 2; //2진수로 입력
				temp /= 2;
			}

			// 1개수가 작은 순으로 채워넣음
			// 현재 만든 숫자가 next의 숫자보다
			while (tail != NULL && CountBit(newdata) >= CountBit(tail))
			{
				head = head->next;
				tail = head->next;
			}
			newdata->next = tail; //노드연결
			head->next = newdata;
			head = column;  //p,s노드위치 초기화
			tail = head->next;
		}
	}
}

/**
* @brief	1의 개수를 리턴
* @date		2022-05-27
* @return	int
* @param
*	node : 1을 셀 숫자
*/
int CountBit(Node* node) {
	int temp = 0;

	for (int i = 0; i < 4; i++) {
		if (node->bits[i] == 1) temp++;
	}

	return temp;
}

void cls() { system("cls"); }

/*		함수 구현부 끝		*/