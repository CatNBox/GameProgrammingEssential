#include "Consol_pushman_stageData.h"


stageData::stageData()
{
	//생성자
}

stageData::~stageData()
{
	//소멸자
}

void stageData::set_stageData(int fStageNum, std::string fstageObj, int fStageWidth, int fStageHeight)
{
	//스테이지 데이터를 받아서 저장
	stageNum = fStageNum;
	stageObj = fstageObj;
	stageWidth = fStageWidth;
	stageHeight = fStageHeight;
}

int stageData::get_stageNum()
{
	//스테이지 번호를 반환
	return stageNum;
}

int stageData::get_stageWidth()
{
	//스테이지 가로길이를 반환
	return stageWidth;
}

int stageData::get_stageHeight()
{
	//스테이지 세로길이를 반환
	return stageHeight;
}

std::string stageData::get_stageObj()
{
	//스테이지 오브젝트들의 데이터를 반환
	return stageObj;
}