#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>

#define TIME 130
#define TAX 1000

//주파수를 이용해 음을 출력해주기위한 define문
#define correct_1 261.6256
#define correct_2 329.6276
#define correct_3 391.9954
#define correct_4 523.2511

int temp_money;
int data;

typedef struct { //사용자 구조체
	char *user;
	int money;
}USER;

void Remove_Cursor();    //커서 없애주는 함수
void game_running(USER user);   //게임 실행 함수
void mixing(int time, int cup[]);   //컵 섞어주는 함수
void put_ball();         //공 넣는거 보여주는 함수
void cup_animation(int flag, int time);   //컵 섞는 동작 애니메이션
void show_the_cup(int cups[]);     //최종적으로 공이 있는 컵위치를 보여주는 함수
USER set_game(char* name, int game_money);    //플레이어 구조체를 반환해주는 함수
bool search_user(USER user, char* name);    //플레이어를 검색하는 함수
void record_print();      //1위부터 5위까지 순위를 출력해주는 함수

int main(void) {
	
	char mode_select;   //게임시작, 순위Top5, 게임종료를 고르는 변수
	USER user;     //플레이어 구조체
	char *name = (char*)malloc(sizeof(name));   //게임시작 전 이름을 받기위해 메모리 할당
	
	while (1) {
		system("cls");
		Remove_Cursor();     //커서 제거
		
		printf("\n\n\n\n");
		printf("      ●        ●        ●●         ●●●●●●          ●●      ●                    ●    ●●●●●●        \n");
		printf("       ●      ●        ●  ●        ●         ●        ●  ●      ●        ●        ●     ●                  \n");
		printf("        ●    ●        ●    ●       ●         ●       ●    ●      ●      ●●      ●      ●                  \n");
		printf("         ●●●        ●●●●●      ●●●●●●       ●●●●●      ●    ●  ●    ●       ●●●●●●        \n");
		printf("           ●         ●        ●     ●         ●     ●        ●      ●  ●    ●  ●        ●                  \n");
		printf("           ●        ●          ●    ●         ●    ●          ●      ●●      ●●         ●                  \n");
		printf("           ●       ●            ●   ●●●●●●    ●            ●      ●        ●          ●●●●●●        \n");
		printf("                                                                                                                       \n");
		printf("                                                                                                                       \n");
		printf("                                                                                          press 'p' : 게임시작         \n");
		printf("                                                                                          press 'r' : 베팅왕 Top5      \n");
		printf("                                                                                          press 'q' : 게임종료         \n\n");
		
		mode_select = getch();   //알파벳 입력받기(출력 숨김)
		
		if (mode_select == 'p') {
			system("cls");
			printf("User name : ");
			gets(name);    //플레이어 이름 받기
			fflush(stdin);  //버퍼비우기

			user = set_game(name, 0);
			if (!search_user(user, name)) {  //받은 플레이어이름을 데이터파일에서 조사하는 함수 찾으면 true 리턴
				user = set_game(name, 10000);  //플레이어이름이 없을시 10000원을 지급하고 플레이어정보 초기화
				data = 0;
			}
			else {          //플레이어 이름이 존재할 때, 파일에 저장된 돈으로 플레이정보 초기화
				user = set_game(name, temp_money); 
				data = 1;
			}
			free(name);    //할당받았던 공간 해제
			game_running(user);    //구조체를 인자로 전달받아 게임실행함수 호출
			continue;  //함수를 빠져나오게 되면 루프의 최상단 40번줄부터 다시 main함수 시작
		}
		else if (mode_select == 'r') {
			record_print();    //'r' 입력시 순위 Top5 출력
			continue;     //함수를 빠져나오게 되면 루프의 최상단 40번줄부터 다시 main함수 시작
		}
		else if (mode_select == 'q')
			break;  //'q'입력시 40번째줄 루프를 빠져나간다.
		else
			continue;   //다른 키가 입력될 시 아무 실행되지 않는 듯한 효과를 주기위해 계속해서 루프시작으로 복귀
	}
	return 0;
}

void record_print() {
	//문제점 기존의 파일을 불러올 수는 있지만, 게임을 처음 시작할 때 어떻게 뽑아올까요?
 		
	system("cls");
	FILE *fps;
	USER temp[5];
	
	int _temp=0;//정렬할 때 숫자를 바꿔줄 공간
	char* _name = (char*)malloc(sizeof(_name));//정렬할 때 이름을 바꿔줄 공간 
	int new_money=0;
	char* new_user;

	for (int i = 0; i < 5; i++) {
		temp[i].money = 0;
		temp[i].user = (char*)malloc(temp[i].user);
	}

	fps = fopen("user_and_money.txt", "r+");
	
	while(!feof(fps)){
		new_user = (char*)malloc(sizeof(new_user));
		fscanf(fps, "%s %d\n", new_user, &new_money);

		for(int i = 0; i<5; i++)
			for (int j = 0; j < 5; j++) {
				if (new_money < temp[j].money)
					continue;
				else {
					_temp = temp[j].money;
					_name = temp[j].user;
					temp[j].money = new_money;
					temp[j].user = new_user;
					new_money = _temp;
					new_user = _name;
				}
			}
	}
	free(_name);
	printf("\n");
	//정렬된 값 확인하기
	
	printf("=========================== 순위 Top 5 ==============================\n\n\n");
	for (int i = 0; i<5; i++)
		printf("%d %s %d\n", i+1, temp[i].user, temp[i].money);
	
	printf("\n\n=====================================================================\n");
	for(int i = 0;i<5;i++)
		free(temp[i].user);
	fclose(fps);

	printf("\n\n                                                         돌아가기 : b\n");
	while (1) {
		char key;
		key = getch();

		if (key == 'b')
			break;
	}
	return;
}

bool search_user(USER user, char* name) {
	/*파일을 fscanf로 계속해서 읽으면서 찾아낸 문자열과 main에서 썻던 이름을 비교하며
	이름이 있으면 true를, 없으면 루프가 끝나고 false를 리턴한다.*/

	FILE* fp;
	fp = fopen("user_and_money.txt", "r"); // 파일 오픈

	while (!feof(fp)) {     //파일의 끝까지 읽는다.
		fscanf(fp, "%s %d\n", user.user, &user.money);

		if (strcmp(name, user.user) == 0) { //문자열 비교함수
			temp_money = user.money;
			fclose(fp);
			return true;
		}
		else
			continue;
	}
	fclose(fp);
	return false;
}

USER set_game(char* name, int game_money) {

	USER user;
	int money;
	user.user = (char*)malloc(sizeof(name));
	user.money = game_money;
	strcpy(user.user, name); //name에 있는 문자열을 복사해 user.user에 붙인다.

	return user;  //위에서 초기화된 플레이어정보를 반환한다.
}

void game_running(USER user) {
	/*메인함수에서 생성해준 구조체를 받아서 게임실행*/

	srand(time(NULL)); //난수출력을 위한 seed값 변형함수
	
	int money = user.money;
	int betting;
	int difficult;
	FILE* fp1;
	int answer;
	char* temp_name = (char*)malloc(sizeof(user.user));
	
	int temp = 0;
	
	while (1) {

		system("cls");
		printf("난이도를 선택하시오. (EASY : 1, MIDDLE : 2, HARD : 3, CRAZY : 4) : ");
		scanf("%d", &difficult); //난이도를 고르는 과정.

		if (difficult < 4 && money >= 10000 + 100000 * (difficult - 1)*(difficult - 1)) {
			/*배팅액 책정해주는 과정에서 돈이 많아지면 낮은 난이도를 선택할수 없게끔 만들어줌.*/

			printf("가진 돈이 너무 많습니다.");
			Sleep(2000);
			continue;
		}

		printf("베팅금액을 입력하시오(보유자산 : %d) : ", money);
		scanf("%d", &betting);  //배팅금액 입력
		fflush(stdin);

		if (betting > money) {  //배팅액보다 보유금액이 적을경우 stop
			printf("가진금액 이상 베팅하실 수 없습니다.");
			Sleep(2000);
			continue;
		}
		else {  //모든조건을 통과 한경우 섞기 시작.
			int cup[3] = { 0,1,0 };

			printf("\n\n\n");
			printf("                              난이도가 선택되었습니다.                                 \n");
			if (difficult < 4)
				printf("                            난이도 : %d  (수수료 : %d + 베팅액20%)                 \n", difficult, TAX * difficult);
			else
				printf("                            난이도 : %d  (수수료 : %d)                             \n", difficult, TAX * difficult * 5);
			Sleep(2000);

			system("cls");
			put_ball(); //공넣어주는 애니메이션 함수
			mixing(difficult, cup); // 컵섞는 함수

			printf("컵을 고르시오 (왼쪽부터 1, 2, 3) : ");
			scanf("%d", &answer);  //답 입력
			fflush(stdin);

			show_the_cup(cup);

			if (cup[answer - 1] == 1) {
				/*답을 입력하고 해당값을 인덱스로 대입해 배열 인덱스에 값이 1이면 정답*/
				Beep(correct_1, 200);
				Beep(correct_2, 200);
				Beep(correct_3, 200);
				Beep(correct_4, 200);

				printf("\n정답!\n");
				Sleep(1500);

				if (difficult == 4) //난이도 최상일 때 특수 수수료
					money += betting * 10 - 5 * difficult*TAX;
				else
					money += (betting * difficult * 4 / 5) - difficult * TAX;
			}
			else {
				/*틀렸을 시에 실행되는 함수, 금액차감 및 모든 금액 소진시 1000원 지급*/
				
				Beep(correct_1, 500);
				printf("\n아쉽지만 틀렸습니다!\n");
				Sleep(1500);

				if (difficult == 4)
					money -= (betting + 5 * TAX * difficult);
				else
					money -= (betting + TAX * difficult);

				if (money <= 0) {
					/*돈을 모두 소진하였을 경우 1000원 지급*/

					printf("돈을 모두 소진하였습니다. +1000원\n");
					money = 1000;
					Sleep(1500);
				}
			}
		}

		printf("돌아가기 : b \n");
		printf("계속하기 : c \n");
		//메인 화면으로 돌아가거나 계속해서 게임을 실행하거나

		char key;
		while (1) {
			key = getch();

			if (key == 'b'||key == 'c')
				break;
			else
				continue;
		}
		if (key == 'b') //멈추고 루프 빠져나옴.
			break;
		else if (key == 'c') //계속하기
			continue;
	}

	user.money = money; //게임에서 사용하던 돈을 구조체에 다시 초기화
	char* buf = (char*)malloc(sizeof(buf));
	
	if (!data) {  
		/*메인화면에서 searchuser함수를 호출했을시 전역변수에서 정보가 있는지 없는지를
		 저장했기 때문에 data의 값에 따라 fopen의 모드를 설정해준다.*/
		
		fp1 = fopen("user_and_money.txt", "a"); 
		/*사용자 정보가 없을 시에는 "a" 추가형으로 파일을 열고 파일 맨끝에 이어서 정보 저장*/

		if (fp1 == NULL)
			fprintf(stderr, "%s 를 열수 없습니다.\n", "user_and_money.txt");
		else 
			fprintf(fp1, "%s %d\n", user.user, user.money);
	}
	else {
		fp1 = fopen("user_and_money.txt", "r+");
		/*읽고 쓰기를 함께해야하므로 r+로 파일을 열었다.*/

		if (fp1 == NULL)
			fprintf(stderr, "%s 를 열수 없습니다.\n", "user_and_money.txt");
		else 
			while (!feof(fp1)) {
				fscanf(fp1, "%s %d\n", temp_name, &temp);
				
				if (!strcmp(temp_name, user.user)) {
					/*해당 문자열(사용하는 아이디)을 찾을 때 까지 문자열 비교*/

					sprintf(buf, "%d", temp);
					/*파일 내에서의 숫자는 문자열로 인식되며 저장된 문자열의 위치를 찾기 위해 
					temp에 저장된 숫자를 문자열로 변형해서 파일포인터 계산*/

					fseek(fp1, -(sizeof(temp_name)+sizeof(buf)+4), SEEK_CUR);
					/*fscanf로 인해 파일포인터 커서가 밀렸으므로 현위치에서 사용자 정보용량만큼
					다시 돌아가서 fprintf를 실행해 줘야함.*/

					fprintf(fp1, "%s %d\n" ,user.user, user.money);
					break;
				}
				else
					continue;
			}
	}
	fclose(fp1);
	
	return;
}

void Remove_Cursor() // 커서깜빡임 제거
{
	CONSOLE_CURSOR_INFO cur;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
	cur.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
}

void mixing(int difficult, int cups[]) {
	/*난이도와 컵의 배열을 인자로 받아서 섞어주는 함수*/

	Remove_Cursor(); //깔끔한 실행을위해 커서를 제거해줌
	srand(time(NULL));

	int rnd;
	int temp = 0;

	system("cls");

	for (int i = 0; i < 10 + difficult*difficult; i++) {
		rnd = rand() % 3; //난수를 생성해 3개의 컵이 섞이는 경우 3가지를 만들어줌
		
		switch (rnd) {
			/*컵이 섞일때마다 cup_animation함수를 실행해 플레이어가 컵위치를 확인할 수 있음*/

			case 0:
				temp = cups[0];
				cups[0] = cups[1];
				cups[1] = temp;
				cup_animation(rnd, difficult);
				break;
			case 1:
				temp = cups[1];
				cups[1] = cups[2];
				cups[2] = temp;
				cup_animation(rnd, difficult);
				break;
			case 2:
				temp = cups[0];
				cups[0] = cups[2];
				cups[2] = temp;
				cup_animation(rnd, difficult);
				break;
		}
	}

}

void put_ball() {
	//처음 컵을 넣어주는 애니메이션을 만들어줌.

	Remove_Cursor();
	FILE* cup = fopen("Put_ball.dat", "rb");
	FILE* after = fopen("picture_original.dat", "rb");
	char buf[2048];
	int size;
	
	size = fread(buf, 1, 2048, cup); //버퍼에 2048바이트만큼 읽어서 사이즈를 반환
	fwrite(buf, 1, size, stdout); //사이즈만큼 읽어서 버퍼에 내용을 표준출력으로 출력
	Sleep(2000);
	system("cls");

	size = fread(buf, 1, 2048, after);
	fwrite(buf, 1, size, stdout);
	Sleep(2000);
	fclose(cup);
	fclose(after);
}

void cup_animation(int flag, int difficult) {
	/*애니메이션 구현을 위해 *.dat파일을 만들고 해당파일이 문자형태가 아니기 때문에
	 바이너리 파일형태로 파일 오픈을 선언했다. 파일 입출력을 빠르게 반복해줌으로써 
	 해당 파일들이 마치 연속된 영상처럼 돌아가는 효과를 이용해 애니메이션을 구현하였다.*/

	FILE* original = fopen("picture_original.dat", "rb");
	char buffer[2048];
	int size;
	int size_1;

	size = fread(buffer, 1, 2048, original);
	fwrite(buffer, 1, size, stdout);
	Sleep(TIME - 30 * (difficult - 1));
	system("cls");

	if (flag == 0) {
		FILE* fp[5];

		fp[0] = fopen("mix0_1(1).dat", "rb");
		fp[1] = fopen("mix0_1(2).dat", "rb");
		fp[2] = fopen("mix0_1(3).dat", "rb");
		fp[3] = fopen("mix0_1(4).dat", "rb");
		fp[4] = fopen("mix0_1(5).dat", "rb");
				
		for (int i = 0; i < 5; i++) {
			size_1 = fread(buffer, 1, 2048, fp[i]);
			fwrite(buffer, 1, size_1, stdout);
			Sleep(TIME - 30 * (difficult - 1));
			system("cls");
		}

		for (int i = 0; i < 5; i++)
			fclose(fp[i]);
	}
	else if (flag == 1) {
		FILE* fp[5];

		fp[0] = fopen("mix1_2(1).dat", "rb");
		fp[1] = fopen("mix1_2(2).dat", "rb");
		fp[2] = fopen("mix1_2(3).dat", "rb");
		fp[3] = fopen("mix1_2(4).dat", "rb");
		fp[4] = fopen("mix1_2(5).dat", "rb");

		for (int i = 0; i < 5; i++) {
			size_1 = fread(buffer, 1, 2048, fp[i]);
			fwrite(buffer, 1, size_1, stdout);
			Sleep(TIME - 30 * (difficult - 1));
			system("cls");
		}

		for (int i = 0; i < 5; i++)
			fclose(fp[i]);
	}
	else if (flag == 2) {
		FILE* fp[9];

		fp[0] = fopen("mix0_2(1).dat", "rb");
		fp[1] = fopen("mix0_2(2).dat", "rb");
		fp[2] = fopen("mix0_2(3).dat", "rb");
		fp[3] = fopen("mix0_2(4).dat", "rb");
		fp[4] = fopen("mix0_2(5).dat", "rb");
		fp[5] = fopen("mix0_2(6).dat", "rb");
		fp[6] = fopen("mix0_2(7).dat", "rb");
		fp[7] = fopen("mix0_2(8).dat", "rb");
		fp[8] = fopen("mix0_2(9).dat", "rb");
		
		for (int i = 0; i < 9; i++) {
			size_1 = fread(buffer, 1, 2048, fp[i]);
			fwrite(buffer, 1, size_1, stdout);
			Sleep((TIME - 30 * (difficult - 1)) / 2);
			//파일 갯수가 2배이므로 Sleep타임을 절반으로 만듬
			system("cls");
		}

		for (int i = 0; i < 9; i++)
			fclose(fp[i]);
	}
	fclose(original);
}

void show_the_cup(int cups[]) {
	/*답을 골랐을 시에 공이 들어가있는 컵을 들어올려 보여주는 함수.
	 컵배열을 인자로 받아 1이 들어가있는 컵만 들어올린다.*/

	Remove_Cursor();
	FILE* show;
	char buffer[2048];
	int size = 0;

	if (cups[0]) 
		show = fopen("show_the_cups(1).dat", "rb");
		
	else if (cups[1]) 
		show = fopen("show_the_cups(2).dat", "rb");
	
	else 
		show = fopen("show_the_cups(3).dat", "rb");
		
	size = fread(buffer, 1, 2048, show);
	fwrite(buffer, 1, size, stdout);
	
	fclose(show);
}