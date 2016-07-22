#include "stdafx.h"
#include "Bricks.h"
//��ʼ�������ڵ�����ֵ
void Bricks::InitBricksIndex() {
	switch (brkType) {
	case BT_Square://������
		body[0].colIndex = 1; body[0].rowIndex = 0;
		body[1].colIndex = 2; body[1].rowIndex = 0;	
		body[2].colIndex = 1; body[2].rowIndex = -1;	
		body[3].colIndex = 2; body[3].rowIndex = -1;
		break;
	case BT_Line://
		body[0].colIndex = 0, body[0].rowIndex = -2;
		body[1].colIndex = 1; body[1].rowIndex = -2;	
		body[2].colIndex = 2; body[2].rowIndex = -2;		
		body[3].colIndex = 3; body[3].rowIndex = -2;
		break;
	case BT_Seven:
		body[0].colIndex = 1, body[0].rowIndex = -2;
		body[1].colIndex = 2; body[1].rowIndex = -2;
		body[2].colIndex = 2; body[2].rowIndex = -1;	
		body[3].colIndex = 2; body[3].rowIndex = 0;
		break;
	case BT_AntiSeven:
		body[0].colIndex = 2, body[0].rowIndex = -2;	
		body[1].colIndex = 1; body[1].rowIndex = -2;	
		body[2].colIndex = 1; body[2].rowIndex = -1;	
		body[3].colIndex = 1; body[3].rowIndex = 0;
		break;
	case BT_Step:
		body[0].colIndex = 0, body[0].rowIndex = -1;	
		body[1].colIndex = 0; body[1].rowIndex = 0;	
		body[2].colIndex = 1; body[2].rowIndex = -1;
		body[3].colIndex = 1; body[3].rowIndex = -2;
		break;
	case BT_AntiStep:
		body[0].colIndex = 1, body[0].rowIndex = -2;	
		body[1].colIndex = 1; body[1].rowIndex = -1;	
		body[2].colIndex = 2; body[2].rowIndex = -1;	
		body[3].colIndex = 2; body[3].rowIndex = 0;
		break;
	case BT_Mountain:
		body[0].colIndex = 0, body[0].rowIndex = -1;	
		body[1].colIndex = 1; body[1].rowIndex = -2;
		body[2].colIndex = 1; body[2].rowIndex = -1;		
		body[3].colIndex = 2; body[3].rowIndex = -1;
		break;
	}
}
//ʹ�÷������ʹ���һ������
//void Bricks::CreateBricks(BricksType type) {
//	number = 4;
//	brkType = type;
//	brkOrient = Up;
//	InitBricksIndex();
//	AdjustPosition();
//}
SYSTEMTIME st;
void Bricks::CreateBricks() {
	//number = 4;
	GetLocalTime(&st);
	if (number == 4) {
		brkType = BricksType(UINT(st.wMilliseconds*rand()) % 7);
	}
	else {
		brkType = BricksType(UINT(st.wMilliseconds)*rand() % 7);
	}
	brkOrient = Up;
	InitBricksIndex();
	AdjustPosition();
}

void Bricks::InitBricks(bool isbasic) {
	if (isbasic) {//��������
		number = 4;
		brkType = BricksType(UINT(st.wMilliseconds) % 7);
		body = new Brick[number];//4Ϊ�����ͷ�������ĵ�����������
	}
	else {//����һЩ�߼�����
		number = 4;
		brkType = BricksType(UINT(st.wMilliseconds) % 7);//�����������Ҫ����
		body = new Brick[number];
	}
	brkOrient = Up;
	//��ÿһ�����鸳ֵ����ֵ����������λ�ù�ϵ�����·���
	//�������ڴ˻�����������
	InitBricksIndex();
	AdjustPosition();
}

Bricks::Bricks(bool isbasic) {
	InitBricks(isbasic);
}
Bricks::Bricks()
{
	InitBricks(true);
}


Bricks::~Bricks()
{
	//�������ͷ��ڴ����ͳ���
//	delete[] body;
}
void Bricks::Rotate(bool isclockwise) {
	if (isclockwise) {
		RotateClockWise();
	}
	else {
		RotateAntiClockWise();
	}
}
void Bricks::RotateAntiClockWise() {
	switch (brkType) {
	case BT_Square:
		RotateClockWise();
		break;
	case BT_Line:
		RotateClockWise();
		break;
	case BT_Seven:
		RotateClockWise(); RotateClockWise(); RotateClockWise();
		break;
	case BT_AntiSeven:
		RotateClockWise(); RotateClockWise(); RotateClockWise();
		break;
	case BT_Step:
		RotateClockWise();
		break;
	case BT_AntiStep:
		RotateClockWise(); 
		break;
	case BT_Mountain:
		RotateClockWise(); RotateClockWise(); RotateClockWise();
		break;
	}
}
void Bricks::RotateClockWise() {
	switch (brkType) {
	case BT_Square:
		break;
	case BT_Line:
		switch (brkOrient) {
			case Up:
				body[0].MoveUpRight(); body[2].MoveDownLeft(); body[3].Move(-2, 2);
				brkOrient = Right;
				break;
			case Right:
				body[0].MoveDownLeft(); body[2].MoveUpRight(); body[3].Move(2, -2);
				brkOrient = Up;
				break;
			default:break;
		}
		break;
	case BT_Seven:
		switch (brkOrient) {
		case Up:
			body[0].MoveDown(); body[3].Move(-2, -1);
			brkOrient = Right;
			break;
		case Right:
			body[1].MoveLeft(); body[3].MoveDownRight(); body[2].MoveDown();
			brkOrient = Down;
			break;
		case Down:
			body[2].Move(0, -2); body[3].Move(2, -2);
			brkOrient = Left;
			break;
		case Left:
			body[3].Move(-1, 2); body[2].MoveDown(); body[1].MoveRight(); body[0].MoveUp();
			brkOrient = Up;
			break;
		default:break;
		}
		break;
	case BT_AntiSeven:
		switch (brkOrient) {
		case Up:
			body[2].MoveRight(); body[3].Move(-1, -2);
			brkOrient = Right;
			break;
		case Right:
			body[3].Move(1, 2); body[1].Move(1, 2);
			brkOrient = Down;
			break;
		case Down:
			body[0].MoveLeft(); body[3].MoveUp(); body[1].MoveUpRight();
			brkOrient = Left;
			break;
		case Left:
			body[0].MoveRight(); body[1].Move(-2,-1); body[2].MoveLeft(); body[3].MoveDown();
			brkOrient = Up;
			break;
		default:
			break;
		}
		break;
	case BT_Step:
		switch (brkOrient) {
		case Up:
			body[1].MoveRight(); body[3].Move(1, 2);
			brkOrient = Right;
			break;
		case Right:
			body[1].MoveLeft(); body[3].Move(-1, -2);
			brkOrient = Up;
			break;
		default:
			break;
		}
		break;
	case BT_AntiStep:
		switch (brkOrient) {
		case Up:
			body[3].MoveLeft(); body[0].Move(-1, 2);
			brkOrient = Right;
			break;
		case Right:
			body[3].MoveRight(); body[0].Move(1, -2);
			brkOrient = Up;
			break;
		default:break;
		}
		break;
	case BT_Mountain:
		switch (brkOrient) {
		case Up:
			body[0].MoveDownRight();
			brkOrient = Right;
			break;
		case Right:
			body[1].MoveDownLeft();
			brkOrient =Down;
			break;
		case Down:
			body[3].MoveUpLeft();
			brkOrient = Left;
			break;
		case Left:
			body[0].MoveUpLeft(); body[1].MoveUpRight(); body[3].MoveDownRight();
			brkOrient = Up;
			break;
		default:break;
		}
		break;
	default:
		break;
	}
}
//����һ�������λ�ã�ʹ�����·��ķ��鵥Ԫ��λ�þ����ܵĽӽ�׼�����ĵײ��Է���
//��Ų����Ϸ���Ϸ�ʱ�ܾ��������Ϸ����ʾ������
void Bricks::AdjustPosition() {
	//���ҳ��ĸ������rowIndex����ֵ�����Ҽ�������0֮��Ĳ�ֵ��Ȼ��ÿһ�����鵥Ԫ��rowIndex���������ֵ��
	int max = -100, i = 0;
	for (i = 0; i < number; i++) {
		if (body[i].rowIndex > max) {
			max = body[i].rowIndex;
		}
	}
	for (i = 0; i < number; i++) 
		body[i].rowIndex -= max+1;
}

void Bricks::MoveUp(int step) {
	for (int i = 0; i < number; i++) {
		body[i].rowIndex -= step;
	}
}
//�����ƶ����飺�������ڵ����з��鵥Ԫ��������-1
void Bricks::MoveUp() {
	MoveUp(1);
}
//�����ƶ����飺�������ڵ����з��鵥Ԫ��������+step
void Bricks::MoveDown(int step)
{
	for (int i = 0; i < number; i++) {
		body[i].rowIndex += step;
	}
}
//�����ƶ����飺�������ڵ����з��鵥Ԫ��������+1
void Bricks::MoveDown() {
	MoveDown(1);
}
//�����ƶ����飺�������ڵ����з��鵥Ԫ��������-step
void Bricks::MoveLeft(int step) {
	for (int i = 0; i < number; i++) {
		body[i].colIndex -= step;
	}
}
//�����ƶ����飺�������ڵ����з��鵥Ԫ��������-1
void Bricks::MoveLeft() {
	MoveLeft(1);
}
//�����ƶ����飺�������ڵ����з��鵥Ԫ��������+step
void Bricks::MoveRight(int step) {
	for (int i = 0; i < number; i++) {
		body[i].colIndex += step;
	}
}
//�����ƶ����飺�������ڵ����з��鵥Ԫ��������+1
void Bricks::MoveRight() {
	MoveRight(1);
}
//��ȡBricks���ڶ�Brick���������е����ֵ
int Bricks::GetMostDownRowIndex() {
	int result = -10;
	for (int i = 0; i < number; i++) {
		if (body[i].rowIndex >= result) {
			result = body[i].rowIndex;
		}
	}
	return result;
}
//��ȡBricks���ڶ�Brick���������е���Сֵ
int Bricks::GetMostUpRowIndex() {
	int result = 20;
	for (int i = 0; i < number; i++) {
		if (body[i].rowIndex <= result) {
			result = body[i].rowIndex;
		}
	}
	return result;
}
//��ȡBricks���ڶ�Brick���������е���Сֵ
int Bricks::GetMostLeftColIndex() {
	int result = 10;
	for (int i = 0; i < number; i++) {
		if (body[i].colIndex <= result) {
			result = body[i].colIndex;
		}
	}
	return result;
}
//��ȡBricks���ڶ�Brick���������е����ֵ
int Bricks::GetMostRightColIndex() {
	int result = 0;
	for (int i = 0; i < number; i++) {
		if (body[i].colIndex >= result) {
			result = body[i].colIndex;
		}
	}
	return result;
}
//�������е�Bricks������ֵ���������ã�����������
void Bricks::Copy(Bricks src) {
	this->brkType = src.brkType;
	this->brkOrient = src.brkOrient;
	this->number = src.number;
	for (int i = 0; i < number; i++) {
		body[i].rowIndex = src.body[i].rowIndex;
		body[i].colIndex = src.body[i].colIndex;
	}
}