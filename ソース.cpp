//[1]ヘッダーをインクルードする場所
#include <stdio.h>	//[1-1]標準入出力ヘッダーをインクルードする
#include <stdlib.h>	//[1-2]標準ライブライリーをインクルードする
#include <time.h>	//[1-3]時間管理ヘッダーをインクルードする
#include <conio.h>	//[1-4]コンソール入出力ヘッダーをインクルードする
#include <vector>	//[1-5]ベクターヘッダーをインクルードする

//[2]定数を定義する場所

#define BOARD_WIDTH (8)		//[2-1]盤面の幅を定義する
#define BOARD_HEIGHT (8)		//[2-2]盤面の高さを定義する

//[3]列挙定数を定義する場所

//[3-1]ターンの種類を定義する
enum
{
	TURN_BLACK,		//[3-1-1]黒
	TURN_WHITE,		//[3-1-2]白
	TURN_NONE,		//[3-1-3]なし
	TURN_MAX		//[3-1-4]ターンの数
};

//[3-2]方向の種類を定義する
enum
{
	DIRECTION_UP,				//[3-2-1]上
	DIRECTION_UP_LEFT,			//[3-2-2]左上
	DIRECTION_LEFT,				//[3-2-3]左
	DIRECTION_DOWN_LEFT,		//[3-2-4]左下
	DIRECTION_DOWN,				//[3-2-5]下
	DIRECTION_DOWN_RIGHT,		//[3-2-6]右下
	DIRECTION_RIGHT,			//[3-2-7]右
	DIRECTION_UP_RIGHT,			//[3-2-8]右上
	DIRECTION_MAX				//[3-2-9]方向の数
};

//[3-3]ゲームモードの種類を定義する
enum
{
	MODE_1P,	//[3-3-1]AIと対戦するモード
	MODE_2P,	//[3-3-2]人間同士の対戦モード
	MODE_WATCH,	//[3-3-3]AI同士対決の観戦モード
	MODE_EXIT,		//[3-3-4]ゲームを終了する
	MODE_MAX	//[3-3-5]モードの数
};

//[4]構造体を宣言する場所

//[4-1]ベクトル構造体を宣言する
typedef struct {
	int x, y;
} VEC2;

//[5]変数を宣言する場所

//[5-1]石のアスキーアートを宣言する
const char* diskAA[TURN_MAX] =
{
	"●",		//[5-1-1]TURN_BLACK	黒い石が置かれている
	"○",		//[5-1-2]TURN_WHITE	白い石が置かれている
	"・"		//[5-1-3]TURN_NONE	石が置かれていない
}; 

//[5-2]ターンの名前を宣言する
const char* turnNames[] =
{
	"黒",		//TURN_BLACK
	"白"		//TURN_WHITE
};

//[5-3]モードの名前を宣言する
const char* modeNames[] =
{
	"１Ｐ　ＧＡＭＥ",	//[5-3-1]MODE_1P		AIと対戦するモード
	"２Ｐ　ＧＡＭＥ",	//[5-3-2]MODE_2P		人間同士の対戦モード
	"ＷＡＴＣＨ",		//[5-3-3]MODE_WATCH		AI同士の対決の観戦モード
	"ＥⅩＩＴ"			//[5-3-4]MODE_EXIT			ゲームを抜ける
};

//[5-4]方向を宣言する
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

//[5-5]盤面の各マスの状態を宣言する
int board[BOARD_HEIGHT][BOARD_WIDTH];

VEC2 cursorPosition;	//[5-6]カーソルの座標を宣言する

int turn;	//[5-7]現在のターンを宣言する

int mode;	//[5-8]現在のモードを宣言する

bool isPlayer[TURN_MAX];	//[5-9]各ターンがプレイヤーかどうかを宣言する

//[6]関数を宣言する場所

//[6-1]ベクトルを加算する関数を宣言する
VEC2 VecAdd(VEC2 _v0, VEC2 _v1)
{
	//[6-1-2]加算したベクトルを返す
	return
	{
		_v0.x + _v1.x,
		_v0.y + _v1.y
	};
}

//[6-2]石を置けるかどうかを判定、または石をひっくり返す関数を宣言する
bool CheckCanPlace(
	int _color,			//石の色
	VEC2 _position,
	bool _turnOver = false)	//座標
{
	bool canPlace = false;		//[6-2-1]置けるかどうかのフラグを宣言する

	//[6-2-2]対象の座標に石が置かれていないか判定する
	if (board[_position.y][_position.x] != TURN_NONE)
	{
		return false;	//[6-2-3]石が置かれていたら置けないという結果を返す
	}

	//[6-2-5]全ての方向を反復する
	for (int i = 0; i < DIRECTION_MAX; i++)
	{
		int opponent = _color ^ 1;//[6-2-4]相手の石の色を宣言する

		//[6-2-5]現在チェック中の座標を宣言する
		VEC2 currentPosition = _position;

		//[6-2-7]隣のマスに移動する
		currentPosition = VecAdd(currentPosition, direction[i]);

		//[6-2-8]相手の石でないか判定する
		if (board[currentPosition.y][currentPosition.x] != opponent)
		{
			//[6-2-9]相手の石でなければ、その方向のチェックを中止する
			continue;
		}

		//[6-2-10]無限ループする
		while (1) 
		{
			//[6-2-11]隣のマスに移動する
			currentPosition = VecAdd(currentPosition, direction[i]);

			//[6-2-12]盤面の外側に出てしまったら、現在の方向のチェックを抜ける
			if ( (currentPosition.x < 0)
				|| (currentPosition.x >= BOARD_WIDTH)
				|| (currentPosition.y < 0)
				|| (currentPosition.y >= BOARD_HEIGHT) )
			{
				//[6-2-13]盤面の外側に出てしまったら、現在の方向のチェックを抜ける1
				break;
			}

			//[6-2-14]チェックするマスに石がないかどうかを判定する
			if (board[currentPosition.y][currentPosition.x] == TURN_NONE)
			{
				break;	//[6-2-15]石がなければ、現在の方向のチェックを抜ける
			}

			//[6-2-16]チェックするマスに自分の石があれば
			if (board[currentPosition.y][currentPosition.x] == _color)
			{
				//[6-2-17]石を置けることが確定する
				canPlace = true;

				//[6-2-18]ひっくり返しフラグが立ってるかどうかを判定する
				if (_turnOver) {

					//[6-2-19]ひっくり返す座標を宣言する
					VEC2 reversePosition = _position;

					//[6-2-20]隣のマスに移動する
					reversePosition = VecAdd(reversePosition, direction[i]);

					//[6-2-21]現在のターンの石が見つかるまで反復する
					do
					{
						//[6-2-22]相手の石をひっくり返す
						board[reversePosition.y][reversePosition.x] = _color;

						//[6-2-23]隣のマスに移動する
						reversePosition = VecAdd(reversePosition, direction[i]);

					} while (board[reversePosition.y][reversePosition.x] != _color);
				}
			}
		}
	}
	return canPlace;			//[6-2-24]石を置けるかどうかを返す


}

//[6-3]盤面上に石を置けるマスがあるかどうかを判定する関数を宣言する
bool CheckCanPlaceAll(int _color)
{
	//[6-3-1]盤面のすべての行を反復する
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		//[6-3-2]盤面のすべての列を反復する
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			//[6-3-3]判定する座標を宣言する
			VEC2 position = { x,y };

			//[6-3-4]対象の座標に石が置けるかどうか判定する
			if (CheckCanPlace(
				_color,
				position))
			{
				return true;	//[6-3-5]石を置けるマスがあるという結果を返す
			}
		}
	}

	return false;		//[6-3-6]石を置けるマスがないという結果を返す
}

//[6-4]任意の石の数を数える関数を宣言する
int GetDiskCount(int _color)
{
	int count = 0;	//[6-4-1]数える石の数を保持する変数を宣言する

	//[6-4-2]盤面の全ての行を反復する
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		//[6-4-3]盤面の全ての列を反復する
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			//[6-4-4]対象のマスに、石があるかどうかを判定する
			if (board[y][x] == _color)
			{
				count++;	//[6-4-5]石の数を加算する
			}
		}
	}

	return count;	//[6-4-6]数えた石の数を返す
}

//[6-5]画面を描画する関数を宣言する
void DrawScreen() {

	system("cls");	//[6-5-1]画面をクリアにする
	
	//[6-5-2]すべての行を反復する
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		//[6-5-3]すべての列を反復する
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			printf("%s", diskAA[board[y][x]]);//[6-5-4]石を描画する
		}

		//[6-5-5]プレイヤーの担当かどうかを判定する
		if (isPlayer[turn]) {

			//[6-5-6]対象の行がカーソルと同じかどうかを判定する
			if (y == cursorPosition.y)
			{
				printf("←");	//[6-5-7]カーソルを描画する
			}
		}


		printf("\n");	//[6-5-8]行の描画の最後に改行を入れる
	}

	//[6-9-9]プレイヤーの担当かどうかを判定する
	if (isPlayer[turn])
	{

		//[6-5-10]盤面の列の数だけ反復する
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			//[6-5-11]カーソルと同じ列かどうかを判定する
			if (x == cursorPosition.x)
			{
				printf("↑");	//[6-5-12]↑矢印を表示する
			}
			else
			{
				printf("　");	//[6-5-13]全角スペースを表示する
			}
		}
	}


	//[6-5-14]カーソルの描画が終わったら改行しておく
	printf("\n");


	//[6-5-15]決着がついていないかどうかを判定する
	if (turn != TURN_NONE)
	{
		//[6-5-16]ターンを表示する
		printf("%sのターンです\n", turnNames[turn]);

	}
	//[6-5-17]決着がついたなら
	else
	{
		//[6-5-18]黒い石の数を宣言する
		int blackCount = GetDiskCount(TURN_BLACK);

		//[6-5-19]白い石の数を宣言する
		int whiteCount = GetDiskCount(TURN_WHITE);

		//[6-5-20]勝者を保持する変数を宣言する
		int winner;

		//[6-5-21]勝者を判定する
		if (blackCount > whiteCount)	//[6-5-22]黒の方が多ければ
		{
			winner = TURN_BLACK;	//[6-5-23]黒の勝ち
		}
		else if (whiteCount > blackCount)	//[6-5-24]白の方が多ければ
		{
			winner = TURN_WHITE;	//[6-5-25]白の勝ち
		}
		else								//[6-5-26]同じ数なら
		{
			winner = TURN_NONE;		//[6-5-27]引き分け
		}

		//[6-5-28]両者の石の数を表示する
		printf("%s%d―%s%d　",
			turnNames[TURN_BLACK],			//黒の名前
			GetDiskCount(TURN_BLACK),		//黒の石の数
			turnNames[TURN_WHITE],			//白の名前
			GetDiskCount(TURN_WHITE));		//白の色の数

		//[6-9-29]引き分けかどうか判定する
		if (winner == TURN_NONE)
		{
			printf("引き分け\n");
		}
		else
		{
			printf("%sの勝ち", turnNames[winner]);
		}
	}
}

//[6-6]モード選択画面の関数を宣言する
void SelectMode()
{
	mode = MODE_1P;	//[6-6-1]ゲームモードを初期化する

	//[6-6-2]無限ループする
	while(1)
	{
		system("cls");	//[6-6-3]画面をクリアにする

		//[6-6-4]メッセージを表示する
		printf("モードを　選択して\nください\n");

		printf("\n\n");		//[6-6-5]２行空ける

		//[6-6-6]全てのモードを反復する
		for (int i = 0; i < MODE_MAX; i++)
		{
			//[6-6-7]現在のモードにはカーソルを、それ以外にはスペースを描画する
			printf("%s　", (i == mode) ? "＞" : "　");

			printf("%s\n", modeNames[i]);	//[6-6-8]モードの名前を描画する

			printf("\n");	//[6-6-9]１行空ける
		}

		//[6-6-10]入力されたキーで分岐する
		switch (_getch())
		{
		case 'w':		//[6-6-11]wキーが押されたら
			mode--;		//[6-6-12]前のモードに切り替える
			break;

		case 's':		//[6-6-11]sキーが押されたら
			mode++;		//[6-6-12]次のモードに切り替える
			break;

		default:		//[6-6-15]その他のキーが押されたら

			//[6-6-16]選択されたモードで分岐する
			switch (mode)
			{
			case MODE_1P:		//[6-6-17]AIと対戦するモードなら

				isPlayer[TURN_BLACK] = true;	//[6-8-18]黒をプレイヤーにする
				isPlayer[TURN_WHITE] = false;	//[6-8-19]白をプレイヤーにしない

				break;
				
			case MODE_2P:		//[6-6-20]人間同士の対戦モードなら

				//[6-6-21]両者をプレイヤーの担当にする
				isPlayer[TURN_BLACK] = isPlayer[TURN_WHITE] = true;		

				break;

			case MODE_WATCH:	//[6-6-22]人間同士の対戦モードなら

				//[6-6-23]両者をプレイヤーの担当にする
				isPlayer[TURN_BLACK] = isPlayer[TURN_WHITE] = false;

				break;

			case MODE_EXIT:			//[6-6-24]

				//[6-6-25]プログラムを終了させる
				mode = MODE_EXIT;

				break;

			}

			return;		//[6-6-2]モード選択を抜ける
		}

		//[6-6-2]カーソルを上下にループさせる
		mode = (MODE_MAX + mode) % MODE_MAX;
	}
} 

//[6-7]ゲームを初期化する関数を宣言する
void Init()
{
	//[6-7-1]盤面内の全ての列を反復する
	for (int y = 0; y < BOARD_HEIGHT; y++) 
	{
		//[6-7-2]盤面のすべての行を反復する
		for (int x = 0; x < BOARD_WIDTH; x++) 
		{
			//[6-7-3]対象のマスを石が置かれてない状態にする
			board[y][x] = TURN_NONE;
		}
	}
	//[6-7-4]盤面中央の右上と左下に黒い石を置く
	board[4][3] = board[3][4] = TURN_BLACK;

	//[6-7-5]盤面中央の左上と右下に白い石を置く
	board[3][3] = board[4][4] = TURN_WHITE;

	turn = TURN_BLACK;	//[6-7-6]黒のターンで初期化する

	cursorPosition = { 3,3 };//[6-7-7]カーソルの座標を初期化する

	DrawScreen();	//[6-7-8]画面を描画する関数を呼び出す
}

//[6-8]石を置くマスを選択する関数を宣言する
VEC2 InputPosition()
{
	//[6-8-1]置けるマスが選択されるまで無限ループする
	while (1) 
	{
		DrawScreen();	//[6-8-2]画面を描画する関数を呼び出す

		//[6-8-3]入力されたキーによって分岐する
		switch (_getch())
		{
		case 'w':						//[6-8-4]wキーを押されたら
			cursorPosition.y--;			//[6-8-5]カーソルを上に移動する
			break;

		case 's':						//[6-8-4]sキーを押されたら
			cursorPosition.y++;			//[6-8-5]カーソルを下に移動する
			break;

		case 'a':						//[6-8-4]aキーを押されたら
			cursorPosition.x--;			//[6-8-5]カーソルを左に移動する
			break;

		case 'd':						//[6-8-4]dキーを押されたら
			cursorPosition.x++;			//[6-8-5]カーソルを右に移動する
			break;

		default:						//[6-8-12]上記以外のキーが押されたら

			//[6-8-13]カーソルの座標に石が置けるかどうか判定する
			if (CheckCanPlace(turn, cursorPosition))	
			{
				return cursorPosition;	//[6-8-14]カーソルの座標を返す

			}
			else	//[6-8-15]置けなければ
			{
				//[6-8-16]置けなかったメッセージを表示する
				printf("そこには　置けません");

				_getch();		//[6-8-17]キーボード入力を待つ
			}

			break;
		}

		

		//[6-8-18]カーソルを左右にループさせる
		cursorPosition.x = (BOARD_WIDTH + cursorPosition.x) % BOARD_WIDTH;

		//[6-8-19]カーソルを上下にループする
		cursorPosition.y = (BOARD_HEIGHT + cursorPosition.y) % BOARD_HEIGHT;
	}
}

//[6-9]プログラムの開始点を宣言する場所
int main()
{
	srand((unsigned int)time(NULL));	//[6-9-1]乱数をシャッフルする

start:		//[6-9-2]開始ラベル
	;		//[6-9-3]空文

	SelectMode();	//[6-9-4]モードを選択する関数を呼び出す

	//[6-9-4-1]ゲーム終了が押されていたら
	if (mode == MODE_EXIT) {

		return 0;		//[6-9-4-1-1]プログラムを終了させる
	}

	//[6-9-4-2]それ以外なら
	else {
		Init();		//[6-9-5]ゲームを初期化する関数を呼び出す

		//[6-9-6]メインループ
		while (1)
		{
			//[6-9-7]置けるマスがないかどうかを判定する
			if (!CheckCanPlaceAll(turn))
			{
				turn ^= 1;	//[6-9-8]ターンを切り替える

				//[6-9-9]相手に置けるマスがないかどうかを判定する
				if (!CheckCanPlaceAll(turn))
				{
					turn = TURN_NONE;	//[6-9-10]決着がついたことにする

					DrawScreen();		//[6-9-11]画面を描画する関数を呼び出す

					_getch();			//[6-9-12]キーボード入力を待つ

					goto start;			//[6-9-13]開始ラベルにジャンプする
				}
				//[6-9-14]相手に置けるマスがあれば
				else
				{
					continue;	//[6-9-15]相手ターンへスキップする
				}

			}

			//[6-9-16]石を置くマスを宣言する
			VEC2 placePotion;

			//[6-9-17]現在のターンの担当がプレイヤーかどうかを判定する
			if (isPlayer[turn])
			{
				//[6-9-18]石を置くマスを選択する関数を呼び出す
				placePotion = InputPosition();
			}
			else
			{
				DrawScreen();	//[6-9-20]盤面を描画する関数を呼び出す

				_getch();		//[6-9-21]キーボード入力を待つ

				//[6-9-22]置ける座標を保持するベクターを宣言する
				std::vector<VEC2> positions;

				//[6-9-23]盤面のすべての行を反復する
				for (int y = 0; y < BOARD_HEIGHT; y++)
				{
					//[6-9-24]盤面のすべての列を反復する
					for (int x = 0; x < BOARD_WIDTH; x++)
					{
						//[6-9-25]対象のマスの座標を宣言する
						VEC2 position = { x,y };

						//[6-9-26]対象の座標に石が置けるかどうか判定する
						if (CheckCanPlace(turn, position))
						{
							//[6-9-27]置ける座標のリストに対象の座標を追加する
							positions.push_back(position);
						}
					}
				}

				//[6-9-28]置ける場所をランダムに取得する
				placePotion = positions[rand() % positions.size()];
			}


			//[6-2-29]石をひっくり返す
			CheckCanPlace(turn, placePotion, true);

			//[6-9-30]現在のターンの石を置く
			board[placePotion.y][placePotion.x] = turn;

			turn ^= 1;	 //[6-9-31]
		}
	}

}