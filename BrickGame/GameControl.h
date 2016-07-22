#pragma once
//游戏控制类，该类封装了GraphicsManager类的绘制功能
//
enum GameStatus {
	GS_PrepareForStart=0,
	GS_Running=1,
	GS_Paused=2,
	GS_GameOver=4
};
class GameControl
{
private:
	bool gameArea[MAX_COLS][MAX_ROWS];//记录游戏区域所有方块状态，false标识未激活占用
	bool nextArea[4][4];//记录在Next区域所有方块状态，false标识未激活占用
	int activeCellCountInRow[MAX_ROWS];//某一行内激活的cells的数量,数组内的元素在方块不能继续下降时和对行整体操作时更新
	long score;//当前得分
	long hiScore;//最高分
	int rowsErased;//已经消除的行数
	int rowsPreset;//预先设定的存在随机方块的行数
	Bricks nextBricks;//下一个Bricks,显示在Next区
	Bricks curBricks;//当前Bricks
	Bricks shadowBricks;//影子方块，用于临时存储当前Bricks操作后所可能占据的空间，
						//帮助判断针对curBricks的移动或旋转操作是否可以顺利进行
	double speed;//运行速度

	bool isClockWiseRotate;
	bool lock = false;


public:
	GraphicsManager graphicsMgr;//绘图管理对象
	GameStatus curStatus = GS_GameOver;
	int level;//当前等级
private:
	//初始化个参数
	void Initiate() {
		for (int row = 0; row < MAX_ROWS; row++) {
			for (int col = 0; col < MAX_COLS; col++) {
				if (col < 4 && row < 4) {
					nextArea[col][row] = false;
					//graphicsMgr.DrawBrickAtNextArea(x, y, nextArea[x][y]);
				}
				gameArea[col][row] = false;
				//graphicsMgr.DrawBrick(x, y, gameArea[x][y]);
			}
			activeCellCountInRow[row] = 0;
		}
		score = 0;
		hiScore = 0;
		rowsErased = 0;
		rowsPreset = 0;
		//nextBricks和curBricks调用默认无参的构造函数
		nextBricks = Bricks(true);
		curBricks = Bricks(true);
		shadowBricks = Bricks(true);
		level = 1;
		isClockWiseRotate = true;
	}
	//设置gameArea内单元的状态，如果状态改变了，绘制该单元
	void SetCellStatus(int col, int row, bool isactive) {
		if (gameArea[col][row] != isactive) {
			gameArea[col][row] = isactive;
			graphicsMgr.DrawBrick(col, row, isactive);
		}
	}
	//将单元设置为活动（占用）状态
	void SetCellActive(int col, int row) {
		//这里不更改activeCellNumberInRow数组内相应行的数值
		SetCellStatus(col, row, true);
	}
	//将单元设置为非活动（占用）状态
	void SetCellInActive(int col, int row) {
		SetCellStatus(col, row, false);
	}
	//获得单元格的填充激活状态
	bool GetCellStatus(int col, int row) {
		if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) {
			return false;
		}
		else {
			return gameArea[col][row];
		}
	}
	//设置Next区域状态
	void SetNextAreaStatus(bool status) {
		for (int colindex = 0; colindex < 4; colindex++) {
			for (int rowindex = 0; rowindex < 4; rowindex++)
			{
				nextArea[colindex][rowindex] = status;
				graphicsMgr.DrawBrickAtNextArea(colindex, rowindex, status);
			}
		}
	}
	//更新游戏区各个位置被方块占据的状态，在方块不能进一步下降时使用。
	void UpdateGameAreaStatus() {
		for (int i = 0; i < curBricks.number; i++) {
			SetCellActive(curBricks.body[i].colIndex, curBricks.body[i].rowIndex);
		}
	}
	//设置某一整行的状态
	void SetRowStatus(int rowindex, bool status) {
		for (int colindex = 0; colindex < MAX_COLS; colindex++) {
			SetCellStatus(colindex, rowindex, status);
		}
	}
	//激活一整行
	void SetRowActive(int rowindex) {
		SetRowStatus(rowindex, true);
		activeCellCountInRow[rowindex] = MAX_COLS;
	}
	//消除一整行
	void SetRowInActive(int rowindex) {
		SetRowStatus(rowindex, false);
		Sleep(50);
		SetRowStatus(rowindex, true);
		Sleep(50);
		SetRowStatus(rowindex, false);
		activeCellCountInRow[rowindex] = 0;
	}

	
	//根据其参数状态显示某一行。
	void DisplayRow(int rowindex) {
		for (int colindex = 0; colindex < MAX_COLS; colindex++) {
			SetCellStatus(colindex, rowindex, gameArea[colindex][rowindex]);
		}
	}
	//向下移动整行，这会覆盖目标行的数据，建议在MoveDown之前先把目标行的所有cell设为Inactive，num>0,往下移动
	void MoveDownRow(int rowindex,int num) {
		if (rowindex >= MAX_ROWS || rowindex + num >= MAX_ROWS || rowindex<0 || rowindex+num<0 || num==0) {
			return;
		}
		for (int col = 0; col < MAX_COLS; col++) {
			SetCellStatus(col, rowindex + num, gameArea[col][rowindex]);
			SetCellStatus(col, rowindex, false);
		}
		activeCellCountInRow[rowindex + num] = activeCellCountInRow[rowindex];
		activeCellCountInRow[rowindex] = 0;
	}
	//针对ShadowBricks判断是否越界
	//1.在游戏区上方时，当方块没有完全显示在游戏区时最左的列索引不得小于3，最右的列索引不得大于6
	//2.在游戏区时：最左的列索引不得小于0，最右的列索引不得大于9
	bool IsOutOfBorder() {
		for (int i = 0; i < shadowBricks.number; i++) {
			Brick temp = shadowBricks.body[i];
			//在游戏区上方,列超出限制
			if (temp.rowIndex < -4)
				return true;
			if (temp.rowIndex < 0) {
				if (temp.colIndex > MAX_COLS-1 || temp.colIndex < 0)
					return true;
			}
			if (temp.rowIndex >= MAX_ROWS)
				return true;
			if (temp.colIndex >  MAX_COLS - 1 || temp.colIndex < 0)
				return true;
		}
		return false;
	}
	//判断是否与现有方块重叠。在游戏区时：整体左侧没有方块被占用
	bool IsSpaceOccupied() {
		for (int i = 0; i < shadowBricks.number; i++) {
			if (shadowBricks.body[i].rowIndex >= 0)
			{
				//只要有一个单元被占据就返回True
				if ((GetCellStatus(shadowBricks.body[i].colIndex, shadowBricks.body[i].rowIndex) == true)
					)//&& (isCellInCurBricks(shadowBricks.body[i].colIndex, shadowBricks.body[i].rowIndex) == false))
					//如果在绘制curBricks时使用DisplayCurBricks方法则需要判断if内的两个条件
					//如果在绘制时使用DrawCurBricks方法时，则不需要判断第二个条件
					return true;
			}
		}
		//只有所有方格没被占据才返回false
		return false;
	}

	//擦除Next区，绘制使所有NextArea区Cell为非激活状态
	void EraseNextArea() {
		SetNextAreaStatus(false);
	}
	//判断一个Cell是否在curBricks内
	bool isCellInCurBricks(int col, int row) {
		for (int i = 0; i < curBricks.number; i++) {
			if (curBricks.body[i].colIndex == col && curBricks.body[i].rowIndex == row)
				return true;
		}
		return false;
	}

public:
	GameControl() {
		Initiate();
	}
	~GameControl();
	void GetHWND(HWND hwnd) {
		graphicsMgr.GetHWND(hwnd);
	}

	//创建一个方块，其信息赋值给NextBricks，但并不显示
	void CreateNextBricks() {
		nextBricks.CreateBricks();
	}
	//在NextArea区绘制NextBricks
	void DrawNextBricks() {
		//EraseNextArea();
		for (int i = 0; i < nextBricks.number; i++)
		{
			Brick *temp = &nextBricks.body[i];
			//这里对于每一行+3，是因为方便在挪至游戏区时不做过多的行索引转换
			if (nextArea[temp->colIndex][temp->rowIndex + 4] != true) {
				nextArea[temp->colIndex][temp->rowIndex + 4] = true;
				graphicsMgr.DrawBrickAtNextArea(temp->colIndex, temp->rowIndex + 3, true);
			}
		}
	}
	//擦除Next区显示的NextBricks方块
	void EraseNextBricks() {
		for (int i = 0; i < nextBricks.number; i++)
		{
			Brick *temp = &nextBricks.body[i];
			//这里对于每一行+3，是因为方便在挪至游戏区时不做过多的行索引转换
			if (nextArea[temp->colIndex][temp->rowIndex + 4] != false) {
				nextArea[temp->colIndex][temp->rowIndex + 4] = false;
				graphicsMgr.DrawBrickAtNextArea(temp->colIndex, temp->rowIndex + 3, false);
			}
		}

	}

	//在游戏区擦除当前Bricks，同时改变Cell状态
	void EraseCurBricks() {
		for (int i = 0; i < curBricks.number; i++) {
			Brick temp = curBricks.body[i];
			if (curBricks.body[i].rowIndex >= 0)
				graphicsMgr.DrawBrick(temp.colIndex, temp.rowIndex, false);
		}
	}

	//在游戏区显示curBricks，同时改变其占据的gameArea内cell的状态
	void DisplayCurBricks()
	{
		for (int i = 0; i < curBricks.number; i++)
		{
			if (curBricks.body[i].rowIndex >= 0)
				SetCellActive(curBricks.body[i].colIndex, curBricks.body[i].rowIndex);
		}
	}
	//在游戏区域仅绘制当前Bricks，但不改变gameArea内cell的状态
	void DrawCurBricks() {
		for (int i = 0; i < curBricks.number; i++)
		{
			Brick temp = curBricks.body[i];
			if (temp.rowIndex >= 0)
				graphicsMgr.DrawBrick(temp.colIndex, temp.rowIndex, true);
		}
	}
	//擦除NextArea（用InActive充填），此方法与ClearNextArea的区别是：前者绘制NextArea里每一个方块，
	//后者仅绘制状态变化的Cell
	void DrawNextArea() {
		//SetNextAreaStatus(true);
		//Sleep(50);
		SetNextAreaStatus(false);
	}
	//根据游戏区方块的状态（gameArea数组表示）绘制。
	void DrawGameArea() {
		for (int colindex = 0; colindex < MAX_COLS; colindex++) {
			for (int rowindex = 0; rowindex < MAX_ROWS; rowindex++) {
				graphicsMgr.DrawBrick(colindex, rowindex, gameArea[colindex][rowindex]);
				//SetCellStatus(colindex, rowindex, gameArea[colindex][rowindex]);
			}
		}
	}
	//绘制游戏区框架
	void DrawGameFrame() {
		graphicsMgr.DrawGameFrame();
	}
	//从游戏区上方到下方先全部激活再全部失活
	void ClearGameAreaRowByRow(PVOID pvoid) {
		int row = 0;
		for (row = 0; row < MAX_ROWS; row++) {
			SetRowStatus(row,true);
			Sleep(20);
		}
		for (row = MAX_ROWS - 1; row >= 0; row--) {
			SetRowStatus(row,false);
			Sleep(20);
		}
	}
	//将NextBricks的数据拷贝并适当转换至curBricks，同时生成下一个NextBricks数据
	void GetCurBricksFromNextBricks() {
		curBricks.Copy(nextBricks);
		//为了方便在NextArea显示，在NextBrick内，列索引从0开始，行索引最大值为0
		//当挪到游戏区上方时，我们希望其一开始显示在靠中央的几列中，且最下方距离游戏区1个方块距离
		//即一开始虽然挪到了游戏区上方，但却不能再游戏区内显示。
		curBricks.MoveRight(MAX_COLS/2-2);
	}
	//尝试向左移动一格，如果可以则移动并且返回true，如果受限不可以移动，则返回false
	bool TryToMoveLeftCurBricks() {
		shadowBricks.Copy(curBricks);
		shadowBricks.MoveLeft();
		if (IsOutOfBorder() || IsSpaceOccupied()) {
			return false;
		}
		EraseCurBricks();
		curBricks.MoveLeft();
		DrawCurBricks();
		return true;
	}
	//尝试向右移动一格，如果可以则移动并且返回true，如果受限不可以移动，则返回false
	bool TryToMoveRightCurBricks() {
		shadowBricks.Copy(curBricks);
		shadowBricks.MoveRight();
		if (IsOutOfBorder() || IsSpaceOccupied()) {
			return false;
		}
		EraseCurBricks();
		curBricks.MoveRight();
		DrawCurBricks();
		return true;
	}
	//尝试向下移动一格，如果可以则移动并且返回true，如果受限不可以移动，则返回false
	bool TryToMoveDownCurBricks() {
		shadowBricks.Copy(curBricks);
		shadowBricks.MoveDown();
		if (IsOutOfBorder() || IsSpaceOccupied()) {
			DisplayCurBricks();//如果之前是DrawCurBricks，则这里需要再调用一次DisplayCurBricks
			for (int i = 0; i < curBricks.number; i++) {
				activeCellCountInRow[curBricks.body[i].rowIndex] += 1;
			}
			return false;
		}
		EraseCurBricks();
		curBricks.MoveDown();
		DrawCurBricks();
		return true;
	}
	//尝试向上移动一个，如果可以则移动并且返回true.此函数在实际过程中不用
	bool TryToMoveUpCurBricks() {
		shadowBricks.Copy(curBricks);
		shadowBricks.MoveUp();
		if (IsOutOfBorder() || IsSpaceOccupied()) {
			return false;
		}
		EraseCurBricks();
		curBricks.MoveUp();
		DrawCurBricks();
		return true;
	}
	//尝试按照既定的参数旋转，如果可以则旋转并且返回true，如果受限不可以旋转，则返回false
	bool TryToRoateCurBricks() {
		shadowBricks.Copy(curBricks);
		shadowBricks.Rotate(isClockWiseRotate);
		if (IsOutOfBorder() || IsSpaceOccupied()) {
			return false;
		}
		EraseCurBricks();
		curBricks.Rotate(isClockWiseRotate);
		DrawCurBricks();
		return true;
	}



	//不能继续向下移动时的后续处理
	void DoAfterCanNotMoveDown() {
		if (lock) 
			//避免该方法没有处理完又进入该方法
			return;
		else{
			lock = true;
			if (curBricks.GetMostUpRowIndex() < 0) {
				curStatus = GS_GameOver;
				lock = false;
				return;
			}
			//消除可能存在的满行,不需要从最低行还是，只需从当前bricks能到达的最大行索引开始
			int row = curBricks.GetMostDownRowIndex();
			for (int i = row; i > row-curBricks.number; i--) {
				if (activeCellCountInRow[i] == MAX_COLS) {
					SetRowInActive(i);
					//如果不是最低一行，则需要移动
					activeCellCountInRow[i] = 0;
				}
			}
			int numtomovedown = 0;
			for (int i = row; i >= 0; i--) {
				if (activeCellCountInRow[i] != 0) {
					MoveDownRow(i, numtomovedown);
				}
				else
				{
					numtomovedown += 1;
				}
			}
			//完成对行的消除之后，需要做的事情
			GetCurBricksFromNextBricks();
			DrawCurBricks();
			EraseNextBricks();
			CreateNextBricks();
			DrawNextBricks();
			lock = false;
		}
	}
	void Running() {
		if (TryToMoveDownCurBricks() == false) {
			DoAfterCanNotMoveDown();
		}
	}
	//void Reset() {
	//	Initiate();
	//	DrawCurBricks();
	//	EraseNextArea();
	//	CreateNextBricks();
	//	DrawNextBricks();
	//	curStatus = GS_Initiated;
	//}

	//准备开始前的绘制
	void PrepareStart() {
		Initiate();
		DrawGameArea();
		EraseNextArea();
		CreateNextBricks();
		DrawNextBricks();
		curStatus = GS_PrepareForStart;
	}
	void Start() {
		GetCurBricksFromNextBricks();
		DrawCurBricks();
		EraseNextArea();
		CreateNextBricks();
		DrawNextBricks();
		curStatus = GS_Running;
	}
	void Pause() 
	{
		curStatus = GS_Paused;
	}

	void Resume() {
		curStatus = GS_Running;
	}
	//提高级别，当提高最高级别时，级别被设定为最低
	void SpeedUp() {
		if (level == 9)
			level = 1;
		else
			level += 1;
	}
	//降低级别，当降低最低级别是，级别被设定为最高
	void SpeedDown() {
		if (level == 0)
			level = 9;
		else
			level -= 1;
	}
};

