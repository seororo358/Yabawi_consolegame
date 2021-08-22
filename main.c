#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>

#define TIME 130
#define TAX 1000

//���ļ��� �̿��� ���� ������ֱ����� define��
#define correct_1 261.6256
#define correct_2 329.6276
#define correct_3 391.9954
#define correct_4 523.2511

int temp_money;
int data;

//test github

typedef struct { //����� ����ü
	char *user;
	int money;
}USER;

void Remove_Cursor();    //Ŀ�� �����ִ� �Լ�
void game_running(USER user);   //���� ���� �Լ�
void mixing(int time, int cup[]);   //�� �����ִ� �Լ�
void put_ball();         //�� �ִ°� �����ִ� �Լ�
void cup_animation(int flag, int time);   //�� ���� ���� �ִϸ��̼�
void show_the_cup(int cups[]);     //���������� ���� �ִ� ����ġ�� �����ִ� �Լ�
USER set_game(char* name, int game_money);    //�÷��̾� ����ü�� ��ȯ���ִ� �Լ�
bool search_user(USER user, char* name);    //�÷��̾ �˻��ϴ� �Լ�
void record_print();      //1������ 5������ ������ ������ִ� �Լ�

int main(void) {
	
	char mode_select;   //���ӽ���, ����Top5, �������Ḧ ������ ����
	USER user;     //�÷��̾� ����ü
	char *name = (char*)malloc(sizeof(name));   //���ӽ��� �� �̸��� �ޱ����� �޸� �Ҵ�
	
	while (1) {
		system("cls");
		Remove_Cursor();     //Ŀ�� ����
		
		printf("\n\n\n\n");
		printf("      ��        ��        �ܡ�         �ܡܡܡܡܡ�          �ܡ�      ��                    ��    �ܡܡܡܡܡ�        \n");
		printf("       ��      ��        ��  ��        ��         ��        ��  ��      ��        ��        ��     ��                  \n");
		printf("        ��    ��        ��    ��       ��         ��       ��    ��      ��      �ܡ�      ��      ��                  \n");
		printf("         �ܡܡ�        �ܡܡܡܡ�      �ܡܡܡܡܡ�       �ܡܡܡܡ�      ��    ��  ��    ��       �ܡܡܡܡܡ�        \n");
		printf("           ��         ��        ��     ��         ��     ��        ��      ��  ��    ��  ��        ��                  \n");
		printf("           ��        ��          ��    ��         ��    ��          ��      �ܡ�      �ܡ�         ��                  \n");
		printf("           ��       ��            ��   �ܡܡܡܡܡ�    ��            ��      ��        ��          �ܡܡܡܡܡ�        \n");
		printf("                                                                                                                       \n");
		printf("                                                                                                                       \n");
		printf("                                                                                          press 'p' : ���ӽ���         \n");
		printf("                                                                                          press 'r' : ���ÿ� Top5      \n");
		printf("                                                                                          press 'q' : ��������         \n\n");
		
		mode_select = getch();   //���ĺ� �Է¹ޱ�(��� ����)
		
		if (mode_select == 'p') {
			system("cls");
			printf("User name : ");
			gets(name);    //�÷��̾� �̸� �ޱ�
			fflush(stdin);  //���ۺ���

			user = set_game(name, 0);
			if (!search_user(user, name)) {  //���� �÷��̾��̸��� ���������Ͽ��� �����ϴ� �Լ� ã���� true ����
				user = set_game(name, 10000);  //�÷��̾��̸��� ������ 10000���� �����ϰ� �÷��̾����� �ʱ�ȭ
				data = 0;
			}
			else {          //�÷��̾� �̸��� ������ ��, ���Ͽ� ����� ������ �÷������� �ʱ�ȭ
				user = set_game(name, temp_money); 
				data = 1;
			}
			free(name);    //�Ҵ�޾Ҵ� ���� ����
			game_running(user);    //����ü�� ���ڷ� ���޹޾� ���ӽ����Լ� ȣ��
			continue;  //�Լ��� ���������� �Ǹ� ������ �ֻ�� 40���ٺ��� �ٽ� main�Լ� ����
		}
		else if (mode_select == 'r') {
			record_print();    //'r' �Է½� ���� Top5 ���
			continue;     //�Լ��� ���������� �Ǹ� ������ �ֻ�� 40���ٺ��� �ٽ� main�Լ� ����
		}
		else if (mode_select == 'q')
			break;  //'q'�Է½� 40��°�� ������ ����������.
		else
			continue;   //�ٸ� Ű�� �Էµ� �� �ƹ� ������� �ʴ� ���� ȿ���� �ֱ����� ����ؼ� ������������ ����
	}
	return 0;
}

void record_print() {
	//������ ������ ������ �ҷ��� ���� ������, ������ ó�� ������ �� ��� �̾ƿñ��?
 		
	system("cls");
	FILE *fps;
	USER temp[5];
	
	int _temp=0;//������ �� ���ڸ� �ٲ��� ����
	char* _name = (char*)malloc(sizeof(_name));//������ �� �̸��� �ٲ��� ���� 
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
	//���ĵ� �� Ȯ���ϱ�
	
	printf("=========================== ���� Top 5 ==============================\n\n\n");
	for (int i = 0; i<5; i++)
		printf("%d %s %d\n", i+1, temp[i].user, temp[i].money);
	
	printf("\n\n=====================================================================\n");
	for(int i = 0;i<5;i++)
		free(temp[i].user);
	fclose(fps);

	printf("\n\n                                                         ���ư��� : b\n");
	while (1) {
		char key;
		key = getch();

		if (key == 'b')
			break;
	}
	return;
}

bool search_user(USER user, char* name) {
	/*������ fscanf�� ����ؼ� �����鼭 ã�Ƴ� ���ڿ��� main���� ���� �̸��� ���ϸ�
	�̸��� ������ true��, ������ ������ ������ false�� �����Ѵ�.*/

	FILE* fp;
	fp = fopen("user_and_money.txt", "r"); // ���� ����

	while (!feof(fp)) {     //������ ������ �д´�.
		fscanf(fp, "%s %d\n", user.user, &user.money);

		if (strcmp(name, user.user) == 0) { //���ڿ� ���Լ�
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
	strcpy(user.user, name); //name�� �ִ� ���ڿ��� ������ user.user�� ���δ�.

	return user;  //������ �ʱ�ȭ�� �÷��̾������� ��ȯ�Ѵ�.
}

void game_running(USER user) {
	/*�����Լ����� �������� ����ü�� �޾Ƽ� ���ӽ���*/

	srand(time(NULL)); //��������� ���� seed�� �����Լ�
	
	int money = user.money;
	int betting;
	int difficult;
	FILE* fp1;
	int answer;
	char* temp_name = (char*)malloc(sizeof(user.user));
	
	int temp = 0;
	
	while (1) {

		system("cls");
		printf("���̵��� �����Ͻÿ�. (EASY : 1, MIDDLE : 2, HARD : 3, CRAZY : 4) : ");
		scanf("%d", &difficult); //���̵��� ������ ����.

		if (difficult < 4 && money >= 10000 + 100000 * (difficult - 1)*(difficult - 1)) {
			/*���þ� å�����ִ� �������� ���� �������� ���� ���̵��� �����Ҽ� ���Բ� �������.*/

			printf("���� ���� �ʹ� �����ϴ�.");
			Sleep(2000);
			continue;
		}

		printf("���ñݾ��� �Է��Ͻÿ�(�����ڻ� : %d) : ", money);
		scanf("%d", &betting);  //���ñݾ� �Է�
		fflush(stdin);

		if (betting > money) {  //���þ׺��� �����ݾ��� ������� stop
			printf("�����ݾ� �̻� �����Ͻ� �� �����ϴ�.");
			Sleep(2000);
			continue;
		}
		else {  //��������� ��� �Ѱ�� ���� ����.
			int cup[3] = { 0,1,0 };

			printf("\n\n\n");
			printf("                              ���̵��� ���õǾ����ϴ�.                                 \n");
			if (difficult < 4)
				printf("                            ���̵� : %d  (������ : %d + ���þ�20%)                 \n", difficult, TAX * difficult);
			else
				printf("                            ���̵� : %d  (������ : %d)                             \n", difficult, TAX * difficult * 5);
			Sleep(2000);

			system("cls");
			put_ball(); //���־��ִ� �ִϸ��̼� �Լ�
			mixing(difficult, cup); // �ż��� �Լ�

			printf("���� �����ÿ� (���ʺ��� 1, 2, 3) : ");
			scanf("%d", &answer);  //�� �Է�
			fflush(stdin);

			show_the_cup(cup);

			if (cup[answer - 1] == 1) {
				/*���� �Է��ϰ� �ش簪�� �ε����� ������ �迭 �ε����� ���� 1�̸� ����*/
				Beep(correct_1, 200);
				Beep(correct_2, 200);
				Beep(correct_3, 200);
				Beep(correct_4, 200);

				printf("\n����!\n");
				Sleep(1500);

				if (difficult == 4) //���̵� �ֻ��� �� Ư�� ������
					money += betting * 10 - 5 * difficult*TAX;
				else
					money += (betting * difficult * 4 / 5) - difficult * TAX;
			}
			else {
				/*Ʋ���� �ÿ� ����Ǵ� �Լ�, �ݾ����� �� ��� �ݾ� ������ 1000�� ����*/
				
				Beep(correct_1, 500);
				printf("\n�ƽ����� Ʋ�Ƚ��ϴ�!\n");
				Sleep(1500);

				if (difficult == 4)
					money -= (betting + 5 * TAX * difficult);
				else
					money -= (betting + TAX * difficult);

				if (money <= 0) {
					/*���� ��� �����Ͽ��� ��� 1000�� ����*/

					printf("���� ��� �����Ͽ����ϴ�. +1000��\n");
					money = 1000;
					Sleep(1500);
				}
			}
		}

		printf("���ư��� : b \n");
		printf("����ϱ� : c \n");
		//���� ȭ������ ���ư��ų� ����ؼ� ������ �����ϰų�

		char key;
		while (1) {
			key = getch();

			if (key == 'b'||key == 'c')
				break;
			else
				continue;
		}
		if (key == 'b') //���߰� ���� ��������.
			break;
		else if (key == 'c') //����ϱ�
			continue;
	}

	user.money = money; //���ӿ��� ����ϴ� ���� ����ü�� �ٽ� �ʱ�ȭ
	char* buf = (char*)malloc(sizeof(buf));
	
	if (!data) {  
		/*����ȭ�鿡�� searchuser�Լ��� ȣ�������� ������������ ������ �ִ��� ��������
		 �����߱� ������ data�� ���� ���� fopen�� ��带 �������ش�.*/
		
		fp1 = fopen("user_and_money.txt", "a"); 
		/*����� ������ ���� �ÿ��� "a" �߰������� ������ ���� ���� �ǳ��� �̾ ���� ����*/

		if (fp1 == NULL)
			fprintf(stderr, "%s �� ���� �����ϴ�.\n", "user_and_money.txt");
		else 
			fprintf(fp1, "%s %d\n", user.user, user.money);
	}
	else {
		fp1 = fopen("user_and_money.txt", "r+");
		/*�а� ���⸦ �Բ��ؾ��ϹǷ� r+�� ������ ������.*/

		if (fp1 == NULL)
			fprintf(stderr, "%s �� ���� �����ϴ�.\n", "user_and_money.txt");
		else 
			while (!feof(fp1)) {
				fscanf(fp1, "%s %d\n", temp_name, &temp);
				
				if (!strcmp(temp_name, user.user)) {
					/*�ش� ���ڿ�(����ϴ� ���̵�)�� ã�� �� ���� ���ڿ� ��*/

					sprintf(buf, "%d", temp);
					/*���� �������� ���ڴ� ���ڿ��� �νĵǸ� ����� ���ڿ��� ��ġ�� ã�� ���� 
					temp�� ����� ���ڸ� ���ڿ��� �����ؼ� ���������� ���*/

					fseek(fp1, -(sizeof(temp_name)+sizeof(buf)+4), SEEK_CUR);
					/*fscanf�� ���� ���������� Ŀ���� �з����Ƿ� ����ġ���� ����� �����뷮��ŭ
					�ٽ� ���ư��� fprintf�� ������ �����.*/

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

void Remove_Cursor() // Ŀ�������� ����
{
	CONSOLE_CURSOR_INFO cur;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
	cur.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
}

void mixing(int difficult, int cups[]) {
	/*���̵��� ���� �迭�� ���ڷ� �޾Ƽ� �����ִ� �Լ�*/

	Remove_Cursor(); //����� ���������� Ŀ���� ��������
	srand(time(NULL));

	int rnd;
	int temp = 0;

	system("cls");

	for (int i = 0; i < 10 + difficult*difficult; i++) {
		rnd = rand() % 3; //������ ������ 3���� ���� ���̴� ��� 3������ �������
		
		switch (rnd) {
			/*���� ���϶����� cup_animation�Լ��� ������ �÷��̾ ����ġ�� Ȯ���� �� ����*/

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
	//ó�� ���� �־��ִ� �ִϸ��̼��� �������.

	Remove_Cursor();
	FILE* cup = fopen("Put_ball.dat", "rb");
	FILE* after = fopen("picture_original.dat", "rb");
	char buf[2048];
	int size;
	
	size = fread(buf, 1, 2048, cup); //���ۿ� 2048����Ʈ��ŭ �о ����� ��ȯ
	fwrite(buf, 1, size, stdout); //�����ŭ �о ���ۿ� ������ ǥ��������� ���
	Sleep(2000);
	system("cls");

	size = fread(buf, 1, 2048, after);
	fwrite(buf, 1, size, stdout);
	Sleep(2000);
	fclose(cup);
	fclose(after);
}

void cup_animation(int flag, int difficult) {
	/*�ִϸ��̼� ������ ���� *.dat������ ����� �ش������� �������°� �ƴϱ� ������
	 ���̳ʸ� �������·� ���� ������ �����ߴ�. ���� ������� ������ �ݺ��������ν� 
	 �ش� ���ϵ��� ��ġ ���ӵ� ����ó�� ���ư��� ȿ���� �̿��� �ִϸ��̼��� �����Ͽ���.*/

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
			//���� ������ 2���̹Ƿ� SleepŸ���� �������� ����
			system("cls");
		}

		for (int i = 0; i < 9; i++)
			fclose(fp[i]);
	}
	fclose(original);
}

void show_the_cup(int cups[]) {
	/*���� ����� �ÿ� ���� ���ִ� ���� ���÷� �����ִ� �Լ�.
	 �Ź迭�� ���ڷ� �޾� 1�� ���ִ� �Ÿ� ���ø���.*/

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