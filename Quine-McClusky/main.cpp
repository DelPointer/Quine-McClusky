#include <iostream>
#include <Windows.h>	// system()
#include <conio.h>		// _getch()
#include <iomanip>		// cout format
#include <algorithm>	// sort(), copy()
#include <random>		
using namespace std;

typedef struct Node {
	int decimals[16];	// 결합된 노드의 10진수
	int bits[4];		// 2진수
	bool used;			// 병합에 사용이 됐는가?
	struct Node* next;	// 다음 노드
}Node;

int numArr[16] = { 0 };	// 입력항
int circuitRow = 0;		// 회로도 길이

/*		함수 선언부 시작		*/
void PrintColumn(Node* column, bool cdt);	// 칼럼 출력
void PrintCircuit(int** graph, Node* PI);	// 회로도 출력
void PrintFx(Node* EPI);					// 최종식 출력
void ChoiceNum();							// 입력항
void Grouping(Node* column);				// 그룹피잉
void Merge(Node* column1, Node* column2);	// 합병한거 저장
void DeleteOverlap(Node* column);			// 합병중에 중복있는거 제거
void PI(Node* column1, Node* column2, Node* PI); // PI추출
void EPI(Node* PI, int** graph, Node* EPI);	// EPI추출
int** CurcuitDiagram(Node* PI);				// 회로식
void Free(Node* column);					// 칼럼에 있는 데이터 없애줌
int  CountBit(Node* node);					// 비트 카운팅
void cls();									// 스크린 클리어
void gotoxy(int x, int y);					// gotoxy()
/*		함수 선언부 끝		*/	

void main() {
	
	Node* table	  = new Node[sizeof(Node)];	// 최초 테이블
	Node* column1 = new Node[sizeof(Node)];	// 1차 분류
	Node* column2 = new Node[sizeof(Node)]; // 2차 분류
	Node* column3 = new Node[sizeof(Node)]; // 3차 분류
	Node* column4 = new Node[sizeof(Node)]; // 4차 분류	최대
	Node* pi	  = new Node[sizeof(Node)]; // PI 
	Node* epi	  = new Node[sizeof(Node)];	// EPI

	int level   = 0;	// 병합 결과가 없는 곳
	int page	= 0;	// 현재 단계
	int key		= 0;
	int result  = 0;
	int** graph_1 = NULL;		// PI 회로도
	int** graph_2 = NULL;		// EPI 회로도

	table->next   = NULL;
	column1->next = NULL;
	column2->next = NULL;
	column3->next = NULL;
	column4->next = NULL;
	pi->next	  = NULL;
	epi->next	  = NULL;
	
	ChoiceNum();
	Grouping(table);

	Merge(table, column1);		// 1차 병합 결과
	Merge(column1, column2);	// 2차 병합 결과
	Merge(column2, column3);	// 3차 병합 결과
	Merge(column3, column4);	// 4차 병합 결과
	
	while (key != 27) {
		cls();
		if (page == 0) { // 최초 테이블
			PrintColumn(table, true);
			cout << "최초 테이블";
		}
		else if (page == 1) {	// 1번 칼럼
			if (column1->next != NULL) {
				PrintColumn(column1, false);
				cout << "1차 병합";
			}
			
			else {
				cout << "병합된 Column이 없습니다. 프로그램을 종료합니다.";
				return;
			}
		}
		else if (page == 2) {	// 2번 칼럼
			if (column2->next != NULL) {
				PrintColumn(column2, false);
				cout << "2차 병합";
				result = 1;
			}
			else page = 5;
		}
		else if (page == 3) {	// 3번 칼럼	
			if (column3->next != NULL) {
				PrintColumn(column3, false);
				cout << "3차 병합";
				result = 2;
			}
			else {
				page = 5;
				result = 1;
			}
		}
		else if (page == 4) {	// 4번 칼럼
			if (column4->next != NULL) {
				PrintColumn(column4, false);
				cout << "4차 병합";
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
			cout << "PI 추출";
		}
		else if (page == 6) {	// graph_1
			PrintCircuit(graph_1, pi);
			cout << "그래프_1";
		}
		else if (page == 7) {	// graph_2
			if (epi->next == NULL) EPI(pi, graph_2, epi);
			PrintCircuit(graph_2, pi);
			cout << "그래프_2";
		}
		else if (page == 8) {	// 결과식
			
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

/*		함수 정의부 시작		*/
/*
* @brief	인자로 받은 column을 출력
* @return	void
* @date		
*	2022-05-27
*	2022-05-29
* @params
*	column : 출력할 column						_IN_
*	cdt	   : 출력할 때 비트 1의 개수를 표현 여부	_IN_
*/
void PrintColumn(Node* column, bool cdt) {
	Node* next = column->next;
	int bit = -1;
	int curbit = -1;
	while (next != NULL) {
		// 예상 출력
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
			
		for (int i = 0; i < 16; i++) //group 출력
		{
			// left << setw(2) : 2칸 좌측 정렬 출력
			if (last == i) cout << left << setw(2) << i;
			else if (next->decimals[i] == 1) cout << left << setw(2) << i << ",";
		}
		cout << "] ";
		for (int i = 0; i < 4; i++) //bit 출력
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
* @brief	회로도 출력
* @date		2022-05-29
* @return	void
* @params
*	graph : 출력할 회로도
*	PI : 좌측에 출력할 PI
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
* @brief	EPI를 이용해 논리식 도출
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
* @breif	주항 입력
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
	cout << "입력항 결정(0: 자동, 1: 수동) "; cin >> mode;
	cout << "입력항 개수 설정(최대 16개) "; cin >> count;

	if (mode == 0) { // 자동
		// 시드값을 얻기 위한 random_device 생성.
		std::random_device rd;

		// random_device 를 통해 난수 생성 엔진을 초기화 한다.
		std::mt19937 gen(rd());

		// 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
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
					ptr1->used = true;
					ptr2->used = true;
					Node* newNode = new Node[sizeof(Node)];
					newNode->next = NULL;
					newNode->used = false;
				
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

	DeleteOverlap(column2);
}

/*
* @brief	column에서 중복된 node 제거
* @date		2022-05-29
* @return	void
* @params
*	column : 중복된 node를 제거할 column
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
			
			// 중복됨
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
* @brief	PI추출
* @date		2022-05-29
* @return	void
* @params
*	column1 : 마지막 칼럼 전 칼럼		_IN_
*	column2 : 마지막 칼럼			_IN_
*	PI		: 추출한 PI 링크드리스트	_OUT_
*/
void PI(Node* column1, Node* column2, Node* PI) {
	Node* ptr1	 = column1->next;
	Node* ptr2	 = column2->next;
	Node* pi	 = PI;
	Node* piTail = pi->next;

	while (ptr1 != NULL) {
		if (ptr1->used == false) {
			Node* newNode = new Node[sizeof(Node)];
			
			// 삽입
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
		// 다음 노드로
		ptr1 = ptr1->next;
	}
	while (ptr2 != NULL) {
		if (ptr2->used == false) {
			Node* newNode = new Node[sizeof(Node)];

			// 삽입
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
		// 다음 노드로
		ptr2 = ptr2->next;
	}
}

/*
* @brief	회로도에서 EPI추출
* @date		2022-05-29
* @return	void
* @params
*	PI : Prime Implicants	_IN_
*	graph : 회로도(주항차트)	_IN_
*	EPI : 추출한 EPI			_OUT_
*/
void EPI(Node* PI, int** graph, Node* EPI) {
	Node* ptr  = PI->next;
	Node* cur  = EPI;


	// 필수주항 찾기
	int i, j, row;
	int arr[16][2];

	for (int i = 0; i < 16; i++) {
		memset(&arr[i], -1, sizeof(int) * 2);
	}
	for (i = 0; i < 16; i++) {
		int count = 0;
		row = 0;	// 필수주항이 있는 row

		for (j = 0; j < circuitRow; j++) {
			if (graph[j][i] == 1) {
				++count;
				row = j;
			}
		}
		
		// 세로줄로 2개 이상이면 다음 열로 넘어감
		if (count != 1) continue;

		arr[i][0] = row;
		arr[i][1] = i;
		// 필수주항 넣음
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
		ptr = PI->next; // ptr 초기화
	} 

	// 행열 삭제
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
	// 숨은 epi 추출
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
* @brief	회로도 구현
* @date		2022-05-29
* @return	void
* @params
*	PI  : 회로도 구현에 사용되는 PI list	_IN_
*	EPI : 추출한 EPI list				_OUT_
*/
int** CurcuitDiagram(Node* PI) {
	Node* ptr = PI->next;
	int row = 0;
	int** graph;

	// row 계산
	while (ptr != NULL) {
		++row;
		ptr = ptr->next;
	}
	ptr = PI->next; // ptr 초기화
	
	// 그래프 생성
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
* @brief	column의 Node 메모리 할당 해제
* @date		2022-05-29
* @return	void
* @params
*	column : 할당 해제할 column
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
	for (int i = 0; i < 4; i++) if (node->bits[i] == 1) temp++;
	return temp;
}

void cls() { system("cls"); }

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
/*		함수 정의부 끝		*/