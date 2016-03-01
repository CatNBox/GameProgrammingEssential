#include <string>

class stageData
{
private:
	int stageNum;		//몇번째 스테이지인지 저장
	std::string stageObj;		//스테이지의 오브젝트들 형태를 저장
	int stageWidth;		//스테이지의 가로길이
	int stageHeight;	//스테이지의 세로길이
public:
	//생성자
	stageData();
	//소멸자
	~stageData();

	//맵데이터 입력받는 함수
	void set_stageData(int fStageNumData, std::string fstageObj, int fStageWidth, int fStageHeight);
	//스테이지 번호를 반환하는 함수
	int get_stageNum();
	//스테이지 오브젝트들의 데이터를 반환하는 함수
	std::string get_stageObj();
	//스테이지의 가로길이를 반환하는 함수
	int get_stageWidth();
	//스테이지의 세로길이를 반환하는 함수
	int get_stageHeight();
};