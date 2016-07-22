#pragma once

enum BricksType {
	//基本类型，enum值小于10
	BT_Square = 0,		//正方形2*2
	BT_Line = 1,			//横条或竖条****
	BT_Seven = 2,		//像数字7
	BT_AntiSeven = 3,	//反数字7
	BT_Step = 4,			//左低右高
	BT_AntiStep = 5,		//左高右低
	BT_Mountain = 6		//山行，
	//高级类型，enum值从11开始
};
enum Orientation {
	Up = 0,
	Right = 1,
	Down = 2,
	Left = 3,
	Unknow=4
};
enum RotateStrategy {
	ClockWise = 0,//顺时针
	AntiClockWise=1//逆时针
};
//单个方块
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
//一个方块：特制多个单个方块的组合形成一个在游戏场景中整体移动的方块组合
//在这个类里，只做逻辑运算，不参与绘图，绘图交给专门的GraphicsManager类。
class Bricks
{
private:
	void InitBricks(bool);
	void InitBricksIndex();
//	void InitBricks(BricksType);
public:
	//方块的类型
	BricksType brkType;
	//方块的朝向
	Orientation brkOrient;
	//方块所含有的单个方块的数量
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
	//调整一个方块的位置，使其最下方的方块单元的位置尽可能的接近准备区的底部以方便
	//当挪至游戏区上方时能尽快的在游戏区显示出来。
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

