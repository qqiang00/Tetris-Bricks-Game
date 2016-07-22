#include "stdafx.h"
#include "Bricks.h"
//初始化方块内的索引值
void Bricks::InitBricksIndex() {
	switch (brkType) {
	case BT_Square://正方形
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
//使用方块类型创建一个方块
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
	if (isbasic) {//基本方块
		number = 4;
		brkType = BricksType(UINT(st.wMilliseconds) % 7);
		body = new Brick[number];//4为该类型方块包含的单个方块数量
	}
	else {//含有一些高级方块
		number = 4;
		brkType = BricksType(UINT(st.wMilliseconds) % 7);//这里的数字需要更改
		body = new Brick[number];
	}
	brkOrient = Up;
	//给每一个方块赋值，赋值代表的是相对位置关系，最下方的
	//其它的在此基础上做减法
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
	//增加了释放内存这句就出错
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
//调整一个方块的位置，使其最下方的方块单元的位置尽可能的接近准备区的底部以方便
//当挪至游戏区上方时能尽快的在游戏区显示出来。
void Bricks::AdjustPosition() {
	//先找出四个坐标点rowIndex最大的值，并且计算其与0之间的差值，然后每一个方块单元的rowIndex加上这个差值。
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
//向上移动方块：将方块内的所有方块单元的行索引-1
void Bricks::MoveUp() {
	MoveUp(1);
}
//向下移动方块：将方块内的所有方块单元的行索引+step
void Bricks::MoveDown(int step)
{
	for (int i = 0; i < number; i++) {
		body[i].rowIndex += step;
	}
}
//向下移动方块：将方块内的所有方块单元的行索引+1
void Bricks::MoveDown() {
	MoveDown(1);
}
//向左移动方块：将方块内的所有方块单元的列索引-step
void Bricks::MoveLeft(int step) {
	for (int i = 0; i < number; i++) {
		body[i].colIndex -= step;
	}
}
//向左移动方块：将方块内的所有方块单元的列索引-1
void Bricks::MoveLeft() {
	MoveLeft(1);
}
//向右移动方块：将方块内的所有方块单元的行索引+step
void Bricks::MoveRight(int step) {
	for (int i = 0; i < number; i++) {
		body[i].colIndex += step;
	}
}
//向右移动方块：将方块内的所有方块单元的行索引+1
void Bricks::MoveRight() {
	MoveRight(1);
}
//获取Bricks中众多Brick的行索引中的最大值
int Bricks::GetMostDownRowIndex() {
	int result = -10;
	for (int i = 0; i < number; i++) {
		if (body[i].rowIndex >= result) {
			result = body[i].rowIndex;
		}
	}
	return result;
}
//获取Bricks中众多Brick的行索引中的最小值
int Bricks::GetMostUpRowIndex() {
	int result = 20;
	for (int i = 0; i < number; i++) {
		if (body[i].rowIndex <= result) {
			result = body[i].rowIndex;
		}
	}
	return result;
}
//获取Bricks中众多Brick的列索引中的最小值
int Bricks::GetMostLeftColIndex() {
	int result = 10;
	for (int i = 0; i < number; i++) {
		if (body[i].colIndex <= result) {
			result = body[i].colIndex;
		}
	}
	return result;
}
//获取Bricks中众多Brick的列索引中的最大值
int Bricks::GetMostRightColIndex() {
	int result = 0;
	for (int i = 0; i < number; i++) {
		if (body[i].colIndex >= result) {
			result = body[i].colIndex;
		}
	}
	return result;
}
//将参数中的Bricks的数据值（不是引用）拷贝至自身
void Bricks::Copy(Bricks src) {
	this->brkType = src.brkType;
	this->brkOrient = src.brkOrient;
	this->number = src.number;
	for (int i = 0; i < number; i++) {
		body[i].rowIndex = src.body[i].rowIndex;
		body[i].colIndex = src.body[i].colIndex;
	}
}