#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>

using namespace std;

/*
    실습
    영웅은 절차적
    
    1) 텍스트로 진행되는 게임
    1) 텍스트로 진행되는 게임
    2) 게임이 시작되면 영웅의 이름과 난이도를 입력
    3) 난이도에 따라서 영웅의 HP / 만나게 되는 몬스터의 숫자가 변동
    4) 몬스터를 잡으면 랜덤하게 돈을 획득한다.(0~100)
    5) 몬스터를 잡으면 던전을 계속 탐험할지 상점을 들를지 결정한다
    6) 상점에 들르면 돈을 소모해서 HP를 회복할 수 있다.
    6_1) 전체회복, +10 +20 +30 차등회복
    7) HP가 0이 되면 게임오버
    8) 몬스터를 모두 잡으면 게임클리어

    0)가위바위보
    0_1) 비기면 승패가 정해질 떄까지 반복
    0_2) 졌을떄만 영웅 HP가 줄어든다.
    0_3) 몬스터는 한번 지면 바로 죽음


	1.체력이 100이하일떄만 포션이 사지도록하기 //해결
	2.몬스터 스폰위치가 저번과는 판과 다르게 하기 //해결
	3.몬스터 숨기기(좌표값으로만 설정하기) //해결
	4.변수명 깔끔하게 하기 //해결


	버그1. 몬스터를 만나면 그냥 패배 처리가 되는 경우가 있음		//해결
	버그2. 게임종료시에 상점던전선택 문구가 나옴			  //해결
	버그3. 가위바위보할떄 123보다 큰숫자를 내면 그냥 이김  	//해결
	버그4. 던전or상점선택시에 다른숫자를 내면 던전에 가짐	 //해결


	season2.
	맵 크기가 난이도에 따라 달라짐

	플레이어가 2칸 움직일때마다 몬스터가 1칸 움직임



	영웅은 절차적 2
	배열 / 구조체 / 함수를 최대한 활용해보자.

	맵의 구성 : 타일로 구성된다.

	타일에는 종류가 있다.(숲, 늪, 땅)마다 만나는 몬스터가 다르다.

	타일 : x좌표, y좌표, 타입, 모양

	플레이어 : 이름, 최대HP(레벨에 따라 증가), HP, 경험치, 레벨, 소유 골드

	몬스터 : 이름, 최대HP, HP, 획득할 수 있는 경험치 / 골드

	물약  : 이름, 가격, 회복량

	상점 : 물약 종류와 수량
	+ 알파 (환영)


	-3중포인터 공부하기 *************************************************************************************************************
	//2중배열을 3중 포인터로 받는이유
	(*map)[i][j] // 함수안에서 받는경우


	-정확한 delete[] (동적할당해제)를 하는 시점과 이유


	변수를 매개변수로 함수에 접근하면 값이 바뀌지않는다 왜냐하면 메모리주소가 다르기 떄문 //서로 다른 변수다

	int main()
	{
		changeNum(&num) //메모리값 보내기
	}

	changeNum(int* num) //메모리값에 접근해서 변경
*/

char fieldTypeChar[] = { '-', '^', '#' };
enum FIELDTYPE { field, swamp, wood};

struct TILE
{
	FIELDTYPE fieldType;
	char shape;
	bool monster;
	bool player;
};

void SetMap(TILE** map, int MapSize)
{
	for (int j = 0; j < MapSize; j++)
	{
		for (int i = 0; i < MapSize; i++)
		{
			map[j][i].fieldType = (FIELDTYPE)(rand() % 3);
			switch (map[j][i].fieldType)
			{
			case FIELDTYPE::field:
				map[j][i].shape = fieldTypeChar[FIELDTYPE::field];
				break;

			case FIELDTYPE::swamp:
				map[j][i].shape = fieldTypeChar[FIELDTYPE::swamp];
				break;

			case FIELDTYPE::wood:
				map[j][i].shape = fieldTypeChar[FIELDTYPE::wood];
				break;
			}
		}
	}	
}

void PrintMap(TILE** map, int MapSize, int pX, int pY)
{
	for (int j = 0; j < MapSize; j++)
	{
		for (int i = 0; i < MapSize; i++)
		{
			if (i == pY && j == pX)
			{
				map[j][i].player = true;
				cout << 'P';
			}
			else if (!map[j][i].monster) cout << map[j][i].shape;
			else cout << 'O';
		}
		cout << endl;
	}
}

struct potionType //이름 가격 회복량
{
	string name;  //이름
	int price;	  //가격
	int recovery; //회복량
};

struct potionShop  //물약 종류와 수량
{
	potionType Ptype[4];
	int amountPotion;
};

struct statusHero//이름, 최대HP(레벨에 따라 증가), HP, 경험치, 레벨, 소유 골드
{
	int Pvalue;
	string name;
	int maxHp = 0;
	int attackPower = 40; // 공격력
	int currentHp =0;
	int currentMoney = 0;
	int currentExp = 0;
	int currentLevel =0 ;
};


struct statusMonster //이름, 최대HP, HP, 획득할 수 있는 경험치 / 골드
{
	string name;
	int maxHp;
	int attackPower; // 공격력
	int currentHp;
	int rootExp;
	int rootGold;
};

struct MONSTERTABLE
{
	statusMonster he[FIELDTYPE::wood+1];
	int count;
};

void StartuiEffect(void)
{
	cout << endl;
	cout << ". |" << endl;
	cout << "" << endl;
	cout << " ╲　　　　　　　　　　　╱" << endl;
	cout << " " << endl;
	cout << "/" << endl;
	cout << "" << endl;
	cout << " ╲　　　　　　 　　 ╱" << endl;
	cout << "" << endl;
	cout << " ╲　　  영웅은　　　╱" << endl;
	cout << "" << endl;
	cout << " - --   ---------　　 - --" << endl;
	cout << "" << endl;
	cout << " ╱　    절차적 ╲" << endl;
	cout << "" << endl;
	cout << " ╱　　 / ╲" << endl;
	cout << "" << endl;
	cout << " ╱　　　　　　　　 ╲ ." << endl;
	cout << "" << endl;
	cout << "/" << endl;
	cout << "" << endl;
	cout << "| ." << endl;
}
void MeetEnemy(void)
{
	system("cls");
	for (int i = 0; i < 10; i++)
		cout << endl;
	cout << "두둥" << endl;
	Sleep(700);
	system("cls");
	cout << "" << endl;
	cout << " ╲　　　　　　　　　　　╱" << endl;
	cout << " " << endl;
	cout << "/" << endl;
	cout << "" << endl;
	cout << " ╲　　　　　　 　　 ╱" << endl;
	cout << "" << endl;
	cout << " ╲　　   적을　　　╱" << endl;
	cout << "" << endl;
	cout << " - --   ---------　　 - --" << endl;
	cout << "" << endl;
	cout << " ╱　    만났다 ╲" << endl;
	cout << "" << endl;
	cout << " ╱　　 / ╲" << endl;
	cout << "" << endl;
	cout << " ╱　　　　　　　　 ╲ ." << endl;
	cout << "" << endl;
	cout << "/" << endl;
	cout << "" << endl;
	cout << "| ." << endl << endl << endl;
}



//맵 배열 생성 함수
void CreateMap(void)
{

}

int main() 
{
	statusHero ruleH;
	MONSTERTABLE monsterTable;
	srand(time(NULL));

	//몬스터타입 설정						이름  최대체력 공격력  현재체력   경험치         골드
	monsterTable.he[FIELDTYPE::wood] = { "슬라임",100,  20,		100,	 rand() % 100,	 rand() % 100 };
	monsterTable.he[FIELDTYPE::wood] = { "고블린",90,   30,		100,	 rand() % 100,	 rand() % 100 };
	monsterTable.he[FIELDTYPE::swamp] = { "주니어",100, 10,  	100,	 rand() % 100,	 rand() % 100 };
	monsterTable.he[FIELDTYPE::swamp] = { "발록",100,   40,	    100,	 rand() % 100,	 rand() % 100 };
	monsterTable.he[FIELDTYPE::field] = { "크림슨",100, 50,	    100,	 rand() % 100,	rand() % 100 };
	



	//상점
	potionShop Pshop;
	Pshop.Ptype[0].name = "RedPotion";
	Pshop.Ptype[0].price = 100;
	Pshop.Ptype[0].recovery = 100;

	Pshop.Ptype[1].name = "BluePotion";
	Pshop.Ptype[1].price = 30;
	Pshop.Ptype[1].recovery = 10;

	Pshop.Ptype[2].name = "YellowPotion";
	Pshop.Ptype[2].price = 40;
	Pshop.Ptype[2].recovery = 20;

	Pshop.Ptype[3].name = "GreenPotion";
	Pshop.Ptype[3].price = 50;
	Pshop.Ptype[3].recovery = 30;


	//시작UI
	StartuiEffect(); 
	
	//영웅설정	
	cout << "영웅 이름을 입력 해주세요(영어만) : ";
	cin >> ruleH.name;
	char inputPlayer; //움직임제어

	int playerX = 0;
	int PlayerY = 0;


	//난이도설정
	int diffLevel = 0;
	do
	{
		cout << "1. EASY 2. NORMAL 3. HARD : ";
		cin >> diffLevel;
		cout << endl;
		if (cin.fail())
		{
			cout << "비정상입력" << endl;
			cin.clear();
			cin.ignore(5, '\n');
		}
	} while (diffLevel > 3 || diffLevel <= 0);
	ruleH.currentHp /= diffLevel;


	//플레이어 체력, 돈
	ruleH.currentHp = 100 / diffLevel;

	//최대체력 받아놓기
	ruleH.maxHp = diffLevel * 100;
	int maxHp = ruleH.maxHp; 
 

	//레벨별로 달라지는 맵 생성
	int sizeMap = diffLevel * 10;

	TILE** MapPrr = new TILE*[sizeMap]; //정사각형으로 만들기떄문에


	//적의 숫자 설정
	int countEnemy = 0;

	for (int i = 0; i < sizeMap; i++)  //몬스터 5퍼센트의 확률로 맵에 무작위로 생성
	{
		MapPrr[i] = new TILE[sizeMap]; //맵안에 동적할당을 해준다.

		for (int j = 0; j < sizeMap; j++) //2차원배열안에 무작위로 몬스터 생성
		{
			MapPrr[i][j].monster = (rand() % 100 < (5 * diffLevel));  //레벨이 올라 갈수록 몬스터수가 많아지는 확률이 증가하게 한다. 1단계 10% 2단계 20% 3단계 30%

			if (MapPrr[i][j].player == true)							//만약 플레이어 위치에 몬스터가 생성되면 몬스터를 지운다.
				MapPrr[playerX][PlayerY].monster = false;		

			if (MapPrr[i][j].monster)
				countEnemy++;
		}
	}
	int moveMonster = 0;    //몬스터 움직임 카운트

	SetMap(MapPrr, sizeMap); //맵 설정해준다.


//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
	while (1)
	{
		int selcetShopdeon = 0; //던전입장 상점입장 선택초기화
		int selcetPostion = 0;  //포션선택



		if (moveMonster == 2)				//움직인 횟수가 2회가되면 몬스터가 상하좌우 랜덤으로 이동한다
		{
			moveMonster = 0;
			for (int i = 0; i < sizeMap; i++)
			{
				for (int j = 0; j < sizeMap; j++)
				{
					if (MapPrr[i][j].monster)
					{
						bool tfx = (rand() % 100) < 50;
						bool tfy = (rand() % 100) < 50;
						int tfxi, tfyi;
						if (tfy) tfyi = 1;
						else tfyi = -1;
						if (tfx) tfxi = 1;
						else tfxi = -1;																			//몬스터 위치가 겹치지않게 해준다
						if (0 < (i + tfyi) && (i + tfyi) < (sizeMap - 1))										//몬스터가 맵 밖으로 나가지않게한다
						{
							if (0 < (j + tfxi) && (j + tfxi) < (sizeMap - 1))
							{
								if (MapPrr[i + tfyi][j + tfxi].monster)
								{	
									continue;
								}
									MapPrr[i][j].monster = false;												//원래있던 몬스터 위치는 없애주고
									MapPrr[i + tfyi][j + tfxi].monster = true;
							}
						}
					}
				}
			}
		}

		system("cls");
		//상태정보창
		cout << "용사 이름 : " << ruleH.name << endl;
		cout << "현재 체력 : " << ruleH.currentHp << endl;
		cout << "현재 돈 : " << ruleH.currentMoney << endl;
		cout << "경험치 : " << ruleH.currentExp << endl;
		cout << "현재 레벨 : " << ruleH.currentLevel << endl;
		cout << "남은 몬스터 : " << countEnemy << endl;
		cout << MapPrr[PlayerY][playerX].fieldType << endl;
		for (int i = 0; i < 3; i++)
			cout << endl;

		PrintMap(MapPrr, sizeMap, playerX, PlayerY);    //맵출력
		cout << MapPrr[PlayerY][playerX].fieldType << endl;

		inputPlayer = _getch(); //플레이어 움직임 받기

        //플레이어 움직임 제어
		if (inputPlayer == 'w' && playerX > 0)
		{
			playerX--;
			moveMonster++;
		}
		else if (inputPlayer == 's' && playerX < (sizeMap -1))
		{
			playerX++;
			moveMonster++;
		}
		else if (inputPlayer == 'a' && PlayerY > 0)
		{
			PlayerY--;
			moveMonster++;
		}
		else if (inputPlayer == 'd' && PlayerY < (sizeMap -1))
		{
			PlayerY++;
			moveMonster++;
		}


		//몬스터 좌표와 플레이어 좌표가 일치할 떄 이벤트
		if (MapPrr[playerX][PlayerY].monster)
		{

			//몬스터타입 설정						이름  최대체력 공격력  현재체력   경험치         골드
			//monsterTable.he[FIELDTYPE::wood] = { "슬라임",100,  20,		100,	 rand() % 100,	 rand() % 100 };
			//monsterTable.he[FIELDTYPE::wood] = { "고블린",90,   30,		100,	 rand() % 100,	 rand() % 100 };
			//monsterTable.he[FIELDTYPE::swamp] = { "주니어",100, 10,  	100,	 rand() % 100,	 rand() % 100 };
			//monsterTable.he[FIELDTYPE::swamp] = { "발록",100,   40,	    100,	 rand() % 100,	 rand() % 100 };
			//monsterTable.he[FIELDTYPE::field] = { "크림슨",100, 50,	    100,	 rand() % 100,	rand() % 100 };


			bool victory = true;
			while (victory && !countEnemy == 0 && ruleH.currentHp > 0)
			{
				//가위바위보
				int rcpCom = 0;
				int rcpUser = 0;

				MeetEnemy(); //만났을떄 이펙트
				rcpCom = rand() % 3 + 1; //컴퓨터  123사이에서 랜덤숫자 입력
				do// 가위바위보 숫자내는데 1 , 2, 3안에서만 내도록 설정
				{
					cout << "**************************************************" << endl;
					cout << "용사 이름 : " << ruleH.name	  <<"                                       " << "몬스터 이름 : " << monsterTable.he[MapPrr[PlayerY][playerX].fieldType].name <<endl;
					cout << "현재 체력 : " << ruleH.currentHp << "                                      " << "몬스터 체력 :" << monsterTable.he[MapPrr[PlayerY][playerX].fieldType].currentHp <<endl;
					cout << "현재 돈 : " << ruleH.currentMoney << "                                         " << "몬스터 공격력"<< monsterTable.he[MapPrr[PlayerY][playerX].fieldType].attackPower<<endl;
					cout << endl;
					cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
					cout << "1. 가위 2. 바위 3. 보 : ";
					cin >> rcpUser;
					if (cin.fail())
					{
						cout << "비정상입력" << endl;
						cin.clear();
						cin.ignore(5, '\n');
					}
				} while (rcpUser >= 4 || rcpUser <= 0);


				if (rcpUser == rcpCom)
				{
					system("cls");
					cout << "==========================================================" << endl << endl;
					cout << "무승부입니다" << endl << endl;
					cout << "==========================================================" << endl;
					Sleep(700);
				}
				else if ((rcpUser > rcpCom || (rcpCom == 3 && rcpUser == 1)) && !(rcpUser - rcpCom == 2)) //컴퓨터가 3일떄 내가 1을 냈으면 승리 / 컴퓨터가 1일떄 내가 3을 내면 패배
				{
					system("cls");
					cout << "==========================================================" << endl << endl;
					cout << "승리입니다" << endl << endl;
					cout << "==========================================================" << endl;
					Sleep(700);
					monsterTable.he[MapPrr[PlayerY][playerX].fieldType].currentHp -= ruleH.attackPower;
					if (monsterTable.he[MapPrr[PlayerY][playerX].fieldType].currentHp <= 0) //만약 몬스터가 체력이 0보다 적어진다면
					{
						monsterTable.he[MapPrr[PlayerY][playerX].fieldType].currentHp -= 20;
						victory = false;
						countEnemy -= 1;						   //몬스터 카운트 -1
						MapPrr[playerX][PlayerY].monster = false;  //몬스터가 있던 자리를 false로 만들어줘서 다른 랜덤한 맵이 들어오도록 한다
						ruleH.currentMoney += monsterTable.he[MapPrr[PlayerY][playerX].fieldType].rootGold;		   //몬스터를 잡으면 해당하는 만큼 돈을 얻는다.

						do
						{
							cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;   //상점 선택칸
							cout << "상점에 갈려면 1번 던전에 갈려면 2번을 눌러주세요 : ";
							cin >> selcetShopdeon;
							if (cin.fail())
							{
								cout << "비정상입력" << endl;
								cin.clear();
								cin.ignore(5, '\n');
							}
						} while (selcetShopdeon >= 3 || selcetShopdeon <= 0);
					}



				}
				else if (rcpUser < rcpCom || (rcpCom == 1 && rcpUser == 3)) //컴퓨터가 1일떄 내가 3을 내면 패배
				{
					system("cls");
					cout << "==========================================================" << endl << endl;
					cout << "패배입니다" << endl << endl;
					cout << "==========================================================" << endl;
					Sleep(700);

					//플레이어 체력 감소
					ruleH.currentHp -= monsterTable.he[MapPrr[PlayerY][playerX].fieldType].attackPower;
				}
			}
			if (countEnemy == 0) //게임종료
			{
				system("cls");
				cout << "게임종료 승리입니다" << endl;
				break;
			}
				//게임종료
			if (ruleH.currentHp <= 0)
			{
				system("cls");
				cout << "게임종료 패배입니다" << endl;
				break;
			}
		}


		if (selcetShopdeon == 1)
		{
			system("cls");
			//상태정보창
			cout << "용사 이름 : " << ruleH.name << endl;
			cout << "현재 체력 : " << ruleH.currentHp << endl;
			cout << "현재 돈 : " << ruleH.currentMoney << endl;
			cout << "남은 숨겨진 몬스터 : " << countEnemy << endl;
			for (int i = 0; i < 5; i++)
				cout << endl;

			for (int i = 0; i < 4; i++) //포션선택 출력메세지
			{
				cout << i + 1 << "." << endl;
				cout << "포션이름 : " << Pshop.Ptype[i].name << endl;
				cout << "포션가격 : " << Pshop.Ptype[i].price << endl;
				cout << "포션회복량 : " << Pshop.Ptype[i].price << endl;
			}

			cout << "번호를 입력해주세요 : ";
			cin >> selcetPostion; //포션 선택하기
			if (cin.fail())
			{
				cout << "비정상입력" << endl;
				cin.clear();
				cin.ignore(5, '\n');
			}

			switch (selcetPostion) //애 함수로만들어주기 ***************************************************
			{
			case 1:
				if (Pshop.Ptype[selcetPostion-1].price <= ruleH.currentMoney && ruleH.currentHp < maxHp)
				{
					ruleH.currentHp += Pshop.Ptype[selcetPostion - 1].recovery;
					ruleH.currentMoney -= Pshop.Ptype[selcetPostion - 1].price;
					if (ruleH.currentHp >= maxHp) //만약 hp가 100이상일떄 포션을 쓰면 체력은 최대체력으로 맞춰주기
						ruleH.currentHp = maxHp;
					cout << "현재 남은 체력 : " << ruleH.currentHp << endl;
					cout << "현재 남은 돈 : " << ruleH.currentMoney << endl;			
				}
				else
				{
					cout << "이미 체력이 100 이상이거나 돈이 부족합니다" << endl;
					Sleep(700);
				}
				break;
			case 2:
				if (Pshop.Ptype[selcetPostion - 1].price <= ruleH.currentMoney && ruleH.currentHp < maxHp)
				{
					ruleH.currentHp += Pshop.Ptype[selcetPostion - 1].recovery;
					ruleH.currentMoney -= Pshop.Ptype[selcetPostion - 1].price;
					if (ruleH.currentHp >= maxHp) //만약 hp가 100이상일떄 포션을 쓰면 체력은 최대체력으로 맞춰주기
						ruleH.currentHp = maxHp;
					cout << "현재 남은 체력 : " << ruleH.currentHp << endl;
					cout << "현재 남은 돈 : " << ruleH.currentMoney << endl;				
				}
				else
				{
					cout << "이미 체력이 100 이상이거나 돈이 부족합니다" << endl;
					Sleep(700);
				}
				break;
			case 3:
				if (Pshop.Ptype[selcetPostion-1].price <= ruleH.currentMoney && ruleH.currentHp < maxHp)
				{
					ruleH.currentHp += Pshop.Ptype[selcetPostion - 1].recovery;
					ruleH.currentMoney -= Pshop.Ptype[selcetPostion - 1].price;
					if (ruleH.currentHp >= maxHp) //만약 hp가 100이상일떄 포션을 쓰면 체력은 최대체력으로 맞춰주기
						ruleH.currentHp = maxHp;
					cout << "현재 남은 체력 : " << ruleH.currentHp << endl;
					cout << "현재 남은 돈 : " << ruleH.currentMoney << endl;			
				}
				else
				{
					cout << "이미 체력이 100 이상이거나 돈이 부족합니다" << endl;
					Sleep(700);
				}
				break;
			case 4:
				if (Pshop.Ptype[selcetPostion - 1].price <= ruleH.currentMoney && ruleH.currentHp < maxHp)
				{
					ruleH.currentHp += Pshop.Ptype[selcetPostion - 1].recovery;
					ruleH.currentMoney -= Pshop.Ptype[selcetPostion - 1].price;
					if (ruleH.currentHp >= maxHp) //만약 hp가 100이상일떄 포션을 쓰면 체력은 최대체력으로 맞춰주기
						ruleH.currentHp = maxHp;
					cout << "현재 남은 체력 : " << ruleH.currentHp << endl;
					cout << "현재 남은 돈 : " << ruleH.currentMoney << endl;
				}
				else
				{
					cout << "이미 체력이 100 이상이거나 돈이 부족합니다" << endl;
					Sleep(700);
				}
				break;

			default:
			{
				cout << "잘못된 입력입니다" << endl;
				Sleep(700);
			}
			}
		}
		else if (selcetShopdeon == 2)
		{
			cout << "던전으로 입장합니다 " << endl;
			Sleep(700);
		}
	}
}

