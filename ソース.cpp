//[1]�w�b�_�[���C���N���[�h����ꏊ
#include <stdio.h>	//[1-1]�W�����o�̓w�b�_�[���C���N���[�h����
#include <stdlib.h>	//[1-2]�W�����C�u���C���[���C���N���[�h����
#include <time.h>	//[1-3]���ԊǗ��w�b�_�[���C���N���[�h����
#include <conio.h>	//[1-4]�R���\�[�����o�̓w�b�_�[���C���N���[�h����
#include <vector>	//[1-5]�x�N�^�[�w�b�_�[���C���N���[�h����

//[2]�萔���`����ꏊ

#define BOARD_WIDTH (8)		//[2-1]�Ֆʂ̕����`����
#define BOARD_HEIGHT (8)		//[2-2]�Ֆʂ̍������`����

//[3]�񋓒萔���`����ꏊ

//[3-1]�^�[���̎�ނ��`����
enum
{
	TURN_BLACK,		//[3-1-1]��
	TURN_WHITE,		//[3-1-2]��
	TURN_NONE,		//[3-1-3]�Ȃ�
	TURN_MAX		//[3-1-4]�^�[���̐�
};

//[3-2]�����̎�ނ��`����
enum
{
	DIRECTION_UP,				//[3-2-1]��
	DIRECTION_UP_LEFT,			//[3-2-2]����
	DIRECTION_LEFT,				//[3-2-3]��
	DIRECTION_DOWN_LEFT,		//[3-2-4]����
	DIRECTION_DOWN,				//[3-2-5]��
	DIRECTION_DOWN_RIGHT,		//[3-2-6]�E��
	DIRECTION_RIGHT,			//[3-2-7]�E
	DIRECTION_UP_RIGHT,			//[3-2-8]�E��
	DIRECTION_MAX				//[3-2-9]�����̐�
};

//[3-3]�Q�[�����[�h�̎�ނ��`����
enum
{
	MODE_1P,	//[3-3-1]AI�Ƒΐ킷�郂�[�h
	MODE_2P,	//[3-3-2]�l�ԓ��m�̑ΐ탂�[�h
	MODE_WATCH,	//[3-3-3]AI���m�Ό��̊ϐ탂�[�h
	MODE_EXIT,		//[3-3-4]�Q�[�����I������
	MODE_MAX	//[3-3-5]���[�h�̐�
};

//[4]�\���̂�錾����ꏊ

//[4-1]�x�N�g���\���̂�錾����
typedef struct {
	int x, y;
} VEC2;

//[5]�ϐ���錾����ꏊ

//[5-1]�΂̃A�X�L�[�A�[�g��錾����
const char* diskAA[TURN_MAX] =
{
	"��",		//[5-1-1]TURN_BLACK	�����΂��u����Ă���
	"��",		//[5-1-2]TURN_WHITE	�����΂��u����Ă���
	"�E"		//[5-1-3]TURN_NONE	�΂��u����Ă��Ȃ�
}; 

//[5-2]�^�[���̖��O��錾����
const char* turnNames[] =
{
	"��",		//TURN_BLACK
	"��"		//TURN_WHITE
};

//[5-3]���[�h�̖��O��錾����
const char* modeNames[] =
{
	"�P�o�@�f�`�l�d",	//[5-3-1]MODE_1P		AI�Ƒΐ킷�郂�[�h
	"�Q�o�@�f�`�l�d",	//[5-3-2]MODE_2P		�l�ԓ��m�̑ΐ탂�[�h
	"�v�`�s�b�g",		//[5-3-3]MODE_WATCH		AI���m�̑Ό��̊ϐ탂�[�h
	"�d�]�h�s"			//[5-3-4]MODE_EXIT			�Q�[���𔲂���
};

//[5-4]������錾����
VEC2 direction[DIRECTION_MAX] =
{
	{ 0,-1},		//[5-4-1]DIRECTION_UP
	{-1,-1},		//[5-4-2]DIRECTION_UP_LEFT
	{-1,0},			//[5-4-3]DIRECTION_LEFT
	{-1,1},			//[5-4-4]DIRECTION_DOWN_LEFT
	{ 0, 1},		//[5-4-5]DIRECTION_DOWN
	{ 1, 1},		//[5-4-6]DIRECTION_DOWN_RIGHT
	{ 1, 0},		//[5-4-7]DIRECTION_RIGHT
	{ 1,-1}			//[5-4-8]DIRECTION_UP_RIGHT
};

//[5-5]�Ֆʂ̊e�}�X�̏�Ԃ�錾����
int board[BOARD_HEIGHT][BOARD_WIDTH];

VEC2 cursorPosition;	//[5-6]�J�[�\���̍��W��錾����

int turn;	//[5-7]���݂̃^�[����錾����

int mode;	//[5-8]���݂̃��[�h��錾����

bool isPlayer[TURN_MAX];	//[5-9]�e�^�[�����v���C���[���ǂ�����錾����

//[6]�֐���錾����ꏊ

//[6-1]�x�N�g�������Z����֐���錾����
VEC2 VecAdd(VEC2 _v0, VEC2 _v1)
{
	//[6-1-2]���Z�����x�N�g����Ԃ�
	return
	{
		_v0.x + _v1.x,
		_v0.y + _v1.y
	};
}

//[6-2]�΂�u���邩�ǂ����𔻒�A�܂��͐΂��Ђ�����Ԃ��֐���錾����
bool CheckCanPlace(
	int _color,			//�΂̐F
	VEC2 _position,
	bool _turnOver = false)	//���W
{
	bool canPlace = false;		//[6-2-1]�u���邩�ǂ����̃t���O��錾����

	//[6-2-2]�Ώۂ̍��W�ɐ΂��u����Ă��Ȃ������肷��
	if (board[_position.y][_position.x] != TURN_NONE)
	{
		return false;	//[6-2-3]�΂��u����Ă�����u���Ȃ��Ƃ������ʂ�Ԃ�
	}

	//[6-2-5]�S�Ă̕����𔽕�����
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int opponent = _color ^ 1;//[6-2-4]����̐΂̐F��錾����

		//[6-2-5]���݃`�F�b�N���̍��W��錾����
		VEC2 currentPosition = _position;

		//[6-2-7]�ׂ̃}�X�Ɉړ�����
		currentPosition = VecAdd(currentPosition, direction[i]);

		//[6-2-8]����̐΂łȂ������肷��
		if (board[currentPosition.y][currentPosition.x] != opponent)
		{
			//[6-2-9]����̐΂łȂ���΁A���̕����̃`�F�b�N�𒆎~����
			continue;
		}

		//[6-2-10]�������[�v����
		while (1) 
		{
			//[6-2-11]�ׂ̃}�X�Ɉړ�����
			currentPosition = VecAdd(currentPosition, direction[i]);

			//[6-2-12]�Ֆʂ̊O���ɏo�Ă��܂�����A���݂̕����̃`�F�b�N�𔲂���
			if ( (currentPosition.x < 0)
				|| (currentPosition.x >= BOARD_WIDTH)
				|| (currentPosition.y < 0)
				|| (currentPosition.y >= BOARD_HEIGHT) )
			{
				//[6-2-13]�Ֆʂ̊O���ɏo�Ă��܂�����A���݂̕����̃`�F�b�N�𔲂���1
				break;
			}

			//[6-2-14]�`�F�b�N����}�X�ɐ΂��Ȃ����ǂ����𔻒肷��
			if (board[currentPosition.y][currentPosition.x] == TURN_NONE)
			{
				break;	//[6-2-15]�΂��Ȃ���΁A���݂̕����̃`�F�b�N�𔲂���
			}

			//[6-2-16]�`�F�b�N����}�X�Ɏ����̐΂������
			if (board[currentPosition.y][currentPosition.x] == _color)
			{
				//[6-2-17]�΂�u���邱�Ƃ��m�肷��
				canPlace = true;

				//[6-2-18]�Ђ�����Ԃ��t���O�������Ă邩�ǂ����𔻒肷��
				if (_turnOver) {

					//[6-2-19]�Ђ�����Ԃ����W��錾����
					VEC2 reversePosition = _position;

					//[6-2-20]�ׂ̃}�X�Ɉړ�����
					reversePosition = VecAdd(reversePosition, direction[i]);

					//[6-2-21]���݂̃^�[���̐΂�������܂Ŕ�������
					do
					{
						//[6-2-22]����̐΂��Ђ�����Ԃ�
						board[reversePosition.y][reversePosition.x] = _color;

						//[6-2-23]�ׂ̃}�X�Ɉړ�����
						reversePosition = VecAdd(reversePosition, direction[i]);

					} while (board[reversePosition.y][reversePosition.x] != _color);
				}
			}
		}
	}
	return canPlace;			//[6-2-24]�΂�u���邩�ǂ�����Ԃ�


}

//[6-3]�Ֆʏ�ɐ΂�u����}�X�����邩�ǂ����𔻒肷��֐���錾����
bool CheckCanPlaceAll(int _color)
{
	//[6-3-1]�Ֆʂ̂��ׂĂ̍s�𔽕�����
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		//[6-3-2]�Ֆʂ̂��ׂĂ̗�𔽕�����
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			//[6-3-3]���肷����W��錾����
			VEC2 position = { x,y };

			//[6-3-4]�Ώۂ̍��W�ɐ΂��u���邩�ǂ������肷��
			if (CheckCanPlace(
				_color,
				position))
			{
				return true;	//[6-3-5]�΂�u����}�X������Ƃ������ʂ�Ԃ�
			}
		}
	}

	return false;		//[6-3-6]�΂�u����}�X���Ȃ��Ƃ������ʂ�Ԃ�
}

//[6-4]�C�ӂ̐΂̐��𐔂���֐���錾����
int GetDiskCount(int _color)
{
	int count = 0;	//[6-4-1]������΂̐���ێ�����ϐ���錾����

	//[6-4-2]�Ֆʂ̑S�Ă̍s�𔽕�����
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		//[6-4-3]�Ֆʂ̑S�Ă̗�𔽕�����
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			//[6-4-4]�Ώۂ̃}�X�ɁA�΂����邩�ǂ����𔻒肷��
			if (board[y][x] == _color)
			{
				count++;	//[6-4-5]�΂̐������Z����
			}
		}
	}

	return count;	//[6-4-6]�������΂̐���Ԃ�
}

//[6-5]��ʂ�`�悷��֐���錾����
void DrawScreen() {

	system("cls");	//[6-5-1]��ʂ��N���A�ɂ���
	
	//[6-5-2]���ׂĂ̍s�𔽕�����
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		//[6-5-3]���ׂĂ̗�𔽕�����
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			printf("%s", diskAA[board[y][x]]);//[6-5-4]�΂�`�悷��
		}

		//[6-5-5]�v���C���[�̒S�����ǂ����𔻒肷��
		if (isPlayer[turn]) {

			//[6-5-6]�Ώۂ̍s���J�[�\���Ɠ������ǂ����𔻒肷��
			if (y == cursorPosition.y)
			{
				printf("��");	//[6-5-7]�J�[�\����`�悷��
			}
		}


		printf("\n");	//[6-5-8]�s�̕`��̍Ō�ɉ��s������
	}

	//[6-9-9]�v���C���[�̒S�����ǂ����𔻒肷��
	if (isPlayer[turn])
	{

		//[6-5-10]�Ֆʂ̗�̐�������������
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			//[6-5-11]�J�[�\���Ɠ����񂩂ǂ����𔻒肷��
			if (x == cursorPosition.x)
			{
				printf("��");	//[6-5-12]������\������
			}
			else
			{
				printf("�@");	//[6-5-13]�S�p�X�y�[�X��\������
			}
		}
	}


	//[6-5-14]�J�[�\���̕`�悪�I���������s���Ă���
	printf("\n");


	//[6-5-15]���������Ă��Ȃ����ǂ����𔻒肷��
	if (turn != TURN_NONE)
	{
		//[6-5-16]�^�[����\������
		printf("%s�̃^�[���ł�\n", turnNames[turn]);

	}
	//[6-5-17]�����������Ȃ�
	else
	{
		//[6-5-18]�����΂̐���錾����
		int blackCount = GetDiskCount(TURN_BLACK);

		//[6-5-19]�����΂̐���錾����
		int whiteCount = GetDiskCount(TURN_WHITE);

		//[6-5-20]���҂�ێ�����ϐ���錾����
		int winner;

		//[6-5-21]���҂𔻒肷��
		if (blackCount > whiteCount)	//[6-5-22]���̕����������
		{
			winner = TURN_BLACK;	//[6-5-23]���̏���
		}
		else if (whiteCount > blackCount)	//[6-5-24]���̕����������
		{
			winner = TURN_WHITE;	//[6-5-25]���̏���
		}
		else								//[6-5-26]�������Ȃ�
		{
			winner = TURN_NONE;		//[6-5-27]��������
		}

		//[6-5-28]���҂̐΂̐���\������
		printf("%s%d�\%s%d�@",
			turnNames[TURN_BLACK],			//���̖��O
			GetDiskCount(TURN_BLACK),		//���̐΂̐�
			turnNames[TURN_WHITE],			//���̖��O
			GetDiskCount(TURN_WHITE));		//���̐F�̐�

		//[6-9-29]�����������ǂ������肷��
		if (winner == TURN_NONE)
		{
			printf("��������\n");
		}
		else
		{
			printf("%s�̏���", turnNames[winner]);
		}
	}
}

//[6-6]���[�h�I����ʂ̊֐���錾����
void SelectMode()
{
	mode = MODE_1P;	//[6-6-1]�Q�[�����[�h������������

	//[6-6-2]�������[�v����
	while(1)
	{
		system("cls");	//[6-6-3]��ʂ��N���A�ɂ���

		//[6-6-4]���b�Z�[�W��\������
		printf("���[�h���@�I������\n��������\n");

		printf("\n\n");		//[6-6-5]�Q�s�󂯂�

		//[6-6-6]�S�Ẵ��[�h�𔽕�����
		for (int i = 0; i < MODE_MAX; i++)
		{
			//[6-6-7]���݂̃��[�h�ɂ̓J�[�\�����A����ȊO�ɂ̓X�y�[�X��`�悷��
			printf("%s�@", (i == mode) ? "��" : "�@");

			printf("%s\n", modeNames[i]);	//[6-6-8]���[�h�̖��O��`�悷��

			printf("\n");	//[6-6-9]�P�s�󂯂�
		}

		//[6-6-10]���͂��ꂽ�L�[�ŕ��򂷂�
		switch (_getch())
		{
		case 'w':		//[6-6-11]w�L�[�������ꂽ��
			mode--;		//[6-6-12]�O�̃��[�h�ɐ؂�ւ���
			break;

		case 's':		//[6-6-11]s�L�[�������ꂽ��
			mode++;		//[6-6-12]���̃��[�h�ɐ؂�ւ���
			break;

		default:		//[6-6-15]���̑��̃L�[�������ꂽ��

			//[6-6-16]�I�����ꂽ���[�h�ŕ��򂷂�
			switch (mode)
			{
			case MODE_1P:		//[6-6-17]AI�Ƒΐ킷�郂�[�h�Ȃ�

				isPlayer[TURN_BLACK] = true;	//[6-8-18]�����v���C���[�ɂ���
				isPlayer[TURN_WHITE] = false;	//[6-8-19]�����v���C���[�ɂ��Ȃ�

				break;
				
			case MODE_2P:		//[6-6-20]�l�ԓ��m�̑ΐ탂�[�h�Ȃ�

				//[6-6-21]���҂��v���C���[�̒S���ɂ���
				isPlayer[TURN_BLACK] = isPlayer[TURN_WHITE] = true;		

				break;

			case MODE_WATCH:	//[6-6-22]�l�ԓ��m�̑ΐ탂�[�h�Ȃ�

				//[6-6-23]���҂��v���C���[�̒S���ɂ���
				isPlayer[TURN_BLACK] = isPlayer[TURN_WHITE] = false;

				break;

			case MODE_EXIT:			//[6-6-24]

				//[6-6-25]�v���O�������I��������
				mode = MODE_EXIT;

				break;

			}

			return;		//[6-6-2]���[�h�I���𔲂���
		}

		//[6-6-2]�J�[�\�����㉺�Ƀ��[�v������
		mode = (MODE_MAX + mode) % MODE_MAX;
	}
} 

//[6-7]�Q�[��������������֐���錾����
void Init()
{
	//[6-7-1]�Ֆʓ��̑S�Ă̗�𔽕�����
	for (int y = 0; y < BOARD_HEIGHT; y++) 
	{
		//[6-7-2]�Ֆʂ̂��ׂĂ̍s�𔽕�����
		for (int x = 0; x < BOARD_WIDTH; x++) 
		{
			//[6-7-3]�Ώۂ̃}�X��΂��u����ĂȂ���Ԃɂ���
			board[y][x] = TURN_NONE;
		}
	}
	//[6-7-4]�Ֆʒ����̉E��ƍ����ɍ����΂�u��
	board[4][3] = board[3][4] = TURN_BLACK;

	//[6-7-5]�Ֆʒ����̍���ƉE���ɔ����΂�u��
	board[3][3] = board[4][4] = TURN_WHITE;

	turn = TURN_BLACK;	//[6-7-6]���̃^�[���ŏ���������

	cursorPosition = { 3,3 };//[6-7-7]�J�[�\���̍��W������������

	DrawScreen();	//[6-7-8]��ʂ�`�悷��֐����Ăяo��
}

//[6-8]�΂�u���}�X��I������֐���錾����
VEC2 InputPosition()
{
	//[6-8-1]�u����}�X���I�������܂Ŗ������[�v����
	while (1) 
	{
		DrawScreen();	//[6-8-2]��ʂ�`�悷��֐����Ăяo��

		//[6-8-3]���͂��ꂽ�L�[�ɂ���ĕ��򂷂�
		switch (_getch())
		{
		case 'w':						//[6-8-4]w�L�[�������ꂽ��
			cursorPosition.y--;			//[6-8-5]�J�[�\������Ɉړ�����
			break;

		case 's':						//[6-8-4]s�L�[�������ꂽ��
			cursorPosition.y++;			//[6-8-5]�J�[�\�������Ɉړ�����
			break;

		case 'a':						//[6-8-4]a�L�[�������ꂽ��
			cursorPosition.x--;			//[6-8-5]�J�[�\�������Ɉړ�����
			break;

		case 'd':						//[6-8-4]d�L�[�������ꂽ��
			cursorPosition.x++;			//[6-8-5]�J�[�\�����E�Ɉړ�����
			break;

		default:						//[6-8-12]��L�ȊO�̃L�[�������ꂽ��

			//[6-8-13]�J�[�\���̍��W�ɐ΂��u���邩�ǂ������肷��
			if (CheckCanPlace(turn, cursorPosition))	
			{
				return cursorPosition;	//[6-8-14]�J�[�\���̍��W��Ԃ�

			}
			else	//[6-8-15]�u���Ȃ����
			{
				//[6-8-16]�u���Ȃ��������b�Z�[�W��\������
				printf("�����ɂ́@�u���܂���");

				_getch();		//[6-8-17]�L�[�{�[�h���͂�҂�
			}

			break;
		}

		

		//[6-8-18]�J�[�\�������E�Ƀ��[�v������
		cursorPosition.x = (BOARD_WIDTH + cursorPosition.x) % BOARD_WIDTH;

		//[6-8-19]�J�[�\�����㉺�Ƀ��[�v����
		cursorPosition.y = (BOARD_HEIGHT + cursorPosition.y) % BOARD_HEIGHT;
	}
}

//[6-9]�v���O�����̊J�n�_��錾����ꏊ
int main()
{
	srand((unsigned int)time(NULL));	//[6-9-1]�������V���b�t������

start:		//[6-9-2]�J�n���x��
	;		//[6-9-3]��

	SelectMode();	//[6-9-4]���[�h��I������֐����Ăяo��

	//[6-9-4-1]�Q�[���I����������Ă�����
	if (mode == MODE_EXIT) {

		return 0;		//[6-9-4-1-1]�v���O�������I��������
	}

	//[6-9-4-2]����ȊO�Ȃ�
	else {
		Init();		//[6-9-5]�Q�[��������������֐����Ăяo��

		//[6-9-6]���C�����[�v
		while (1)
		{
			//[6-9-7]�u����}�X���Ȃ����ǂ����𔻒肷��
			if (!CheckCanPlaceAll(turn))
			{
				turn ^= 1;	//[6-9-8]�^�[����؂�ւ���

				//[6-9-9]����ɒu����}�X���Ȃ����ǂ����𔻒肷��
				if (!CheckCanPlaceAll(turn))
				{
					turn = TURN_NONE;	//[6-9-10]�������������Ƃɂ���

					DrawScreen();		//[6-9-11]��ʂ�`�悷��֐����Ăяo��

					_getch();			//[6-9-12]�L�[�{�[�h���͂�҂�

					goto start;			//[6-9-13]�J�n���x���ɃW�����v����
				}
				//[6-9-14]����ɒu����}�X�������
				else
				{
					continue;	//[6-9-15]����^�[���փX�L�b�v����
				}

			}

			//[6-9-16]�΂�u���}�X��錾����
			VEC2 placePotion;

			//[6-9-17]���݂̃^�[���̒S�����v���C���[���ǂ����𔻒肷��
			if (isPlayer[turn])
			{
				//[6-9-18]�΂�u���}�X��I������֐����Ăяo��
				placePotion = InputPosition();
			}
			else
			{
				DrawScreen();	//[6-9-20]�Ֆʂ�`�悷��֐����Ăяo��

				_getch();		//[6-9-21]�L�[�{�[�h���͂�҂�

				//[6-9-22]�u������W��ێ�����x�N�^�[��錾����
				std::vector<VEC2> positions;

				//[6-9-23]�Ֆʂ̂��ׂĂ̍s�𔽕�����
				for (int y = 0; y < BOARD_HEIGHT; y++)
				{
					//[6-9-24]�Ֆʂ̂��ׂĂ̗�𔽕�����
					for (int x = 0; x < BOARD_WIDTH; x++)
					{
						//[6-9-25]�Ώۂ̃}�X�̍��W��錾����
						VEC2 position = { x,y };

						//[6-9-26]�Ώۂ̍��W�ɐ΂��u���邩�ǂ������肷��
						if (CheckCanPlace(turn, position))
						{
							//[6-9-27]�u������W�̃��X�g�ɑΏۂ̍��W��ǉ�����
							positions.push_back(position);
						}
					}
				}

				//[6-9-28]�u����ꏊ�������_���Ɏ擾����
				placePotion = positions[rand() % positions.size()];
			}


			//[6-2-29]�΂��Ђ�����Ԃ�
			CheckCanPlace(turn, placePotion, true);

			//[6-9-30]���݂̃^�[���̐΂�u��
			board[placePotion.y][placePotion.x] = turn;

			turn ^= 1;	 //[6-9-31]
		}
	}

}