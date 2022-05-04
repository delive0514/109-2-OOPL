/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <cstdlib>
#include <time.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <fstream>
bool times = true;
int start,END =0;
int fuck0;
int level;
int MaxLevel;
bool write_load = false; //true是load false是write
void delay()
	{
			time_t start_time, cur_time; // 变量声明
			time(&start_time);
			do {
			 time(&cur_time);
			} while ((cur_time - start_time) < 1);
	}
namespace game_framework {


	
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////
	CGameStateInit::CGameStateInit(CGame* g)
		: CGameState(g)
	{
	}
	CPractice::CPractice(){
		x = 85; //x end = 470
		y = 220;
	}
	int k,i = 0;
	void CPractice::OnMove(int position) {
		x = 85;
		for (int i = 1; i < position; i++) {
			x += 60;
		}
		if (x >= 470) {
			x = 85;
		}
		
	}
	void CPractice::OnJump() {
		if (y == 220) {
			y = 210;
		}
		else
			y = 220;
	}
	void CPractice::OnJump2() {
		if (y == 220) {
			y = 400;
		}
		else
			y = 220;
	}
	void CPractice::LoadBitmap() {
		pic.LoadBitmap(IDB_BITMAP42,RGB(255,255,255));

	}
	void CPractice::OnShow() {
		pic.SetTopLeft(x, y);
		pic.ShowBitmap();
	}
	int CPractice::getX() {
		return this->x;
	}
	void CBouncingBall::SetXY(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void CBouncingBall::SetFloor(int floor) {
		this->floor = floor;
	}
	void CBouncingBall::SetVelocity(int velocity) {
		this->velocity = velocity;
		this->initial_velocity = velocity;
	}

	void CGameStateInit::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
		//
		// 開始載入資料
		//
		for (int i = IDB_BITMAP9; i <= IDB_BITMAP16; i++) {
			title1.AddBitmap(i,RGB(255,255,255));
		}
		title1.SetTopLeft(40, 80);
		about.LoadBitmapA(IDB_BITMAP75);
		x = 450;
		y = 70;
		title1.SetDelayCount(2);
		BG.LoadBitmap(IDB_BITMAP43);
		logo.AddBitmap(IDB_INITSELECTBOX,RGB(0,0,0));
		logo.AddBitmap(IDB_BITMAP44,RGB(0, 0, 0));
		logo.AddBitmap(IDB_BITMAP45,RGB(0, 0, 0));
		logo.SetTopLeft(x, 70);
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\click.mp3");
		CAudio::Instance()->Load(AUDIO_CLICK, "sounds\\dingT1.mp3");
		about.SetIsShow(false);
		//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateInit::OnBeginState()
	{
		if (times) {
			CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\menu.mp3");
			times = false;
		}

		CAudio::Instance()->Play(AUDIO_LAKE, true);
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		const char KEY_ENTER = 13;
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		//const char KEY_ENTER = 0;
		if (nChar == KEY_ENTER) {
			if (y == 70) {
				level = 1;
				CAudio::Instance()->Play(AUDIO_DING);
				MaxLevel = 0;
				GotoGameState(GAME_STATE_STAGE);// 切換至GAME_STATE_RUN
				END = clock();
			}
			if (y == 145) {
				CAudio::Instance()->Play(AUDIO_DING);
				GotoGameState(GAME_STATE_STORE);// 切換至GAME_STATE_RUN
				END = clock();
			}
			else if (y == 220) {
				PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
			}
			if (y == 295) {
				about.SetIsShow(true);
			}
			else about.SetIsShow(false);
		}
		else if (nChar == KEY_DOWN) {
			CAudio::Instance()->Play(AUDIO_DING);
			if (y < 295)
				y += 75;
			logo.SetTopLeft(450, y);
		}
		else if (nChar == KEY_UP) {
			CAudio::Instance()->Play(AUDIO_DING);
			if (y > 105)
				y -= 75;
			logo.SetTopLeft(450, y);
		}
		if (nChar == KEY_ESC && about.IsShow())
			about.SetIsShow(false);
		else if (nChar == KEY_ESC) // Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		//GotoGameState(GAME_STATE_STAGE);		// 切換至GAME_STATE_RUN
	}
	void CGameStateInit::OnMove() {
		title1.OnMove();
		logo.OnMove();
	}
	void CGameStateInit::OnShow()
	{
		//
		// 貼上logo
		//
		//logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
		//title.ShowBitmap();
		BG.ShowBitmap();
		logo.OnShow();
		title1.SetDelayCount(3);
		title1.OnShow();
		//
		// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
		//
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, * fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 200));

		pDC->TextOut(450, 105, "開始新遊戲");
		pDC->TextOut(450, 180, "繼續遊戲");

		//pDC->TextOut(300,205,"Ctrl-F 切換至全螢幕或視窗");
		//if (ENABLE_GAME_PAUSE)
			//pDC->TextOut(300,305,"Ctrl-Q 暫停遊戲");
		pDC->TextOut(450, 255, "離開遊戲");
		pDC->TextOut(450, 330, "About");
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
		if (about.IsShow()) {
			about.OnShow();
		}
		about.SetIsAlive(true);
		about.SetXY(80, 60);
		
		
		
	}
	CGameStateStage::CGameStateStage(CGame* g)
		: CGameState(g)
	{
	}
	void CGameStateStage::OnInit()
	{
		bg.LoadBitmap(IDB_BITMAP46);
		select.LoadBitmap(IDB_BITMAP33,RGB(255,255,255));
		stage11.AddBitmap(IDB_BITMAP32, RGB(255, 255, 255));
		stage11.AddBitmap(IDB_BITMAP40, RGB(255, 255, 255));
		stage12.AddBitmap(IDB_BITMAP34, RGB(255, 255, 255));
		stage12.AddBitmap(IDB_BITMAP47, RGB(255, 255, 255));
		stage13.AddBitmap(IDB_BITMAP35, RGB(255, 255, 255));
		stage13.AddBitmap(IDB_BITMAP48, RGB(255, 255, 255));
		
		
		//stage13.LoadBitmap(IDB_13);
		//stage12.SetTopLeft(205, 300);
		//stage13.SetTopLeft(305, 300);
		x = 80;
		y = 250;

	}

	void CGameStateStage::OnBeginState()
	{
		if (times) {
			times = false;
		}
		CAudio::Instance()->Play(AUDIO_LAKE);
		select.SetTopLeft(x, y);
		stage11.SetTopLeft(85, 240);
		stage12.SetTopLeft(280, 260);
		stage13.SetTopLeft(485, 240);
	}

	void CGameStateStage::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		const char KEY_ENTER = 13;
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char save = 83;
		//const char KEY_ENTER = 0;
		
		if (nChar == KEY_ENTER) {
			if (x == 80) {
				level = 1;
				CAudio::Instance()->Play(AUDIO_DING);
				CAudio::Instance()->Stop(AUDIO_LAKE);
				GotoGameState(GAME_STATE_RUN);// 切換至GAME_STATE_RUN
				END = clock();
			}
			else if (x == 280 && MaxLevel >= 1) {
				level = 2;
				CAudio::Instance()->Play(AUDIO_DING);
				CAudio::Instance()->Stop(AUDIO_LAKE);
				GotoGameState(GAME_STATE_RUN);// 切換至GAME_STATE_RUN
				END = clock();
			}
			else if (x == 480 && MaxLevel >= 2) {
				level = 3;
				CAudio::Instance()->Play(AUDIO_DING);
				CAudio::Instance()->Stop(AUDIO_LAKE);
				GotoGameState(GAME_STATE_RUN);// 切換至GAME_STATE_RUN
				END = clock();
			}
		}
		else if (nChar == KEY_LEFT) {
			if (x > 80)
				x -= 200;
			select.SetTopLeft(x, y);
		}
		else if (nChar == KEY_RIGHT) {
			if (MaxLevel >= 3 && 480 <=x) {
				GotoGameState(GAME_STATE_STAGE2);
			}
			else if (x < 480) {
				x += 200;
				select.SetTopLeft(x, y);
			}
		}
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			GotoGameState(GAME_STATE_INIT);// 切換至GAME_STATE_RUN
		else if (nChar == save) {
			write_load = true;
			GotoGameState(GAME_STATE_STORE);
		}
	}

	void CGameStateStage::OnLButtonDown(UINT nFlags, CPoint point)
	{
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
	void CGameStateStage::OnMove() {
		if (x == 80) {
			stage11.SetDelayCount(10);
			stage11.OnMove();
			stage12.Reset();
		}
		else if (x == 280) {
			stage12.SetDelayCount(10);
			stage12.OnMove();
			stage11.Reset();
			stage13.Reset();
		}
		else if (x == 480) {
			stage13.SetDelayCount(10);
			stage13.OnMove();
			stage12.Reset();
		}
	}
	void CGameStateStage::OnShow()
	{
		bg.ShowBitmap();
		string stage_state[3];
		if (MaxLevel > 3) {
			for (i = 0; i < 3; i++) {
				stage_state[i] = "完成";
			}
		}
		else {
			for (i = 0; i < MaxLevel; i++) {
				stage_state[i] = "完成";
			}
			if (MaxLevel != 3) {
				stage_state[MaxLevel] = "未完成";
				for (i = MaxLevel + 1; i < 3; i++) {
					stage_state[i] = "未解鎖";
				}
			}
		}
		//stage11.ShowBitmap();
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		//stage11.ShowBitmap();
		CFont f, * fpq;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fpq = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(TRANSPARENT);

		pDC->SetTextColor(RGB(0, 255, 255));
		//stage11.SetTopLeft(105, 300);
		//stage12.SetTopLeft(205, 300);
		//stage13.SetTopLeft(305, 300);
		pDC->TextOut(0, 0, "ESC返回開始畫面");
		pDC->TextOut(85, 200, "1-1 東方電舞曲");
		pDC->TextOut(285, 200, "1-2 親密");
		pDC->TextOut(455, 200, "1-3 東方不眠夜");
		pDC->TextOut(105, 350, stage_state[0].c_str());
		pDC->TextOut(305, 350, stage_state[1].c_str());
		pDC->TextOut(485, 350, stage_state[2].c_str());
		//pDC->TextOut(450, 205, "繼續遊戲");

		//pDC->TextOut(300,205,"Ctrl-F 切換至全螢幕或視窗");
		//if (ENABLE_GAME_PAUSE)
			//pDC->TextOut(300,305,"Ctrl-Q 暫停遊戲");
		//pDC->TextOut(450, 305, "離開遊戲");
		pDC->SelectObject(fpq);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

		//上面是文字區 絕對不能把放圖的工作放到上面
		
		stage11.OnShow();
		stage12.OnShow();
		stage13.OnShow();
		select.ShowBitmap();
	}
	CGameStateStage2::CGameStateStage2(CGame* g)
		: CGameState(g)
	{
	}
	void CGameStateStage2::OnInit()
	{
		
		bg.LoadBitmap(IDB_BITMAP46);
		select.LoadBitmap(IDB_BITMAP33, RGB(255, 255, 255));
		stage11.LoadBitmap(IDB_BITMAP72, RGB(255, 255, 255));
		stage12.LoadBitmap(IDB_BITMAP73, RGB(255, 255, 255));
		stage13.LoadBitmap(IDB_BITMAP74, RGB(255, 255, 255));
		

		//stage13.LoadBitmap(IDB_13);
		//stage12.SetTopLeft(205, 300);
		//stage13.SetTopLeft(305, 300);
		x = 80;
		y = 250;

	}

	void CGameStateStage2::OnBeginState()
	{
		CAudio::Instance()->Play(AUDIO_LAKE);
		if (times) {
			times = false;
		}

		select.SetTopLeft(x, y);
		stage11.SetTopLeft(90, 250);
		stage12.SetTopLeft(285, 240);
		stage13.SetTopLeft(475, 240);
	}

	void CGameStateStage2::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		const char KEY_ENTER = 13;
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char save = 83;
		//const char KEY_ENTER = 0;

		if (nChar == KEY_ENTER) {
			if (x == 80 && MaxLevel >= 3) {
				level = 4;
				CAudio::Instance()->Play(AUDIO_DING);
				CAudio::Instance()->Stop(AUDIO_LAKE);
				GotoGameState(GAME_STATE_RUN);// 切換至GAME_STATE_RUN
				END = clock();
			}
			else if (x == 280 && MaxLevel >= 4) {
				level = 5;
				CAudio::Instance()->Play(AUDIO_DING);
				CAudio::Instance()->Stop(AUDIO_LAKE);
				GotoGameState(GAME_STATE_RUN);// 切換至GAME_STATE_RUN
				END = clock();
			}
			else if (x == 480 && MaxLevel >= 5) {
				level = 6;
				CAudio::Instance()->Play(AUDIO_DING);
				CAudio::Instance()->Stop(AUDIO_LAKE);
				GotoGameState(GAME_STATE_RUN);// 切換至GAME_STATE_RUN
				END = clock();
			}
		}
		else if (nChar == KEY_LEFT) {
			if (x <= 80) {
				GotoGameState(GAME_STATE_STAGE);
			}
			else if (x > 80) {
				x -= 200;
				select.SetTopLeft(x, y);
			}
		}
		else if (nChar == KEY_RIGHT) {
			if (x < 480)
				x += 200;
			select.SetTopLeft(x, y);
		}
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			GotoGameState(GAME_STATE_INIT);// 切換至GAME_STATE_RUN
		else if (nChar == save) {
			write_load = true;
			GotoGameState(GAME_STATE_STORE);
		}
	}

	void CGameStateStage2::OnLButtonDown(UINT nFlags, CPoint point)
	{
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
	void CGameStateStage2::OnMove() {
		if (x == 80) {
			
		}
		else if (x == 280) {
			
		}
		else if (x == 480) {
			
		}
	}
	void CGameStateStage2::OnShow()
	{
		bg.ShowBitmap();
		string stage_state[3];
		int M2 = MaxLevel - 3;
		for (i = 0; i < M2; i++) {
			stage_state[i] = "完成";
		}
		if (M2 != 3) {
			stage_state[M2] = "未完成";
			for (i = M2 + 1; i < 3; i++) {
				stage_state[i] = "未解鎖";
			}
		}
		//stage11.ShowBitmap();
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		//stage11.ShowBitmap();
		CFont f, * fpq;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fpq = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(TRANSPARENT);

		pDC->SetTextColor(RGB(0, 255, 255));
		//stage11.SetTopLeft(105, 300);
		//stage12.SetTopLeft(205, 300);
		//stage13.SetTopLeft(305, 300);
		pDC->TextOut(0, 0, "ESC返回開始畫面");
		pDC->TextOut(85, 200, "2-1");
		pDC->TextOut(285, 200, "2-2");
		pDC->TextOut(455, 200, "2-3");
		pDC->TextOut(105, 350, stage_state[0].c_str());
		pDC->TextOut(305, 350, stage_state[1].c_str());
		pDC->TextOut(485, 350, stage_state[2].c_str());
		//pDC->TextOut(450, 205, "繼續遊戲");

		//pDC->TextOut(300,205,"Ctrl-F 切換至全螢幕或視窗");
		//if (ENABLE_GAME_PAUSE)
			//pDC->TextOut(300,305,"Ctrl-Q 暫停遊戲");
		//pDC->TextOut(450, 305, "離開遊戲");
		pDC->SelectObject(fpq);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

		//上面是文字區 絕對不能把放圖的工作放到上面

		stage11.ShowBitmap();
		stage12.ShowBitmap();
		stage13.ShowBitmap();
		select.ShowBitmap();
	}
	CGameStateStore::CGameStateStore(CGame* g)
		: CGameState(g)
	{
	}
	void CGameStateStore::OnInit()
	{
		bg.LoadBitmap(IDB_BITMAP43);
		store1.LoadBitmap(IDB_BITMAP36, RGB(255, 0, 255));
		store2.LoadBitmap(IDB_BITMAP37, RGB(255, 0, 255));
		store3.LoadBitmap(IDB_BITMAP38, RGB(255, 10, 255));
		select.LoadBitmap(IDB_BITMAP41, RGB(255,255,255));
		//stage13.LoadBitmap(IDB_13);
		//stage12.SetTopLeft(205, 300);
		//stage13.SetTopLeft(305, 300);
		x = 85;
		y = 300;

	}

	void CGameStateStore::OnBeginState()
	{
		if (times) {
			times = false;
		}
		select.SetTopLeft(x, y);
		store1.SetTopLeft(0, 100);
		store2.SetTopLeft(215, 100);
		store3.SetTopLeft(430, 100);
	}

	void CGameStateStore::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		const char KEY_ENTER = 13;
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		//const char KEY_ENTER = 0;
		if (nChar == KEY_ENTER) {
			if (!write_load) {
				if (x == 85) {
					ifstream ofs("store1.txt");
					string l;
					getline(ofs, l);
					MaxLevel = atoi(l.c_str());
					ofs.close();
					GotoGameState(GAME_STATE_STAGE);// 切換至GAME_STATE_RUN
				}
				else if (x == 285) {
					ifstream ofs("store2.txt");
					string l;
					getline(ofs, l);
					MaxLevel = atoi(l.c_str());
					ofs.close();
					GotoGameState(GAME_STATE_STAGE);// 切換至GAME_STATE_RUN
				}
				else {
					ifstream ofs("store3.txt");
					string l;
					getline(ofs, l);
					MaxLevel = atoi(l.c_str());
					ofs.close();
					GotoGameState(GAME_STATE_STAGE);// 切換至GAME_STATE_RUN
				}
			}
			else {
				//write
				if (x == 85) {
					ofstream ofs;
					ofs.open("store1.txt");
					ofs.ios_base::trunc;
					ofs << MaxLevel << endl;
					ofs.close();
					GotoGameState(GAME_STATE_STAGE);// 切換至GAME_STATE_RUN
				}
				else if (x == 285) {
					ofstream ofs;
					ofs.open("store2.txt");
					ofs.ios_base::trunc;
					ofs << MaxLevel << endl;
					ofs.close();
					GotoGameState(GAME_STATE_STAGE);// 切換至GAME_STATE_RUN
				}
				else {
					ofstream ofs;
					ofs.open("store3.txt");
					ofs.ios_base::trunc;
					ofs << MaxLevel << endl;
					ofs.close();
					GotoGameState(GAME_STATE_STAGE);// 切換至GAME_STATE_RUN
				}
			}
		}
		else if (nChar == KEY_LEFT) {
			if (x > 85)
				x -= 200;
			select.SetTopLeft(x, y);
		}
		else if (nChar == KEY_RIGHT) {
			if (x < 485)
				x += 200;
			select.SetTopLeft(x, y);
		}
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			GotoGameState(GAME_STATE_INIT);// 切換至GAME_STATE_RUN
	
	}

	void CGameStateStore::OnLButtonDown(UINT nFlags, CPoint point)
	{
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
	void CGameStateStore::OnMove() {

	}
	void CGameStateStore::OnShow()
	{
		//stage11.ShowBitmap();
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		//stage11.ShowBitmap();
		CFont f, * fpq;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fpq = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 200));
		//stage11.SetTopLeft(105, 300);
		//stage12.SetTopLeft(205, 300);
		//stage13.SetTopLeft(305, 300);
		pDC->TextOut(0, 0, "ESC返回開始畫面");
		//pDC->TextOut(450, 205, "繼續遊戲");

		//pDC->TextOut(300,205,"Ctrl-F 切換至全螢幕或視窗");
		//if (ENABLE_GAME_PAUSE)
			//pDC->TextOut(300,305,"Ctrl-Q 暫停遊戲");
		//pDC->TextOut(450, 305, "離開遊戲");
		pDC->SelectObject(fpq);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

		//上面是文字區 絕對不能把放圖的工作放到上面
		bg.ShowBitmap();
		select.ShowBitmap();
		store1.ShowBitmap();
		store2.ShowBitmap();
		store3.ShowBitmap();
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame* g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_STAGE);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
	}

	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
		//
		// 開始載入資料
		//
		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 最終進度為100%
		//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, * fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "廢物! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);					// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}
	CGameStatePass::CGameStatePass(CGame* g)
		: CGameState(g)
	{
		
	}

	void CGameStatePass::OnMove()
	{
		counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_STAGE);
	}

	void CGameStatePass::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
		if (MaxLevel <= level) {
			MaxLevel = level;
		}
	}

	void CGameStatePass::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
		//
		// 開始載入資料
		//
		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 最終進度為100%
		//
		ShowInitProgress(100);
	}

	void CGameStatePass::OnShow()
	{
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, * fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];
		// Demo 數字對字串的轉換
		if(fuck0 == 100)
			sprintf(str, "你的評級為S ");
		else if(fuck0 >90)
			sprintf(str, "你的評級為A ");
		else if(fuck0 >70)
			sprintf(str, "你的評級為B ");
		else if(fuck0 >50)
			sprintf(str, "你的評級為C ");
		else
			sprintf(str, "你好爛 " );
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);					// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame* g)
		: CGameState(g)
	{
		
	}

	CGameStateRun::~CGameStateRun()
	{
		
	}

	void CGameStateRun::OnBeginState()
	{
		LoadKeyboardLayout("0x0409", KLF_ACTIVATE | KLF_SETFORPROCESS);
		const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
		const int HITS_LEFT = 5;
		int CLOCK = start;
		const int HITS_LEFT_X = 590;
		const int HITS_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;
		beat_x = 0;
		clap.clear();
		first.clear();
		everytime.clear();
		location.clear();
		jump_time_list.clear();
		noise.SetXY(0, 0);
		noise.SetIsAlive(true);
		noise.SetIsShow(false);
		test1.SetXY(425, 0);
		test1.SetIsAlive(true);
		test1.SetIsShow(false);
		//hand2.SetXY(SIZE_X - hand1.Width() - 20, SIZE_Y - hand1.Height());
		
		bg1.SetDelayCount(3);
		hand.SetDelayCount(1);
		background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
		//help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
		clocktime.SetInteger(CLOCK);
		clocktime.SetTopLeft(0, 400);
		hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
		hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
		//CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
		char cmp3 = level + '0';
		string smp3 = "sounds\\";
		smp3.append(1, cmp3);
		smp3 += ".mp3";
		char mp3[20];
		strcpy(mp3, smp3.c_str());
		//CAudio::Instance()->Load(AUDIO_NTUT, mp3);
		CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
		
		if (level == 1) {
			CAudio::Instance()->Play(AUDIO_ONE, false);			// 撥放 MIDI
			stage1 data;
			for (int i = 0; i < data.total_tap; i++) {
				clap.push_back(data.clap[i]-250);
				first.push_back(data.first[i]-250);
			}
			total_tap = data.total_tap;
			for (int i = 0; i < total_tap; i++) {
				for (int j = 1; j <= 7; j++) {
					location.push_back(j);
					jump_time_list.push_back(1);
				}
			}
			for (int i = 0; i < total_tap; i++)
			{
				int interval = (clap[i] - first[i]) / 6;
				int s = first[i];
				for (int j = 0; j < 6; j++)
				{
					everytime.push_back(s);
					s += interval;
				}
				everytime.push_back(clap[i]);
			}
			everytime.push_back(999999999);
		}
		else if (level == 2) {
			CAudio::Instance()->Play(AUDIO_TWO, false);			// 撥放 MIDI
			stage2 data;
			for (int i = 0; i < data.total_tap; i++) {
				clap.push_back(data.clap[i]-100);
				first.push_back(data.first[i]-100);
			}
			total_tap = data.total_tap;
			for (int i = 0; i < total_tap; i++) {
				for (int j = 1; j <= 7; j++) {
					location.push_back(j);
					jump_time_list.push_back(1);
				}
			}
			for (int i = 0; i < total_tap; i++)
			{
				int interval = (clap[i] - first[i]) / 6;
				int s = first[i];
				for (int j = 0; j < 6; j++)
				{
					everytime.push_back(s);
					s += interval;
				}
				everytime.push_back(clap[i]);
			}
			everytime.push_back(999999999);
		}
		else if (level == 3) {
			CAudio::Instance()->Play(AUDIO_THREE, false);			// 撥放 MIDI
			stage3 data;
			for (int i = 0; i < data.total_tap; i++) {
				clap.push_back(data.clap[i]-100);
				first.push_back(data.first[i]-100);
			}
			total_tap = data.total_tap;
			for (int i = 0; i < total_tap; i++) {
				if (i == 3) {
					for (int j = 1; j <= 7; j++) {
						location.push_back(j);
						if (j == 3) 
							jump_time_list.push_back(12);
						else if(j == 7)
							jump_time_list.push_back(4);
						else 
							jump_time_list.push_back(1);
					}
				}
				else if (i == 4) {
					for (int j = 1; j <= 7; j++) {
						location.push_back(j);
						if (j == 6)
							jump_time_list.push_back(12);
						else if (j == 7)
							jump_time_list.push_back(16);
						else
							jump_time_list.push_back(1);
					}
				}
				else if (i == 6) {
					int lo[] = { 1,2,3,4,5,4,5,7 };
					for (int j = 0; j <8; j++) {
						location.push_back(lo[j]);
						jump_time_list.push_back(1);
					}
				}
				else if (i == 7) {
					int lo[] = { 1,2,3,5,7 };
					for (int j = 0; j <5; j++) {
						location.push_back(lo[j]);
						if(3== lo[j])
							jump_time_list.push_back(8);
						else if(5 == lo[j])
							jump_time_list.push_back(8);
						else if(7 == lo[j])
							jump_time_list.push_back(16);
						else
							jump_time_list.push_back(1);
					}
				}
				else if (i == 11) {
					for (int j = 1; j <= 7; j++) {
						location.push_back(j);
						if (j == 3)
							jump_time_list.push_back(12);
						else if (j == 6)
							jump_time_list.push_back(8);
						else
							jump_time_list.push_back(1);
					}
				}
				else if (i == 12) {
					int lo[] = { 1,2,3,2,4,5,6,7 };
					for (int j = 0; j <8; j++) {
						location.push_back(lo[j]);
						if (j == 6)
							jump_time_list.push_back(8);
						else if (j == 7)
							jump_time_list.push_back(8);
						else
							jump_time_list.push_back(1);
					}
				}
				else if (i == 13) {
					int lo[] = { 1,3,1,2,4,2,5,6,7 };
					for (int j = 0; j < 9; j++) {
						location.push_back(lo[j]);
						if (j == 8)
							jump_time_list.push_back(8);
						else
							jump_time_list.push_back(1);
					}
				}
				else if (i == 14) {
					int lo[] = { 1,3,1,2,3,2,4,6,7 };
					for (int j = 0; j < 9; j++) {
						location.push_back(lo[j]);
							jump_time_list.push_back(1);
					}
				}
				else if (i == 15) {
					int lo[] = { 1,2,3,2,1,4,5,6,7 };
					for (int j = 0; j < 9; j++) {
						location.push_back(lo[j]);
						jump_time_list.push_back(1);
					}
				}
				else if (i == 19) {
					int lo[] = { 1,2,3,4,5,3,6,7 };
					for (int j = 0; j < 8; j++) {
						location.push_back(lo[j]);
						if( j == 2 || j == 5)
							jump_time_list.push_back(8);
						else
							jump_time_list.push_back(1);	
					}
				}
				else if (i == 23) {
					int lo[] = { 1,5,7 };
					for (int j = 0; j < 3; j++) {
						location.push_back(lo[j]);
						if (j ==0)
							jump_time_list.push_back(24);
						else if ( j ==1)
							jump_time_list.push_back(8);
						else
							jump_time_list.push_back(1);
					}
				}
				else if (i == 26) {
					int lo[] = { 1,2,3,1,4,5,6,7 };
					for (int j = 0; j < 8; j++) {
						location.push_back(lo[j]);
						if (j == 1)
							jump_time_list.push_back(8);
						else
							jump_time_list.push_back(1);
					}
				}
				else if (i == 27 || i == 28) {
					int lo[] = { 1,2,3,1,4,1,6,7 };
					for (int j = 0; j < 8; j++) {
						location.push_back(lo[j]);
						if (j == 7)
							jump_time_list.push_back(8);
						else
							jump_time_list.push_back(1);
					}
				}
				else if (i == 31) {
					int lo[] = { 1,2,3,1,4,1,6,7 };
					for (int j = 0; j < 8; j++) {
						location.push_back(lo[j]);
						jump_time_list.push_back(1);
					}
				}
				else if (i == 32) {
					int lo[] = { 1,2,1,1,4,5,6,7 };
					for (int j = 0; j < 8; j++) {
						location.push_back(lo[j]);
						jump_time_list.push_back(1);
					}
				}
				else if (i == 33) {
					int lo[] = { 1,5,4,5,6,7 };
					for (int j = 0; j < 6; j++) {
						if (j == 0) {
							jump_time_list.push_back(24);
						}
						else {
							jump_time_list.push_back(1);
						}
						location.push_back(lo[j]);
					}
				}
				else if (i == 34) {
					int lo[] = { 1,1,2,3,4,1,5,6,7 };
					for (int j = 0; j < 9; j++) {
						location.push_back(lo[j]);
						jump_time_list.push_back(1);
					}
				}
				else {
					for (int j = 1; j <= 7; j++) {
						if (i == 5 && j == 6) {
							location.push_back(5);
							jump_time_list.push_back(1);
						}
						else if (i == 9 && j == 7) {
							location.push_back(j);
							jump_time_list.push_back(12);
						}
						else if (i == 10 && j == 7) {
							location.push_back(j);
							jump_time_list.push_back(8);
						}
						else if (i == 17 && j == 7) {
							location.push_back(j);
							jump_time_list.push_back(20);
						}
						else if (i == 24 && j == 7) {
							location.push_back(j);
							jump_time_list.push_back(8);
						}
						else if (i == 25 && j == 3) {
							location.push_back(j);
							jump_time_list.push_back(12);
						}
						else if (i == 30 && j == 5) {
							location.push_back(1);
							jump_time_list.push_back(1);
						}
						else {
							location.push_back(j);
							jump_time_list.push_back(1);
						}
					}
				}
			}
			for (int i = 0; i < total_tap; i++)
			{
				int len = 6;
				if (i == 6 || i ==12 || i == 19 || i ==26 || i == 27 || i == 28 || i ==31 || i ==32) {
					len = 7;
				}
				else if (i == 13 || i == 14 || i == 15 || i == 34) {
					len = 8;
				}
				else if (i == 23) {
					len = 2;
				}
				else if (i == 33) {
					len = 5;
				}
				else if (i == 7) {
					len = 4;
				}
				int interval = (clap[i] - first[i]) / len;
				int s = first[i];
				for (int j = 0; j < len; j++)
				{
					everytime.push_back(s);
					s += interval;
				}
				everytime.push_back(clap[i]);
			}
			everytime.push_back(999999999);
		}
		else if (level == 4) {
			CAudio::Instance()->Play(AUDIO_FOUR, false);			// 撥放 MIDI
			stage4 data;
			for (int i = 0; i < data.total_tap; i++) {
				clap.push_back(data.clap[i]);
				first.push_back(data.first[i]);
			}
			total_tap = data.total_tap;
			for (int i = 0; i < total_tap; i++) {
				for (int j = 1; j <= 30; j++) {
					if (j < 21)
						location.push_back(1);
					else if (j < 26)
						location.push_back(2);
					else
						location.push_back(j-25+2);
					if (j == 1)
						jump_time_list.push_back(2);
					else
						jump_time_list.push_back(1);
				}
			}
			for (int i = 0; i < total_tap; i++)
			{
				int interval = (clap[i] - first[i]) / 29;
				int s = first[i];
				for (int j = 0; j < 29; j++)
				{
					everytime.push_back(s);
					s += interval;
				}
				everytime.push_back(clap[i]);
			}
			everytime.push_back(999999999);
		}
		else if (level == 5) {
			CAudio::Instance()->Play(AUDIO_FIVE, false);			// 撥放 MIDI
			stage5 data;
			for (int i = 0; i < data.total_tap; i++) {
				clap.push_back(data.clap[i]-100);
				first.push_back(data.first[i]-100);
			}
			total_tap = data.total_tap;
			for (int i = 0; i < total_tap; i++) {
				if (i < 21) {
					for (int j = 1; j <= 30; j++) {
						if (j < 21)
							location.push_back(1);
						else if (j < 26)
							location.push_back(2);
						else
							location.push_back(j - 25 + 2);
						if (j == 1)
							jump_time_list.push_back(2);
						else
							jump_time_list.push_back(1);
					}
				}
				else{
					for (int j = 1; j <= 10; j++) {
						if (j < 5)
							location.push_back(1);
						else if (j < 6)
							location.push_back(2);
						else
							location.push_back(j - 5 + 2);
						if (j == 1)
							jump_time_list.push_back(2);
						else
							jump_time_list.push_back(1);
					}
				}
			}
			for (int i = 0; i < total_tap; i++)
			{
				if (i < 21) {
					int interval = (clap[i] - first[i]) / 29;
					int s = first[i];
					for (int j = 0; j < 29; j++)
					{
						everytime.push_back(s);
						s += interval;
					}
					everytime.push_back(clap[i]);
				}
				else {
					int interval = (clap[i] - first[i]) / 9;
					int s = first[i];
					for (int j = 0; j < 9; j++)
					{
						everytime.push_back(s);
						s += interval;
					}
					everytime.push_back(clap[i]);
				}
			}
			everytime.push_back(999999999);
		}
		else if (level == 6) {
			CAudio::Instance()->Play(AUDIO_SIX, false);			// 撥放 MIDI
			stage6 data;
			for (int i = 0; i < data.total_tap; i++) {
				clap.push_back(data.clap[i]-100);
				first.push_back(data.first[i]-100);
			}
			total_tap = data.total_tap;
			for (int i = 0; i < total_tap; i++) {
				for (int j = 1; j <= 20; j++) {
					if (j < 11)
						location.push_back(1);
					else if (j < 16)
						location.push_back(2);
					else
						location.push_back(j - 15 + 2);
					if (j == 1) 
						jump_time_list.push_back(2);
					else
						jump_time_list.push_back(1);
				}
			}
			for (int i = 0; i < total_tap; i++)
			{
				int interval = (clap[i] - first[i]) / 19;
				int s = first[i];
				for (int j = 0; j < 19; j++)
				{
					everytime.push_back(s);
					s += interval;
				}
				everytime.push_back(clap[i]);
			}
			everytime.push_back(999999999);
		}
		
	}
	ofstream ofs;
	int tt = 210;
	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		//
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		// 移動背景圖的座標
		
		if(level==1)background1.OnMove();
		if (level == 3 && c>=30 && c<31 && start<clap[31] ) {
			noise.SetIsShow(true);
		}
		if (level == 3) {
			bg1.OnMove();
			if (tt == 210 || start < clap[c])
				tt = 220;
			else
				tt = 210;
		}
		if (level == 5) {
			
			bg3.OnMove();
			
			if (tt == 210 || start<clap[c])
				tt = 220;
			else
				tt = 210;
			
			//if (c > 74 && c < 109) {
				//c_practice.OnJump2();
			//}
			
		}
		if (level == 6) {

			if (tt == 210 || start < clap[c])
				tt = 220;
			else
				tt = 210;
			if (c > 74 && c < 109) {
				c_practice.OnJump2();
			}

		}
			
		if (level == 6)bg4.OnMove();

		start = (clock()-END)-600;
		//bar.OnMove();
		//clocktime.SetInteger(start);
		int const min = 20;
		int const max = 480;
		int const minx = 0;
		int const maxx = 300;
		/*if (start/40 == clap[c]/40) {
			c++;
			int x = rand() % (maxx - min + 1) + min;
			int y = rand() % (max + 1) + 0;
			
			background.SetTopLeft(x,y);
		}
		if (start / 100 > (clap[43] / 100 )+50) {
			c++;
		}*/
		
		
		/* 產生 [min , max] 的整數亂數 */
		
		
		
		if (start > everytime[beat_x]-100) {
			c_practice.OnMove(location[beat_x]);
			jump_time = jump_time_list[beat_x]*2;
			beat_x++;
		}
		if (jump_time) {
			c_practice.OnJump();
			jump_time--;
		}
		/*
		if (c_practice.getX() == 450 && test1.IsAlive()) {
			CAudio::Instance()->Play(AUDIO_DING);
			hits_left.Add(-1);
			if (hits_left.GetInteger() <= 0) {
				CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
				GotoGameState(GAME_STATE_OVER);
			}
		}
		*/
		//bball.OnMove();
		//gamemap.OnMove();
	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
		//
		// 開始載入資料
		//
		noise.LoadBitmapA(IDB_BITMAP61);
		isClick = false;
		test1.LoadBitmap(177);
		bg.LoadBitmap(IDB_BITMAP49);
		for (int i = IDB_BITMAP62; i <= IDB_BITMAP71; i++) {
			bg1.AddBitmap(i);
		}
		bg2.LoadBitmap(IDB_BITMAP52);
		bg3.AddBitmap(IDB_BITMAP52);
		bg3.AddBitmap(IDB_BITMAP53);
		for (int i = IDB_BITMAP54; i <= IDB_BITMAP59; i++) {
			bg4.AddBitmap(i);
		}
		bg3.SetDelayCount(2);
		bg4.SetDelayCount(2);
		background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
		background1.AddBitmap(IDB_BG1);
		background1.AddBitmap(IDB_BG2);
		background1.SetDelayCount(7);
		// 完成部分Loading動作，提高進度
		//
		ShowInitProgress(50);
		//Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 繼續載入其他資料
		//
		
		bar.LoadBitmap(IDB_BITMAP17,RGB(255, 255, 255));
		
		
		hand.AddBitmap(IDB_HAND1,RGB(255, 255, 255));
		hand.AddBitmap(IDB_HAND2, RGB(255, 255, 255));
		test.LoadBitmap(IDB_INITSELECTBOX);
		xx1.LoadBitmap(IDB_BITMAP30,RGB(0,0,0));
		xx2.LoadBitmap(IDB_BITMAP30, RGB(0, 0, 0));
		help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
		corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
		//corner.ShowBitmap(background);							// 將corner貼到background
		//bball.LoadBitmap();										// 載入圖形
		hits_left.LoadBitmap();
		//clocktime.LoadBitmap();
		c_practice.LoadBitmap();
		//gamemap.LoadBitmap();
		//CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding1.mp3");	// 載入編號0的聲音ding.wav
		//CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
		//
		//CAudio::Instance()->Load(AUDIO_NTUT,mp3);
		//CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\2.mp3");
		
		// 載入編號2的聲音ntut.mid
		CAudio::Instance()->Load(AUDIO_ONE, "sounds\\1.mp3");
		CAudio::Instance()->Load(AUDIO_TWO, "sounds\\2.mp3");
		CAudio::Instance()->Load(AUDIO_THREE, "sounds\\3.mp3");
		CAudio::Instance()->Load(AUDIO_FOUR, "sounds\\4.mp3");
		CAudio::Instance()->Load(AUDIO_FIVE, "sounds\\5.mp3");
		CAudio::Instance()->Load(AUDIO_SIX, "sounds\\6.mp3");
		//
		// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE) {
			hand.OnMove();
			
			//test1.SetIsShow(!test1.IsShow());
			//test1.OnShow();
			//test1.SetIsShow(true); 2021/04/08
		}
	}
	bool H = false;
	vector <int> fuck;
	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_SPACE = ' ';
		const char save = 83; //SKIP
		if (nChar == KEY_SPACE) {
			//test1.SetIsShow(!test1.
			isClick = true;
			
			if (start - 300 < clap[c] && clap[c] < start + 300) {
				isGet = true;
			}
			else {
				isGet = false;
			}
			test1.SetIsShow(true);
			//noise.SetIsShow(true);
			
			hand.OnMove();
			hand.Reset();
			
			fuck.push_back(start);
			
			//hand2.SetIsShow(true);
			
			//test1.OnShow();
			//test1.SetIsAlive(!test1.IsAlive());
			/*if(c_practice.getX()>=400)
				hits_left.Add(+1);
			else
				hits_left.Add(-1);*/
			//
			// 若剩餘碰撞次數為0，則跳到Game Over狀態
			//

			
			int x = rand() % (400 - 20 + 1) + 20;
			int y = rand() % (480 + 1) + 0;
			background.SetTopLeft(x, y);
		}
		else if (nChar == save) {
			fuck0 = 100;
			if (level == 1) {
				CAudio::Instance()->Stop(AUDIO_ONE);	// 停止 MIDI
			}
			else if (level == 2) {
				CAudio::Instance()->Stop(AUDIO_TWO);	// 停止 MIDI
			}
			else if (level == 3) {
				CAudio::Instance()->Stop(AUDIO_THREE);	// 停止 MIDI
			}
			else if (level == 4) {
				CAudio::Instance()->Stop(AUDIO_FOUR);	// 停止 MIDI
			}
			else if (level == 5) {
				CAudio::Instance()->Stop(AUDIO_FIVE);	// 停止 MIDI
			}
			else if (level == 6) {
				CAudio::Instance()->Stop(AUDIO_SIX);	// 停止 MIDI
			}
			c = 0;
			GotoGameState(GAME_STATE_PASS);
		}
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnShow()
	{
		//
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//
		//
		//  貼上背景圖、撞擊數、球、擦子、彈跳的球
		//
		//gamemap.OnShow();
		
		if (level == 1) {
			background1.OnShow();
		}
		else if(level ==2) {
			bg.ShowBitmap();
		}
		else if(level == 3){
			bg1.OnShow();
		}
		else if (level == 4) {
			bg2.ShowBitmap();
		}
		else if (level == 5) {
			bg3.OnShow();
		}
		else if (level == 6) {
			bg4.OnShow();
		}
		if (level != 5)bar.SetTopLeft(0, 210);
		else if(level ==5 || level==6) bar.SetTopLeft(0, tt);
					// 貼上背景圖
		//help.ShowBitmap();					// 貼上說明圖
		hits_left.ShowBitmap();
		//clocktime.ShowBitmap();
		
		bar.ShowBitmap();
		
		if (test1.IsShow()) {
			test1.OnShow();
			//test1.SetIsAlive(!test1.IsAlive());
		}
		
		//isGet = false;
		if (clap[c]+100 <= start) { //到了一個時間點才會做判斷 而不是按空白鍵做判斷
			if (!isClick) {
				hits_left.Add(-1);
			}
			else {
				if (isGet) {
					hits_left.Add(1);
				}
				else
				{
					hits_left.Add(-1);
				}
			}
			c++;
			isClick = false;
			if (hits_left.GetInteger() <= 0) {
				//CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
				//CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
				/*
				ofs.open("time8.txt");
				for (auto f : fuck) {
					ofs << f << endl;
				}
				ofs.close();
				*/
				if (level == 1) {
					CAudio::Instance()->Stop(AUDIO_ONE);	// 停止 MIDI
				}
				else if (level == 2) {
					CAudio::Instance()->Stop(AUDIO_TWO);	// 停止 MIDI
				}
				else if (level == 3) {
					CAudio::Instance()->Stop(AUDIO_THREE);	// 停止 MIDI
				}
				else if (level == 4) {
					CAudio::Instance()->Stop(AUDIO_FOUR);	// 停止 MIDI
				}
				else if (level == 5) {
					CAudio::Instance()->Stop(AUDIO_FIVE);	// 停止 MIDI
				}
				else if (level == 6) {
					CAudio::Instance()->Stop(AUDIO_SIX);	// 停止 MIDI
				}
				c = 0;
				GotoGameState(GAME_STATE_OVER);
			}
			if (c == total_tap) {
				if (level == 1) {
					CAudio::Instance()->Stop(AUDIO_ONE);	// 停止 MIDI
				}
				else if (level == 2) {
					CAudio::Instance()->Stop(AUDIO_TWO);	// 停止 MIDI
				}
				else if (level == 3) {
					CAudio::Instance()->Stop(AUDIO_THREE);	// 停止 MIDI
				}
				else if (level == 4) {
					CAudio::Instance()->Stop(AUDIO_FOUR);	// 停止 MIDI
				}
				else if (level == 5) {
					CAudio::Instance()->Stop(AUDIO_FIVE);	// 停止 MIDI
				}
				else if (level == 6) {
					CAudio::Instance()->Stop(AUDIO_SIX);	// 停止 MIDI
				}
				fuck0 = (hits_left.GetInteger() * 100 / (total_tap+5));
				c = 0;
				GotoGameState(GAME_STATE_PASS);
				
			}
		}
		
		test1.SetIsShow(false);
		if (start > everytime[0])
			c_practice.OnShow();
		if (start > everytime[28] - 100 && level == 2) {
			xx1.SetTopLeft(310, 220);
			xx1.ShowBitmap();
			xx2.SetTopLeft(370, 220);
			xx2.ShowBitmap();
		}
		
		
		hand.SetTopLeft(640 - 479, 480 - 75);
		hand.OnShow();
		//
		//  貼上左上及右下角落的圖
		//
		if (noise.IsShow()) {
			noise.OnShow();
		}
		noise.SetIsShow(false);
		
		corner.SetTopLeft(0, 0);
		//corner.ShowBitmap();
		corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
		//corner.ShowBitmap();
		//gamemap.OnShow();
	}
}
