#include "Consol_pushman_stageData.h"


stageData::stageData()
{
	//������
}

stageData::~stageData()
{
	//�Ҹ���
}

void stageData::set_stageData(int fStageNum, std::string fstageObj, int fStageWidth, int fStageHeight)
{
	//�������� �����͸� �޾Ƽ� ����
	stageNum = fStageNum;
	stageObj = fstageObj;
	stageWidth = fStageWidth;
	stageHeight = fStageHeight;
}

int stageData::get_stageNum()
{
	//�������� ��ȣ�� ��ȯ
	return stageNum;
}

int stageData::get_stageWidth()
{
	//�������� ���α��̸� ��ȯ
	return stageWidth;
}

int stageData::get_stageHeight()
{
	//�������� ���α��̸� ��ȯ
	return stageHeight;
}

std::string stageData::get_stageObj()
{
	//�������� ������Ʈ���� �����͸� ��ȯ
	return stageObj;
}