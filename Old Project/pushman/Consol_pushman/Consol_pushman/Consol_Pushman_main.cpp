#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <windows.h>
#include <conio.h>
#include "Consol_pushman_stageData.h" //스테이지데이터 구조를 가진 클래스

using namespace std;

/*
1번 스테이지형태
########
# ** p #
# 00   #
#      #
########
*/

/*
1. 초기화/스테이지선택
2. 키입력
3. 계산
4. 출력
*/

//게임오브젝트
enum object
	{
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_GOAL,
		OBJ_BLOCK,
		OBJ_BLOCK_ON_GOAL,
		OBJ_MAN,
		OBJ_MAN_ON_GOAL,

		OBJ_UNKNOWN
	};

int findPlayerPos(const object* state, const int width, const int height);														//플레이어 위치검색
void initialize(object* state, const string stageData, const int width, const int height);	//데이터 초기설정
void draw(const object* state, const int width, const int height);							//화면그리기
void update(char inputKey, object* state, const int width, const int height);				//데이터 업데이트
bool winCheck(const object* state, const int width, const int height);						//승리조건 체크
void fReadStageData(list<stageData>& stageList);

#define inputErrMsg cout<<"잘못된 입력입니다."<<endl

int main()
{
	int iflag = 0;						//스테이지선택 및 메인루프 플래그
	int stageCnt = 0;					//스테이지의 개수
	list<stageData> stageList;			//스테이지리스트 생성
	char bufInputKey = 0;				//버퍼-키입력
	
	int bufStageNum = 0;				//버퍼-스테이지번호
	string bufStageObj = "";			//버퍼-스테이지오브젝트정보
	int bufStageWidth = 0;				//버퍼-스테이지가로길이
	int bufStageHeight = 0;				//버퍼-스테이지세로길이
	stageData bufStageData;		//버퍼-스테이지데이터 리스트
		
	//스테이지 읽어오기 - 파일데이터 읽어오기******************
	fReadStageData(stageList);

	//입력된 스테이지 개수
	stageCnt = stageList.size();

	//초기 스테이지 입력
	while(1)
	{
		//초기입력
		cout << "플레이할 스테이지 번호를 입력하세요 (0:종료, 1~" << stageCnt << ":스테이지번호)" << endl;
		cin >> iflag;
		//cout << iflag << endl; - 디버그용

		//화면초기화
		system("cls");			

		//입력체크
		if(iflag > stageCnt) inputErrMsg;
		else if(iflag < 0) inputErrMsg;
		else break; //stageCnt보다 크거나 0보다 작은 경우가 아니라면 정상진행으로 판단
	}

	//cout << "iflag : " << iflag << endl; - 디버그용

	//선택된 스테이지로 스테이지 정보 초기화
	list<stageData>::iterator findStage = stageList.begin();

	//선택한 스테이지 검색
	while(findStage != stageList.end())
	{
		if(iflag == (*findStage).get_stageNum())
		{
			bufStageObj = (*findStage).get_stageObj();		//스테이지 오브젝트
			bufStageWidth = (*findStage).get_stageWidth();	//스테이지 가로길이
			bufStageHeight = (*findStage).get_stageHeight();//스테이지 세로길이
		}
		findStage++;
	}

	cout << "figure out" << endl;

	//게임데이터 초기화
	object* state = new object[bufStageWidth * bufStageHeight];		//오브젝트배열
	initialize(state, bufStageObj, bufStageWidth, bufStageHeight);	//초기화 함수

	//메인루프
	while(iflag)
	{
		//화면출력
		draw(state, bufStageWidth, bufStageHeight);

		//승리조건체크
		if(winCheck(state, bufStageWidth, bufStageHeight))
		{
			cout << "Congratulations!! 완성하셨습니다!!" << endl
				 << "아무키나 누르면 종료됩니다." << endl;
			bufInputKey = _getch();
			break;
		}

		//입력부
		cout << "입력키 안내 : W-위쪽, A-왼쪽, S-아래쪽, D-오른쪽, X-종료" << endl;
		bufInputKey = _getch();
		//cout << bufInputKey << endl;

		//게임데이터 업데이트
		update(bufInputKey, state, bufStageWidth, bufStageHeight);
	}
	delete state;
}


//데이터 초기설정
void initialize(object* state, const string stageData, const int width, const int height)	
{
	//const string sRead = stageData;	//읽기용
	int x = 0;						//배열x축
	int y = 0;						//배열y축
	
	for(int i = 0;i < stageData.length(); i++)
	{
		object bufObj; //state입력용

		switch(stageData.at(i))
		{
		case ' ': bufObj = OBJ_SPACE; break;
		case '#': bufObj = OBJ_WALL; break;
		case '*': bufObj = OBJ_GOAL; break;
		case 'o': bufObj = OBJ_BLOCK; break;
		case 'O': bufObj = OBJ_BLOCK_ON_GOAL; break;
		case 'p': bufObj = OBJ_MAN; break;
		case 'P': bufObj = OBJ_MAN_ON_GOAL; break;
		case '\n': x = 0; y++; bufObj = OBJ_UNKNOWN; break;
			//개행문자를 만나면 x를 0으로 돌리고 y를 하나 증가하고 state에는 아무것도 추가하지않는다.
		default : bufObj = OBJ_UNKNOWN;
		}

		if(bufObj != OBJ_UNKNOWN)
		{
			//알수없는 데이터가 아닐경우 state에 추가
			state[y * width + x] = bufObj;
			x++;
		}
	}
}

//화면그리기
void draw(const object* state, const int width, const int height)
{
	system("cls");
	object readDraw; //읽기용

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			//x축 한줄
			readDraw = state[y * width + x];

			switch(readDraw)
			{
			case OBJ_SPACE:			cout << ' '; break;
			case OBJ_WALL:			cout << '#'; break;
			case OBJ_GOAL:			cout << '*'; break;
			case OBJ_BLOCK:			cout << 'o'; break;
			case OBJ_BLOCK_ON_GOAL: cout << 'O'; break;
			case OBJ_MAN:			cout << 'p'; break;
			case OBJ_MAN_ON_GOAL:	cout << 'P'; break;
			}
		}
		//행바꿈
		cout << endl;
	}
}

//데이터 업데이트
void update(char inputKey, object* state, const int width, const int height)
{
	int playerPos = 0;
	int subjectPos = 0;

	//플레이어 위치파악
	playerPos = findPlayerPos(state, width, height);
	//cout << findPlayerPos(state, width, height) << endl;

	//입력된 키를 바탕으로
	//각 오브젝트 위치정보 수정
	//배열첨자값
	//세로입력은 +-width
	//가로입력은 +-1
	switch(inputKey)
	{
	case 'w': subjectPos = playerPos - width; break;
	case 'W': subjectPos = playerPos - width; break;
	case 's': subjectPos = playerPos + width; break;
	case 'S': subjectPos = playerPos + width; break;
	case 'a': subjectPos = playerPos - 1; break;
	case 'A': subjectPos = playerPos - 1; break;
	case 'd': subjectPos = playerPos + 1; break;
	case 'D': subjectPos = playerPos + 1; break;
	case 'x': subjectPos = -1; exit(1);
	case 'X': subjectPos = -1; exit(1);
	default : subjectPos = -1; cout << "잘못된 키입력입니다." << endl;
	}

	if((subjectPos < 0)||(subjectPos > (width * height)))
	{
		//목표위치가 배열범위를 벗어날경우 아무것도 하지않음
	}
	else
	{
		//배열범위를 벗어나지 않을경우, 위치조정
		if(state[playerPos] == OBJ_MAN)
		{//플레이어 위치가 골 위가 아닌경우
			if(state[subjectPos] == OBJ_SPACE)
			{
				//목표위치가 빈칸인 경우 그대로 이동
				state[subjectPos] = OBJ_MAN;
				state[playerPos] = OBJ_SPACE;
			}
			else if(state[subjectPos] == OBJ_GOAL)
			{
				//목표위치가 골인 경우
				state[subjectPos] = OBJ_MAN_ON_GOAL;
				state[playerPos] = OBJ_SPACE;
			}
			else if(state[subjectPos] == OBJ_BLOCK)
			{
				//목표위치가 블록인 경우
				//진행방향 2칸앞을 검사
				if(state[subjectPos + (subjectPos - playerPos)] == OBJ_SPACE)
				{
					//빈칸이면 블록과 플레이어를 전진
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK;
					state[subjectPos] = OBJ_MAN;
					state[playerPos] = OBJ_SPACE;
				}
				else if(state[subjectPos + (subjectPos - playerPos)] == OBJ_GOAL)
				{
					//골이면 블록을 골위에 있는 상태로 바꿔서 이동
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK_ON_GOAL;
					state[subjectPos] = OBJ_MAN;
					state[playerPos] = OBJ_SPACE;
				}
			}
			else if(state[subjectPos] == OBJ_BLOCK_ON_GOAL)
			{
				//목표위치가 골위의 블록인 경우
				//진행방향 2칸앞을 검사
				if(state[subjectPos + (subjectPos - playerPos)] == OBJ_SPACE)
				{
					//빈칸이면 블록과 플레이어를 전진
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK;
					state[subjectPos] = OBJ_MAN_ON_GOAL;
					state[playerPos] = OBJ_SPACE;
				}
				else if(state[subjectPos + (subjectPos - playerPos)] == OBJ_GOAL)
				{
					//골이면 블록을 골위에 있는 상태로 바꿔서 이동
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK_ON_GOAL;
					state[subjectPos] = OBJ_MAN_ON_GOAL;
					state[playerPos] = OBJ_SPACE;
				}
			}
		}
		else if(state[playerPos] == OBJ_MAN_ON_GOAL)
		{//플레이어 위치가 골 위인 경우
			if(state[subjectPos] == OBJ_SPACE)
			{
				//목표위치가 빈칸인 경우 그대로 이동
				state[subjectPos] = OBJ_MAN;
				state[playerPos] = OBJ_GOAL;
			}
			else if(state[subjectPos] == OBJ_GOAL)
			{
				//목표위치가 골인 경우
				state[subjectPos] = OBJ_MAN_ON_GOAL;
				state[playerPos] = OBJ_GOAL;
			}
			else if(state[subjectPos] == OBJ_BLOCK)
			{
				//목표위치가 블록인 경우
				//진행방향 2칸앞을 검사
				if(state[subjectPos + (subjectPos - playerPos)] == OBJ_SPACE)
				{
					//빈칸이면 블록과 플레이어를 전진
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK;
					state[subjectPos] = OBJ_MAN;
					state[playerPos] = OBJ_GOAL;
				}
				else if(state[subjectPos + (subjectPos - playerPos)] == OBJ_GOAL)
				{
					//골이면 블록을 골위에 있는 상태로 바꿔서 이동
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK_ON_GOAL;
					state[subjectPos] = OBJ_MAN;
					state[playerPos] = OBJ_GOAL;
				}
			}
			else if(state[subjectPos] == OBJ_BLOCK_ON_GOAL)
			{
				//목표위치가 골위의 블록인 경우
				//진행방향 2칸앞을 검사
				if(state[subjectPos + (subjectPos - playerPos)] == OBJ_SPACE)
				{
					//빈칸이면 블록과 플레이어를 전진
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK;
					state[subjectPos] = OBJ_MAN_ON_GOAL;
					state[playerPos] = OBJ_GOAL;
				}
				else if(state[subjectPos + (subjectPos - playerPos)] == OBJ_GOAL)
				{
					//골이면 블록을 골위에 있는 상태로 바꿔서 이동
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK_ON_GOAL;
					state[subjectPos] = OBJ_MAN_ON_GOAL;
					state[playerPos] = OBJ_GOAL;
				}
			}
		}
	}
}

//승리조건 체크
bool winCheck(const object* state, const int width, const int height)
{
	for(int i = 0; i < (width * height); i++)
	{
		//GOAL에 올라가있지않은 블록이 있다면 아직 클리어되지 않은 것으로 간주.
		if(state[i] == OBJ_BLOCK)
		{
			return false;
		}
	}
	return true;
}

//플레이어 위치검색
int findPlayerPos(const object* state, const int width, const int height)
{
	int i = 0; //첨자용

	for(i; i < (width * height); i++)
	{
		//플레이어 위치 첨자를 반환
		if(state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;
		}
	}

	return i;
}

//파일읽고 스테이지데이터를 리스트에 입력
void fReadStageData(list<stageData>& stageList)
{
	string dataStr = "";
	string noteStr = "";
	string objStr = "";
	int lineNum = 0;
	int pathFlag = 1; // 1:스테이지넘버, 2:스테이지가로, 3:스테이지세로, 4:스테이지오브젝트
	int objFlag = 0;

	int bufStageNum = 0;				//버퍼-스테이지번호
	string bufStageObj = "";			//버퍼-스테이지오브젝트정보
	int bufStageWidth = 0;				//버퍼-스테이지가로길이
	int bufStageHeight = 0;				//버퍼-스테이지세로길이
	stageData bufStageData;				//버퍼-스테이지데이터 리스트

//	bufStageData = new stageData;

	ifstream readFile;
	readFile.open("Resource/stageData.txt", ios::in);

	//한줄씩 읽어들이기
	while(getline(readFile, dataStr))
	{
		//주석인지 확인하기 위해 첫 글자 검사
		noteStr = dataStr.at(0);

		//첫 글자가 - 이면 주석이므로 데이터처리를 하지않음
		if(noteStr.compare("-") != 0)
		{
			if(pathFlag == 1) //Flag = 1 : StageNum
			{
				//정수로 변환해서 입력
				bufStageNum = atoi(dataStr.c_str()); /* c++11 환경에선 stoi()함수이용 */

				//Flag를 2로 올려서 가로길이를 받을 수 있도록 조정
				pathFlag = 2;
			}
			else if(pathFlag == 2) //Flag = 2 : StageWidth
			{
				//데이터무결성 체크
				if(bufStageNum < 1)
				{
					cout << bufStageNum 
						 << "스테이지의 번호 데이터오류(" << bufStageNum << ")로 가로길이를 읽을 수 없습니다." << endl 
						 << "프로그램을 종료합니다." << endl;
					Sleep(2000);
					exit(1);
				}

				//정수로 변환해서 입력
				bufStageWidth = atoi(dataStr.c_str()); /* c++11 환경에선 stoi()함수이용 */

				//Flag를 3으로 올려서 세로길이를 받을 수 있도록 조정
				pathFlag = 3;
			}
			else if(pathFlag == 3) //Flag = 3 : StageHeight
			{
				//데이터무결성 체크
				if(bufStageWidth < 1)
				{
					cout << bufStageNum 
						 << "스테이지의 가로길이 데이터오류(" << bufStageWidth << ")로 세로길이를 읽을 수 없습니다." << endl 
						 << "프로그램을 종료합니다." << endl;
					Sleep(2000);
					exit(1);
				}

				//정수로 변환해서 입력
				bufStageHeight = atoi(dataStr.c_str()); /* c++11 환경에선 stoi()함수이용 */

				//Flag를 4로 올려서 오브젝트를 받을 수 있도록 조정
				pathFlag = 4;
			}
			else if(pathFlag == 4) //Flag = 4 : StageObj
			{
				//데이터무결성 체크
				if(bufStageHeight < 1)
				{
					cout << bufStageNum 
						 << "스테이지의 세로길이 데이터오류(" << bufStageHeight << ")로 오브젝트를 읽을 수 없습니다." << endl 
						 << "프로그램을 종료합니다." << endl;
					Sleep(2000);
					exit(1);
				}

				//스테이지 세로길이bufStageHeight만큼 반복
				objFlag++;

				objStr.append(dataStr);
				objStr.append("\n");

				//bufStageHeight만큼 반복이 끝나면 리스트에 넣고
				//변수와 Flag를 초기화하여 새로운 스테이지를 받을 수 있도록 조정
				if(objFlag == bufStageHeight)
				{
					//정리된 objStr를 char형으로 변환
					bufStageObj = objStr;
					//cout << bufStageObj << endl; - 디버그용

					//리스트에 넣기
					bufStageData.set_stageData(bufStageNum,bufStageObj,bufStageWidth,bufStageHeight);
					stageList.push_back(bufStageData);

					//초기화
					objFlag = 0;
					pathFlag = 1;
					bufStageNum = 0;
					bufStageWidth = 0;
					bufStageHeight = 0;
					objStr = "";
				}
			}//path end
		}//compare end
	}//while end
}