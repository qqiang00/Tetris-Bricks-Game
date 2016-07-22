#pragma once

enum BricksType {
	//�������ͣ�enumֵС��10
	BT_Square = 0,		//������2*2
	BT_Line = 1,			//����������****
	BT_Seven = 2,		//������7
	BT_AntiSeven = 3,	//������7
	BT_Step = 4,			//����Ҹ�
	BT_AntiStep = 5,		//����ҵ�
	BT_Mountain = 6		//ɽ�У�
	//�߼����ͣ�enumֵ��11��ʼ
};
enum Orientation {
	Up = 0,
	Right = 1,
	Down = 2,
	Left = 3,
	Unknow=4
};
enum RotateStrategy {
	ClockWise = 0,//˳ʱ��
	AntiClockWise=1//��ʱ��
};
//��������
struct Brick
{
public:
	int colIndex=0;
	int rowIndex=0;
	//bool isactive;
	/*Brick() {
		rowIndex = colIndex = 0;
	}*/
	void Move(int dcol, int drow) {
		 colIndex+= dcol; rowIndex += drow;
	}
	void MoveDown() { rowIndex += 1; }
	void MoveUp() { rowIndex -= 1; }
	void MoveLeft() { colIndex -= 1; }
	void MoveRight() { colIndex += 1; }
	void MoveDownLeft() { rowIndex += 1; colIndex -= 1; }
	void MoveDownRight() { rowIndex += 1; colIndex += 1; }
	void MoveUpLeft() { rowIndex -= 1; colIndex -= 1; }
	void MoveUpRight() { rowIndex -= 1; colIndex += 1; }
};
//һ�����飺���ƶ���������������γ�һ������Ϸ�����������ƶ��ķ������
//��������ֻ���߼����㣬�������ͼ����ͼ����ר�ŵ�GraphicsManager�ࡣ
class Bricks
{
private:
	void InitBricks(bool);
	void InitBricksIndex();
//	void InitBricks(BricksType);
public:
	//���������
	BricksType brkType;
	//����ĳ���
	Orientation brkOrient;
	//���������еĵ������������
	int number;
	Brick* body;
public:
	Bricks();
	Bricks(bool);
//	Bricks(BricksType);
	~Bricks();
	void CreateBricks();
//	void CreateBricks(BricksType);
//	void CreateBricks(bool isbasictype);
	void RotateClockWise();
	void RotateAntiClockWise();
	void Rotate(bool isclockwise);
	//����һ�������λ�ã�ʹ�����·��ķ��鵥Ԫ��λ�þ����ܵĽӽ�׼�����ĵײ��Է���
	//��Ų����Ϸ���Ϸ�ʱ�ܾ��������Ϸ����ʾ������
	void AdjustPosition();
	void MoveUp();
	void MoveUp(int);
	void MoveDown();
	void MoveDown(int);
	void MoveLeft();
	void MoveLeft(int);
	void MoveRight();
	void MoveRight(int);
	int GetMostLeftColIndex();
	int GetMostRightColIndex();
	int GetMostDownRowIndex();
	int GetMostUpRowIndex();
	void Copy(Bricks);
};

