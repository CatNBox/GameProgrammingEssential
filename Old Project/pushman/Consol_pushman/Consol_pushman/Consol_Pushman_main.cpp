#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <windows.h>
#include <conio.h>
#include "Consol_pushman_stageData.h" //�������������� ������ ���� Ŭ����

using namespace std;

/*
1�� ������������
########
# ** p #
# 00   #
#      #
########
*/

/*
1. �ʱ�ȭ/������������
2. Ű�Է�
3. ���
4. ���
*/

//���ӿ�����Ʈ
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

int findPlayerPos(const object* state, const int width, const int height);														//�÷��̾� ��ġ�˻�
void initialize(object* state, const string stageData, const int width, const int height);	//������ �ʱ⼳��
void draw(const object* state, const int width, const int height);							//ȭ��׸���
void update(char inputKey, object* state, const int width, const int height);				//������ ������Ʈ
bool winCheck(const object* state, const int width, const int height);						//�¸����� üũ
void fReadStageData(list<stageData>& stageList);

#define inputErrMsg cout<<"�߸��� �Է��Դϴ�."<<endl

int main()
{
	int iflag = 0;						//������������ �� ���η��� �÷���
	int stageCnt = 0;					//���������� ����
	list<stageData> stageList;			//������������Ʈ ����
	char bufInputKey = 0;				//����-Ű�Է�
	
	int bufStageNum = 0;				//����-����������ȣ
	string bufStageObj = "";			//����-��������������Ʈ����
	int bufStageWidth = 0;				//����-�����������α���
	int bufStageHeight = 0;				//����-�����������α���
	stageData bufStageData;		//����-�������������� ����Ʈ
		
	//�������� �о���� - ���ϵ����� �о����******************
	fReadStageData(stageList);

	//�Էµ� �������� ����
	stageCnt = stageList.size();

	//�ʱ� �������� �Է�
	while(1)
	{
		//�ʱ��Է�
		cout << "�÷����� �������� ��ȣ�� �Է��ϼ��� (0:����, 1~" << stageCnt << ":����������ȣ)" << endl;
		cin >> iflag;
		//cout << iflag << endl; - ����׿�

		//ȭ���ʱ�ȭ
		system("cls");			

		//�Է�üũ
		if(iflag > stageCnt) inputErrMsg;
		else if(iflag < 0) inputErrMsg;
		else break; //stageCnt���� ũ�ų� 0���� ���� ��찡 �ƴ϶�� ������������ �Ǵ�
	}

	//cout << "iflag : " << iflag << endl; - ����׿�

	//���õ� ���������� �������� ���� �ʱ�ȭ
	list<stageData>::iterator findStage = stageList.begin();

	//������ �������� �˻�
	while(findStage != stageList.end())
	{
		if(iflag == (*findStage).get_stageNum())
		{
			bufStageObj = (*findStage).get_stageObj();		//�������� ������Ʈ
			bufStageWidth = (*findStage).get_stageWidth();	//�������� ���α���
			bufStageHeight = (*findStage).get_stageHeight();//�������� ���α���
		}
		findStage++;
	}

	cout << "figure out" << endl;

	//���ӵ����� �ʱ�ȭ
	object* state = new object[bufStageWidth * bufStageHeight];		//������Ʈ�迭
	initialize(state, bufStageObj, bufStageWidth, bufStageHeight);	//�ʱ�ȭ �Լ�

	//���η���
	while(iflag)
	{
		//ȭ�����
		draw(state, bufStageWidth, bufStageHeight);

		//�¸�����üũ
		if(winCheck(state, bufStageWidth, bufStageHeight))
		{
			cout << "Congratulations!! �ϼ��ϼ̽��ϴ�!!" << endl
				 << "�ƹ�Ű�� ������ ����˴ϴ�." << endl;
			bufInputKey = _getch();
			break;
		}

		//�Էº�
		cout << "�Է�Ű �ȳ� : W-����, A-����, S-�Ʒ���, D-������, X-����" << endl;
		bufInputKey = _getch();
		//cout << bufInputKey << endl;

		//���ӵ����� ������Ʈ
		update(bufInputKey, state, bufStageWidth, bufStageHeight);
	}
	delete state;
}


//������ �ʱ⼳��
void initialize(object* state, const string stageData, const int width, const int height)	
{
	//const string sRead = stageData;	//�б��
	int x = 0;						//�迭x��
	int y = 0;						//�迭y��
	
	for(int i = 0;i < stageData.length(); i++)
	{
		object bufObj; //state�Է¿�

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
			//���๮�ڸ� ������ x�� 0���� ������ y�� �ϳ� �����ϰ� state���� �ƹ��͵� �߰������ʴ´�.
		default : bufObj = OBJ_UNKNOWN;
		}

		if(bufObj != OBJ_UNKNOWN)
		{
			//�˼����� �����Ͱ� �ƴҰ�� state�� �߰�
			state[y * width + x] = bufObj;
			x++;
		}
	}
}

//ȭ��׸���
void draw(const object* state, const int width, const int height)
{
	system("cls");
	object readDraw; //�б��

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			//x�� ����
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
		//��ٲ�
		cout << endl;
	}
}

//������ ������Ʈ
void update(char inputKey, object* state, const int width, const int height)
{
	int playerPos = 0;
	int subjectPos = 0;

	//�÷��̾� ��ġ�ľ�
	playerPos = findPlayerPos(state, width, height);
	//cout << findPlayerPos(state, width, height) << endl;

	//�Էµ� Ű�� ��������
	//�� ������Ʈ ��ġ���� ����
	//�迭÷�ڰ�
	//�����Է��� +-width
	//�����Է��� +-1
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
	default : subjectPos = -1; cout << "�߸��� Ű�Է��Դϴ�." << endl;
	}

	if((subjectPos < 0)||(subjectPos > (width * height)))
	{
		//��ǥ��ġ�� �迭������ ������ �ƹ��͵� ��������
	}
	else
	{
		//�迭������ ����� �������, ��ġ����
		if(state[playerPos] == OBJ_MAN)
		{//�÷��̾� ��ġ�� �� ���� �ƴѰ��
			if(state[subjectPos] == OBJ_SPACE)
			{
				//��ǥ��ġ�� ��ĭ�� ��� �״�� �̵�
				state[subjectPos] = OBJ_MAN;
				state[playerPos] = OBJ_SPACE;
			}
			else if(state[subjectPos] == OBJ_GOAL)
			{
				//��ǥ��ġ�� ���� ���
				state[subjectPos] = OBJ_MAN_ON_GOAL;
				state[playerPos] = OBJ_SPACE;
			}
			else if(state[subjectPos] == OBJ_BLOCK)
			{
				//��ǥ��ġ�� ����� ���
				//������� 2ĭ���� �˻�
				if(state[subjectPos + (subjectPos - playerPos)] == OBJ_SPACE)
				{
					//��ĭ�̸� ��ϰ� �÷��̾ ����
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK;
					state[subjectPos] = OBJ_MAN;
					state[playerPos] = OBJ_SPACE;
				}
				else if(state[subjectPos + (subjectPos - playerPos)] == OBJ_GOAL)
				{
					//���̸� ����� ������ �ִ� ���·� �ٲ㼭 �̵�
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK_ON_GOAL;
					state[subjectPos] = OBJ_MAN;
					state[playerPos] = OBJ_SPACE;
				}
			}
			else if(state[subjectPos] == OBJ_BLOCK_ON_GOAL)
			{
				//��ǥ��ġ�� ������ ����� ���
				//������� 2ĭ���� �˻�
				if(state[subjectPos + (subjectPos - playerPos)] == OBJ_SPACE)
				{
					//��ĭ�̸� ��ϰ� �÷��̾ ����
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK;
					state[subjectPos] = OBJ_MAN_ON_GOAL;
					state[playerPos] = OBJ_SPACE;
				}
				else if(state[subjectPos + (subjectPos - playerPos)] == OBJ_GOAL)
				{
					//���̸� ����� ������ �ִ� ���·� �ٲ㼭 �̵�
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK_ON_GOAL;
					state[subjectPos] = OBJ_MAN_ON_GOAL;
					state[playerPos] = OBJ_SPACE;
				}
			}
		}
		else if(state[playerPos] == OBJ_MAN_ON_GOAL)
		{//�÷��̾� ��ġ�� �� ���� ���
			if(state[subjectPos] == OBJ_SPACE)
			{
				//��ǥ��ġ�� ��ĭ�� ��� �״�� �̵�
				state[subjectPos] = OBJ_MAN;
				state[playerPos] = OBJ_GOAL;
			}
			else if(state[subjectPos] == OBJ_GOAL)
			{
				//��ǥ��ġ�� ���� ���
				state[subjectPos] = OBJ_MAN_ON_GOAL;
				state[playerPos] = OBJ_GOAL;
			}
			else if(state[subjectPos] == OBJ_BLOCK)
			{
				//��ǥ��ġ�� ����� ���
				//������� 2ĭ���� �˻�
				if(state[subjectPos + (subjectPos - playerPos)] == OBJ_SPACE)
				{
					//��ĭ�̸� ��ϰ� �÷��̾ ����
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK;
					state[subjectPos] = OBJ_MAN;
					state[playerPos] = OBJ_GOAL;
				}
				else if(state[subjectPos + (subjectPos - playerPos)] == OBJ_GOAL)
				{
					//���̸� ����� ������ �ִ� ���·� �ٲ㼭 �̵�
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK_ON_GOAL;
					state[subjectPos] = OBJ_MAN;
					state[playerPos] = OBJ_GOAL;
				}
			}
			else if(state[subjectPos] == OBJ_BLOCK_ON_GOAL)
			{
				//��ǥ��ġ�� ������ ����� ���
				//������� 2ĭ���� �˻�
				if(state[subjectPos + (subjectPos - playerPos)] == OBJ_SPACE)
				{
					//��ĭ�̸� ��ϰ� �÷��̾ ����
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK;
					state[subjectPos] = OBJ_MAN_ON_GOAL;
					state[playerPos] = OBJ_GOAL;
				}
				else if(state[subjectPos + (subjectPos - playerPos)] == OBJ_GOAL)
				{
					//���̸� ����� ������ �ִ� ���·� �ٲ㼭 �̵�
					state[subjectPos + (subjectPos - playerPos)] = OBJ_BLOCK_ON_GOAL;
					state[subjectPos] = OBJ_MAN_ON_GOAL;
					state[playerPos] = OBJ_GOAL;
				}
			}
		}
	}
}

//�¸����� üũ
bool winCheck(const object* state, const int width, const int height)
{
	for(int i = 0; i < (width * height); i++)
	{
		//GOAL�� �ö��������� ����� �ִٸ� ���� Ŭ������� ���� ������ ����.
		if(state[i] == OBJ_BLOCK)
		{
			return false;
		}
	}
	return true;
}

//�÷��̾� ��ġ�˻�
int findPlayerPos(const object* state, const int width, const int height)
{
	int i = 0; //÷�ڿ�

	for(i; i < (width * height); i++)
	{
		//�÷��̾� ��ġ ÷�ڸ� ��ȯ
		if(state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;
		}
	}

	return i;
}

//�����а� �������������͸� ����Ʈ�� �Է�
void fReadStageData(list<stageData>& stageList)
{
	string dataStr = "";
	string noteStr = "";
	string objStr = "";
	int lineNum = 0;
	int pathFlag = 1; // 1:���������ѹ�, 2:������������, 3:������������, 4:��������������Ʈ
	int objFlag = 0;

	int bufStageNum = 0;				//����-����������ȣ
	string bufStageObj = "";			//����-��������������Ʈ����
	int bufStageWidth = 0;				//����-�����������α���
	int bufStageHeight = 0;				//����-�����������α���
	stageData bufStageData;				//����-�������������� ����Ʈ

//	bufStageData = new stageData;

	ifstream readFile;
	readFile.open("Resource/stageData.txt", ios::in);

	//���پ� �о���̱�
	while(getline(readFile, dataStr))
	{
		//�ּ����� Ȯ���ϱ� ���� ù ���� �˻�
		noteStr = dataStr.at(0);

		//ù ���ڰ� - �̸� �ּ��̹Ƿ� ������ó���� ��������
		if(noteStr.compare("-") != 0)
		{
			if(pathFlag == 1) //Flag = 1 : StageNum
			{
				//������ ��ȯ�ؼ� �Է�
				bufStageNum = atoi(dataStr.c_str()); /* c++11 ȯ�濡�� stoi()�Լ��̿� */

				//Flag�� 2�� �÷��� ���α��̸� ���� �� �ֵ��� ����
				pathFlag = 2;
			}
			else if(pathFlag == 2) //Flag = 2 : StageWidth
			{
				//�����͹��Ἲ üũ
				if(bufStageNum < 1)
				{
					cout << bufStageNum 
						 << "���������� ��ȣ �����Ϳ���(" << bufStageNum << ")�� ���α��̸� ���� �� �����ϴ�." << endl 
						 << "���α׷��� �����մϴ�." << endl;
					Sleep(2000);
					exit(1);
				}

				//������ ��ȯ�ؼ� �Է�
				bufStageWidth = atoi(dataStr.c_str()); /* c++11 ȯ�濡�� stoi()�Լ��̿� */

				//Flag�� 3���� �÷��� ���α��̸� ���� �� �ֵ��� ����
				pathFlag = 3;
			}
			else if(pathFlag == 3) //Flag = 3 : StageHeight
			{
				//�����͹��Ἲ üũ
				if(bufStageWidth < 1)
				{
					cout << bufStageNum 
						 << "���������� ���α��� �����Ϳ���(" << bufStageWidth << ")�� ���α��̸� ���� �� �����ϴ�." << endl 
						 << "���α׷��� �����մϴ�." << endl;
					Sleep(2000);
					exit(1);
				}

				//������ ��ȯ�ؼ� �Է�
				bufStageHeight = atoi(dataStr.c_str()); /* c++11 ȯ�濡�� stoi()�Լ��̿� */

				//Flag�� 4�� �÷��� ������Ʈ�� ���� �� �ֵ��� ����
				pathFlag = 4;
			}
			else if(pathFlag == 4) //Flag = 4 : StageObj
			{
				//�����͹��Ἲ üũ
				if(bufStageHeight < 1)
				{
					cout << bufStageNum 
						 << "���������� ���α��� �����Ϳ���(" << bufStageHeight << ")�� ������Ʈ�� ���� �� �����ϴ�." << endl 
						 << "���α׷��� �����մϴ�." << endl;
					Sleep(2000);
					exit(1);
				}

				//�������� ���α���bufStageHeight��ŭ �ݺ�
				objFlag++;

				objStr.append(dataStr);
				objStr.append("\n");

				//bufStageHeight��ŭ �ݺ��� ������ ����Ʈ�� �ְ�
				//������ Flag�� �ʱ�ȭ�Ͽ� ���ο� ���������� ���� �� �ֵ��� ����
				if(objFlag == bufStageHeight)
				{
					//������ objStr�� char������ ��ȯ
					bufStageObj = objStr;
					//cout << bufStageObj << endl; - ����׿�

					//����Ʈ�� �ֱ�
					bufStageData.set_stageData(bufStageNum,bufStageObj,bufStageWidth,bufStageHeight);
					stageList.push_back(bufStageData);

					//�ʱ�ȭ
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