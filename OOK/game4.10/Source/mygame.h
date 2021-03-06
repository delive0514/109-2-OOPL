/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include "CEraser.h"
#include "CBall.h"
#include "CBouncingBall.h"
#include "ClongGray.h"
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "stage4.h"
#include "stage5.h"
#include "stage6.h"
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT,				// 2
		AUDIO_CLICK,
		AUDIO_TEST,
		AUDIO_ONE,
		AUDIO_TWO,
		AUDIO_THREE,
		AUDIO_FOUR,
		AUDIO_FIVE,
		AUDIO_SIX
	};
	class CBouncingBall;
	class CPractice {
	public:
		CPractice();
		void LoadBitmap();
		void OnMove(int );
		void OnShow();
		void OnJump();
		void OnJump2();
		int getX();
	private:
		CMovingBitmap pic;
		int x, y;
	};
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
		void OnMove();
	private:
		CAnimation logo;								// csie的logo
		CMovingBitmap title;
		CMovingBitmap BG;
		ClongGray about;
		CAnimation title1;
		int x,y;
	};
	class CGameStateStage : public CGameState {
	public:
		CGameStateStage(CGame* g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
		void OnMove();
	private:
		CMovingBitmap bg;
		CAnimation stage11;
		CAnimation stage12;
		CAnimation stage13;
		CMovingBitmap select;
		CPractice test11;
		int x, y;
	};
	class CGameStateStage2 : public CGameState {
	public:
		CGameStateStage2(CGame* g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
		void OnMove();
	private:
		CMovingBitmap bg;
		CMovingBitmap stage11;
		CMovingBitmap stage12;
		CMovingBitmap stage13;
		CMovingBitmap select;
		CPractice test11;
		int x, y;
	};
	class CGameStateStore : public CGameState {
	public:
		CGameStateStore(CGame* g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
		void OnMove();
	private:
		CMovingBitmap bg;
		CMovingBitmap store1;
		CMovingBitmap store2;
		CMovingBitmap store3;
		CMovingBitmap select;
		CPractice test11;
		int x, y;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap	background;	// 背景圖
		CMovingBitmap   bg;
		CAnimation		bg1;
		CMovingBitmap   bg2;
		CAnimation      bg3;
		CAnimation      bg4;
		CMovingBitmap   bg5;
		CMovingBitmap	help;		// 說明圖
		CMovingBitmap   test;
		CMovingBitmap	corner;		// 角落圖
		CMovingBitmap   hand1;
		CMovingBitmap   xx1;
		CMovingBitmap   xx2;
		ClongGray   noise;
		ClongGray		hand2;
		CInteger		hits_left;	// 剩下的撞擊數
		CInteger		num;
		CInteger		clocktime;
		CMovingBitmap long_gray;
		CPractice c_practice;
		ClongGray test1;
		vector<int> everytime;
		vector<int> location;
		vector<int> jump_time_list;
		bool isClick;
		bool isGet;
		CAnimation hand;
		CMovingBitmap bar;
		CAnimation background1;
		CAnimation tempo;
		int beat_x = 1;
		int beat_y = 0;
		int c = 0;      //時間的位置
		vector<int> clap;
		vector<int> first;
		int total_tap;
		int jump_time=2;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
	};

	class CGameStatePass : public CGameState {
	public:
		CGameStatePass(CGame* g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
	};

}