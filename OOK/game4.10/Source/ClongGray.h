namespace game_framework {
	//�`����ͤ��תť���|�]�X�Ӫ�����
	class ClongGray
	{
	public:
		ClongGray();
		bool IsAlive();											// �O�_����
		bool IsShow();
		void LoadBitmap(int x);										// ���J�ϧ�
		//void OnMove();											// ����
		void OnShow();											// �N�ϧζK��e��
		void SetXY(int nx, int ny);								// �]�w�y��
		void SetIsAlive(bool alive);							// �]�w�O�_����
		void SetIsShow(bool show);
		//void SetDelay(int d);								
	protected:
		CMovingBitmap bmp;			// ��
		//CMovingBitmap bmp_center;	// ��ߪ���			
		int x, y;					// �Ϫ��y��
		bool is_alive;				// �O�_����
		bool is_show;
	};
}