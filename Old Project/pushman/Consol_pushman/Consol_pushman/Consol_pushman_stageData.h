#include <string>

class stageData
{
private:
	int stageNum;		//���° ������������ ����
	std::string stageObj;		//���������� ������Ʈ�� ���¸� ����
	int stageWidth;		//���������� ���α���
	int stageHeight;	//���������� ���α���
public:
	//������
	stageData();
	//�Ҹ���
	~stageData();

	//�ʵ����� �Է¹޴� �Լ�
	void set_stageData(int fStageNumData, std::string fstageObj, int fStageWidth, int fStageHeight);
	//�������� ��ȣ�� ��ȯ�ϴ� �Լ�
	int get_stageNum();
	//�������� ������Ʈ���� �����͸� ��ȯ�ϴ� �Լ�
	std::string get_stageObj();
	//���������� ���α��̸� ��ȯ�ϴ� �Լ�
	int get_stageWidth();
	//���������� ���α��̸� ��ȯ�ϴ� �Լ�
	int get_stageHeight();
};