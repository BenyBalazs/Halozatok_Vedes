#include <stdio.h>
#include <stdlib.h>
//Api és a funciók könyvtárai
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>




int main(int argc, const char *argv[]){
	
	if(argc != 2){
		printf("%s\n","Valami nem jó! Így futtasd: ./*nev* *EGY PORT*");
	}

	int port;
	port = atoi(argv[1]);
	
	int client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address, server;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection = connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	
	if (connect == -1){
		printf("%s\n","Szívás nincs kapcsolat" );
	}

	char messege[256];
	recv(client_socket, &messege, sizeof(messege), 0 );
	printf("%s\n",messege);


	int round;
	recv(client_socket, &round, sizeof(round), 0 );

	int money;
	recv(client_socket, &money, sizeof(money), 0 );


	printf("%s\n","A játék kezdetét veszi" );
	printf("%s%d\n","Ennyi kör lesz: ",round);
	printf("%s%d\n","Ennyi a kezdőpénz lesz: ",money);
	char messege2 [256] = "Megérkeztem várom a feladatot";
	send(client_socket, &messege2 , sizeof(messege2), 0);
	char Endgame = 's';
	char Noise = 'd';
	int Random_Number = 0;
	char brake = 'b';

//Végtelen ciklusba írjuk mert majd úgyis a szervertől kajuk a break barancsot.

	for(;;){

		int stop = 0;
		int bet_amount = 0;
		int bet_money_amount = 0;
		int bet = 0;
		printf("%s\n","Lehetőséged van 1, 2 vagy 3 DB zsetonnal való fogadásra (A mennyiséget add meg a billentyűzeten majd üss egy entert.)" );
		scanf("%d", &bet_amount);

		if(bet_amount == 1)
			bet = 1;
		if (bet_amount == 2)
			bet = 2;
		if (bet_amount == 3)
			bet = 3;

		send(client_socket, &bet , sizeof(bet), 0);

		printf("%s\n"," ");
		printf("%s\n","Várunk a másik játékosra" );
		printf("%s\n"," ");
		
		while (1){

			char i = 'A';
			recv(client_socket, &i, sizeof(i), 0 );
			// printf("%c\n",i );
			if (i == 'B')
				break;

		}

		int bet_numbers[bet_amount];

		for (int i = 0; i < bet; ++i)
		{
			printf("%s%d%s\n","0-9-ig tudsz fogadni a billentyűzeteddel ez az ", (i+1) ,". fogadásod" );	
			scanf("%d", &bet_numbers[i]);
		}

		send(client_socket, &bet_numbers , sizeof(bet_numbers), 0);

		printf("%s\n"," ");
		printf("%s\n","Várunk a másik játékosra" );
		printf("%s\n"," ");
		
		while (1){

			char i = 'A';
			recv(client_socket, &i, sizeof(i), 0 );
			// printf("%c\n",i );
			if (i == 'B')
				break;

		}

		printf("%s\n"," ");
		printf("%s%d%s\n","Eldöntheted, hogy a jelenlegi pénzedből: ",money, " Mennyit szeretnél felrakni" );
		scanf ("%d", &bet_money_amount);
		printf("%s%d\n","A téted: ",bet_money_amount );

		send(client_socket, &bet_money_amount , sizeof(bet_money_amount), 0);

		printf("%s\n"," ");
		printf("%s\n","Várunk a másik játékosra" );
		printf("%s\n"," ");
		
		while (1){

			char i = 'A';
			recv(client_socket, &i, sizeof(i), 0 );
			//printf("%c\n",i );
			if (i == 'B')
				break;

		}

		printf("%s\n"," ");
		printf("%s\n","Várunk az eredményre" );
		printf("%s\n"," ");

		char massage[256];
		recv(client_socket, &Random_Number, sizeof(Random_Number), 0 );
		recv(client_socket, &messege, sizeof(messege), 0 );
		recv(client_socket, &money, sizeof(money), 0 );

		printf("%s\n"," ");
		printf("%s%d\n","A rulett által kisorsolt szám a következő: ", Random_Number );
		printf("%s\n","A te számaid ");

		for (int i = 0; i < bet; ++i)
		{
			printf("%d\n",bet_numbers[i] );
		}

		printf("%s\n"," ");
		printf("%s%d\n","Megérkezett az eredmény az új pénzösszeg: ",money);
		printf("%s\n"," ");

		//Csekkoljuk, hogy mehet-e tovább a játék és, hogy ki vesztett nyert.
		recv ( client_socket, &Noise, sizeof(Noise), 0 );

		if (Noise == 'g'){
			recv ( client_socket, &Endgame, sizeof(Endgame), 0 );

			if (Endgame == 'A')
			{
				printf("%s\n","A játék véget ért az 1. játékos kifogyott a pénzből");
				break;
			}

			if (Endgame == 'B'){
				printf("%s\n","A játék véget ért a 2. játékos kifogyott a pénzből");
				break;

			}

			if (Endgame == 'C')
			{
				printf("%s\n","A játék véget ért nincs több kör");
				break;
			}
		}

		printf("%s\n"," ");
		printf("%s\n","Lehetőséged van megszakítani a játékot ha szeretnéd a megszakításhoz kis m betűt kell beírnod, minden egyéb esetben a játék folytatódik");
		scanf(" %c", &brake);
		printf("%s\n"," ");
		printf("%c\n", brake);
		send (client_socket, &brake, sizeof(brake), 0);
		recv (client_socket, &brake, sizeof(brake), 0);
		if (brake == 'm')
			break;
	}
	printf("%s\n"," " );
	printf("%s\n","A játék befejeződött a kliens most kilép" );
	close(client_socket);
	return 0;	
}