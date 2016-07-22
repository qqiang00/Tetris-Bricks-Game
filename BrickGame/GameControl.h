#pragma once
//��Ϸ�����࣬�����װ��GraphicsManager��Ļ��ƹ���
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
	bool gameArea[MAX_COLS][MAX_ROWS];//��¼��Ϸ�������з���״̬��false��ʶδ����ռ��
	bool nextArea[4][4];//��¼��Next�������з���״̬��false��ʶδ����ռ��
	int activeCellCountInRow[MAX_ROWS];//ĳһ���ڼ����cells������,�����ڵ�Ԫ���ڷ��鲻�ܼ����½�ʱ�Ͷ����������ʱ����
	long score;//��ǰ�÷�
	long hiScore;//��߷�
	int rowsErased;//�Ѿ�����������
	int rowsPreset;//Ԥ���趨�Ĵ���������������
	Bricks nextBricks;//��һ��Bricks,��ʾ��Next��
	Bricks curBricks;//��ǰBricks
	Bricks shadowBricks;//Ӱ�ӷ��飬������ʱ�洢��ǰBricks������������ռ�ݵĿռ䣬
						//�����ж����curBricks���ƶ�����ת�����Ƿ����˳������
	double speed;//�����ٶ�

	bool isClockWiseRotate;
	bool lock = false;


public:
	GraphicsManager graphicsMgr;//��ͼ�������
	GameStatus curStatus = GS_GameOver;
	int level;//��ǰ�ȼ�
private:
	//��ʼ��������
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
		//nextBricks��curBricks����Ĭ���޲εĹ��캯��
		nextBricks = Bricks(true);
		curBricks = Bricks(true);
		shadowBricks = Bricks(true);
		level = 1;
		isClockWiseRotate = true;
	}
	//����gameArea�ڵ�Ԫ��״̬�����״̬�ı��ˣ����Ƹõ�Ԫ
	void SetCellStatus(int col, int row, bool isactive) {
		if (gameArea[col][row] != isactive) {
			gameArea[col][row] = isactive;
			graphicsMgr.DrawBrick(col, row, isactive);
		}
	}
	//����Ԫ����Ϊ���ռ�ã�״̬
	void SetCellActive(int col, int row) {
		//���ﲻ����activeCellNumberInRow��������Ӧ�е���ֵ
		SetCellStatus(col, row, true);
	}
	//����Ԫ����Ϊ�ǻ��ռ�ã�״̬
	void SetCellInActive(int col, int row) {
		SetCellStatus(col, row, false);
	}
	//��õ�Ԫ�����伤��״̬
	bool GetCellStatus(int col, int row) {
		if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) {
			return false;
		}
		else {
			return gameArea[col][row];
		}
	}
	//����Next����״̬
	void SetNextAreaStatus(bool status) {
		for (int colindex = 0; colindex < 4; colindex++) {
			for (int rowindex = 0; rowindex < 4; rowindex++)
			{
				nextArea[colindex][rowindex] = status;
				graphicsMgr.DrawBrickAtNextArea(colindex, rowindex, status);
			}
		}
	}
	//������Ϸ������λ�ñ�����ռ�ݵ�״̬���ڷ��鲻�ܽ�һ���½�ʱʹ�á�
	void UpdateGameAreaStatus() {
		for (int i = 0; i < curBricks.number; i++) {
			SetCellActive(curBricks.body[i].colIndex, curBricks.body[i].rowIndex);
		}
	}
	//����ĳһ���е�״̬
	void SetRowStatus(int rowindex, bool status) {
		for (int colindex = 0; colindex < MAX_COLS; colindex++) {
			SetCellStatus(colindex, rowindex, status);
		}
	}
	//����һ����
	void SetRowActive(int rowindex) {
		SetRowStatus(rowindex, true);
		activeCellCountInRow[rowindex] = MAX_COLS;
	}
	//����һ����
	void SetRowInActive(int rowindex) {
		SetRowStatus(rowindex, false);
		Sleep(50);
		SetRowStatus(rowindex, true);
		Sleep(50);
		SetRowStatus(rowindex, false);
		activeCellCountInRow[rowindex] = 0;
	}

	
	//���������״̬��ʾĳһ�С�
	void DisplayRow(int rowindex) {
		for (int colindex = 0; colindex < MAX_COLS; colindex++) {
			SetCellStatus(colindex, rowindex, gameArea[colindex][rowindex]);
		}
	}
	//�����ƶ����У���Ḳ��Ŀ���е����ݣ�������MoveDown֮ǰ�Ȱ�Ŀ���е�����cell��ΪInactive��num>0,�����ƶ�
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
	//���ShadowBricks�ж��Ƿ�Խ��
	//1.����Ϸ���Ϸ�ʱ��������û����ȫ��ʾ����Ϸ��ʱ���������������С��3�����ҵ����������ô���6
	//2.����Ϸ��ʱ�����������������С��0�����ҵ����������ô���9
	bool IsOutOfBorder() {
		for (int i = 0; i < shadowBricks.number; i++) {
			Brick temp = shadowBricks.body[i];
			//����Ϸ���Ϸ�,�г�������
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
	//�ж��Ƿ������з����ص�������Ϸ��ʱ���������û�з��鱻ռ��
	bool IsSpaceOccupied() {
		for (int i = 0; i < shadowBricks.number; i++) {
			if (shadowBricks.body[i].rowIndex >= 0)
			{
				//ֻҪ��һ����Ԫ��ռ�ݾͷ���True
				if ((GetCellStatus(shadowBricks.body[i].colIndex, shadowBricks.body[i].rowIndex) == true)
					)//&& (isCellInCurBricks(shadowBricks.body[i].colIndex, shadowBricks.body[i].rowIndex) == false))
					//����ڻ���curBricksʱʹ��DisplayCurBricks��������Ҫ�ж�if�ڵ���������
					//����ڻ���ʱʹ��DrawCurBricks����ʱ������Ҫ�жϵڶ�������
					return true;
			}
		}
		//ֻ�����з���û��ռ�ݲŷ���false
		return false;
	}

	//����Next��������ʹ����NextArea��CellΪ�Ǽ���״̬
	void EraseNextArea() {
		SetNextAreaStatus(false);
	}
	//�ж�һ��Cell�Ƿ���curBricks��
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

	//����һ�����飬����Ϣ��ֵ��NextBricks����������ʾ
	void CreateNextBricks() {
		nextBricks.CreateBricks();
	}
	//��NextArea������NextBricks
	void DrawNextBricks() {
		//EraseNextArea();
		for (int i = 0; i < nextBricks.number; i++)
		{
			Brick *temp = &nextBricks.body[i];
			//�������ÿһ��+3������Ϊ������Ų����Ϸ��ʱ���������������ת��
			if (nextArea[temp->colIndex][temp->rowIndex + 4] != true) {
				nextArea[temp->colIndex][temp->rowIndex + 4] = true;
				graphicsMgr.DrawBrickAtNextArea(temp->colIndex, temp->rowIndex + 3, true);
			}
		}
	}
	//����Next����ʾ��NextBricks����
	void EraseNextBricks() {
		for (int i = 0; i < nextBricks.number; i++)
		{
			Brick *temp = &nextBricks.body[i];
			//�������ÿһ��+3������Ϊ������Ų����Ϸ��ʱ���������������ת��
			if (nextArea[temp->colIndex][temp->rowIndex + 4] != false) {
				nextArea[temp->colIndex][temp->rowIndex + 4] = false;
				graphicsMgr.DrawBrickAtNextArea(temp->colIndex, temp->rowIndex + 3, false);
			}
		}

	}

	//����Ϸ��������ǰBricks��ͬʱ�ı�Cell״̬
	void EraseCurBricks() {
		for (int i = 0; i < curBricks.number; i++) {
			Brick temp = curBricks.body[i];
			if (curBricks.body[i].rowIndex >= 0)
				graphicsMgr.DrawBrick(temp.colIndex, temp.rowIndex, false);
		}
	}

	//����Ϸ����ʾcurBricks��ͬʱ�ı���ռ�ݵ�gameArea��cell��״̬
	void DisplayCurBricks()
	{
		for (int i = 0; i < curBricks.number; i++)
		{
			if (curBricks.body[i].rowIndex >= 0)
				SetCellActive(curBricks.body[i].colIndex, curBricks.body[i].rowIndex);
		}
	}
	//����Ϸ��������Ƶ�ǰBricks�������ı�gameArea��cell��״̬
	void DrawCurBricks() {
		for (int i = 0; i < curBricks.number; i++)
		{
			Brick temp = curBricks.body[i];
			if (temp.rowIndex >= 0)
				graphicsMgr.DrawBrick(temp.colIndex, temp.rowIndex, true);
		}
	}
	//����NextArea����InActive������˷�����ClearNextArea�������ǣ�ǰ�߻���NextArea��ÿһ�����飬
	//���߽�����״̬�仯��Cell
	void DrawNextArea() {
		//SetNextAreaStatus(true);
		//Sleep(50);
		SetNextAreaStatus(false);
	}
	//������Ϸ�������״̬��gameArea�����ʾ�����ơ�
	void DrawGameArea() {
		for (int colindex = 0; colindex < MAX_COLS; colindex++) {
			for (int rowindex = 0; rowindex < MAX_ROWS; rowindex++) {
				graphicsMgr.DrawBrick(colindex, rowindex, gameArea[colindex][rowindex]);
				//SetCellStatus(colindex, rowindex, gameArea[colindex][rowindex]);
			}
		}
	}
	//������Ϸ�����
	void DrawGameFrame() {
		graphicsMgr.DrawGameFrame();
	}
	//����Ϸ���Ϸ����·���ȫ��������ȫ��ʧ��
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
	//��NextBricks�����ݿ������ʵ�ת����curBricks��ͬʱ������һ��NextBricks����
	void GetCurBricksFromNextBricks() {
		curBricks.Copy(nextBricks);
		//Ϊ�˷�����NextArea��ʾ����NextBrick�ڣ���������0��ʼ�����������ֵΪ0
		//��Ų����Ϸ���Ϸ�ʱ������ϣ����һ��ʼ��ʾ�ڿ�����ļ����У������·�������Ϸ��1���������
		//��һ��ʼ��ȻŲ������Ϸ���Ϸ�����ȴ��������Ϸ������ʾ��
		curBricks.MoveRight(MAX_COLS/2-2);
	}
	//���������ƶ�һ������������ƶ����ҷ���true��������޲������ƶ����򷵻�false
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
	//���������ƶ�һ������������ƶ����ҷ���true��������޲������ƶ����򷵻�false
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
	//���������ƶ�һ������������ƶ����ҷ���true��������޲������ƶ����򷵻�false
	bool TryToMoveDownCurBricks() {
		shadowBricks.Copy(curBricks);
		shadowBricks.MoveDown();
		if (IsOutOfBorder() || IsSpaceOccupied()) {
			DisplayCurBricks();//���֮ǰ��DrawCurBricks����������Ҫ�ٵ���һ��DisplayCurBricks
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
	//���������ƶ�һ��������������ƶ����ҷ���true.�˺�����ʵ�ʹ����в���
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
	//���԰��ռȶ��Ĳ�����ת�������������ת���ҷ���true��������޲�������ת���򷵻�false
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



	//���ܼ��������ƶ�ʱ�ĺ�������
	void DoAfterCanNotMoveDown() {
		if (lock) 
			//����÷���û�д������ֽ���÷���
			return;
		else{
			lock = true;
			if (curBricks.GetMostUpRowIndex() < 0) {
				curStatus = GS_GameOver;
				lock = false;
				return;
			}
			//�������ܴ��ڵ�����,����Ҫ������л��ǣ�ֻ��ӵ�ǰbricks�ܵ���������������ʼ
			int row = curBricks.GetMostDownRowIndex();
			for (int i = row; i > row-curBricks.number; i--) {
				if (activeCellCountInRow[i] == MAX_COLS) {
					SetRowInActive(i);
					//����������һ�У�����Ҫ�ƶ�
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
			//��ɶ��е�����֮����Ҫ��������
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

	//׼����ʼǰ�Ļ���
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
	//��߼��𣬵������߼���ʱ�������趨Ϊ���
	void SpeedUp() {
		if (level == 9)
			level = 1;
		else
			level += 1;
	}
	//���ͼ��𣬵�������ͼ����ǣ������趨Ϊ���
	void SpeedDown() {
		if (level == 0)
			level = 9;
		else
			level -= 1;
	}
};

