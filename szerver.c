#include <stdio.h>
#include <stdlib.h>
//Api és a funciók könyvtárai
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

int main(int argc, const char *argv[]){
	
	if(argc != 4){
		printf("%s\n","Valami nem jó! Így futtasd: ./*nev* *EGY PORT* *körök száma* *a játékosok kezdőösszege*");
		return -1;
	}
	
	int port;
	port = atoi(argv[1]);
 	
 	int round;
 	round = atoi(argv[2]);

 	int money;
 	money = atoi(argv[3]);

	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address, client;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	

	int palyer_count = 0;
	int length;
	int client_socet;
	int PlayerOne;
	int PlayerTwo;

	while(palyer_count != 2){

		if (listen(server_socket, 5) != 0){
			printf("%s\n","nincs catlakozás" );
		}
		else
			printf("%s\n","Szerver hallgat" );
		length = sizeof(client);

		client_socet = accept(server_socket, (struct sockaddr*)&server_address, &length);
		
		if (client_socet < 0){

			printf("%s\n","A szerver nem fogadta el a klienst" );
		}
		else if(palyer_count==0){

			printf("%s\n","A szerver elfagadta a klienst az 1." );
			char messege[256] = "Csatlakoztál te vagy az 1. játékos. Még várunk a második játékost";
			send(client_socet, messege , sizeof(messege),0);
			PlayerOne = client_socet;
			palyer_count++;
		}

		else if(palyer_count==1){

			printf("%s\n","A szerver elfagadta a klienst a 2." );
			char messege2 [256] = "Csatlakoztál te vagy az 2. játékos";
			send(client_socet, messege2 , sizeof(messege2),0);
			PlayerTwo = client_socet;
			palyer_count++;

		}
	}

	//Kiküldjük a Pénzt meg a játszmák számát a klienseknek.
	send(PlayerOne, &round , sizeof(round),0);
	send(PlayerOne, &money , sizeof(money),0);

	send(PlayerTwo, &round , sizeof(round),0);
	send(PlayerTwo, &money , sizeof(money),0);

	char teszt1[256];
	char teszt2[256];

	recv(PlayerOne, &teszt1, sizeof(teszt1), 0 );
	recv(PlayerTwo, &teszt2, sizeof(teszt2), 0 );

	printf("%s\n",teszt1 );
	printf("%s\n",teszt1 );


	int PlayerOne_money = money;
	int PlayerTwo_money = money;
	char PlayerOne_RanOutOfMoney = 'A';
	char PlayerTwo_RanOutOfMoney = 'B';
	char Endgame = 'C';
	char Noise = 'N';
	char PlayerOneWantsToLeave = 'n';
	char PlayerTwoWantsToLeave = 'n';


 	int brakepoint = 0; 
	char i = 'B';

	//Mennyi számra fogadtak.
	int PlayerOne_bet = 0;
	int PlayerTwo_bet = 0;

	for(;;){

		int PlayerOne_bet_money = 0; 
		int PlayerTwo_bet_money = 0;

		//Megkapjuk, hogy mennyi számra szeretnének fogadni a játéosok fogadni.
		recv(PlayerOne, &PlayerOne_bet, sizeof(PlayerOne_bet), 0 );
		printf("%s%d\n","Az 1. játékos fogadási mennyisége ", PlayerOne_bet);
		recv(PlayerTwo, &PlayerTwo_bet, sizeof(PlayerTwo_bet), 0 );
		//Kliens feloldása
		send(PlayerOne, &i , sizeof(i),0);
		send(PlayerTwo, &i , sizeof(i),0);
		printf("%s%d\n","A 2. játékos fogadási mennyisége ", PlayerTwo_bet);

		//megkapjuk, hogy mielyik számra fogadtak.
		int PlayerOne_bet_numbers[PlayerOne_bet];
		int PlayerTwo_bet_numbers[PlayerTwo_bet];

		recv(PlayerOne, &PlayerOne_bet_numbers, sizeof(PlayerOne_bet_numbers), 0 );
		recv(PlayerTwo, &PlayerTwo_bet_numbers, sizeof(PlayerTwo_bet_numbers), 0 );	

		printf("%s\n"," ");
		printf("%s\n","1. játékos számai");

		for (int i = 0; i < PlayerOne_bet; ++i)
		{
			printf("%d\n", PlayerOne_bet_numbers[i] );
		}

		printf("%s\n"," ");
		printf("%s\n","2. játékos számai");

		for (int i = 0; i < PlayerTwo_bet; ++i)
		{
			printf("%d\n",PlayerTwo_bet_numbers[i] );
		}

		printf("%s\n"," ");
		printf("%s\n","Várunk a feltett pénzekre");
		printf("%s\n"," ");

		//Kliens feloldása
		send(PlayerOne, &i , sizeof(i),0);
		send(PlayerTwo, &i , sizeof(i),0);

		//A játékosok feltett pénzének a megkapása.
		recv(PlayerOne, &PlayerOne_bet_money, sizeof(PlayerOne_bet_money), 0 );
		recv(PlayerTwo, &PlayerTwo_bet_money, sizeof(PlayerTwo_bet_money), 0 );

		//Kliens feloldása
		send(PlayerOne, &i , sizeof(i),0);
		send(PlayerTwo, &i , sizeof(i),0);

		//Random szám generálása a rulettnek
		srand(time(NULL));
		int random_number = rand() % 10;

		printf("%s\n"," ");
		printf("%s\n","Megérkeztek a feltett pénzek");
		printf("%s\n"," ");

		//Visszaküldjük a random számot a játékosoknak.
		send(PlayerOne, &random_number, sizeof(random_number), 0);
		send(PlayerTwo, &random_number, sizeof(random_number), 0);

		int PlayerOne_hit = 0;

		//PlayerOne nak megnézzük, hogy van e benne.
		for (int i = 0; i < PlayerOne_bet; ++i)
		{
			if (PlayerOne_bet_numbers[i] == random_number )
				PlayerOne_hit++; 
		}

		// Kismillió iffel eldöntjük, hogy mi is lessz a kimenet.
		if (PlayerOne_hit == 0){
			PlayerOne_money = PlayerOne_money - PlayerOne_bet_money;
			char massage[256] = "Sajnos nem találtad el egy számot se a tétedet elveszted";
			send(PlayerOne, &massage, sizeof(massage), 0 );
			send(PlayerOne, &PlayerOne_money, sizeof(PlayerOne_money), 0 );
		}

		if (PlayerOne_bet == 1 && PlayerOne_hit >= 1 ){
			PlayerOne_money = PlayerOne_money = (PlayerOne_bet_money * 6);
			char massage[256] = "Gratulálok Telitalálat a téted hatszorosát nyerted vissza";
			send(PlayerOne, &massage, sizeof(massage), 0 );
			send(PlayerOne, &PlayerOne_money, sizeof(PlayerOne_money), 0 );
		}

		if (PlayerOne_bet == 2 && PlayerOne_hit >= 1 ){
			PlayerOne_money = PlayerOne_money + (PlayerOne_bet_money * 3);
			char massage[256] = "Gratulálok legalább 1 számot eltaláltál a kettőből a téted háromszorosa visszajár";
			send(PlayerOne, &massage, sizeof(massage), 0 );
			send(PlayerOne, &PlayerOne_money, sizeof(PlayerOne_money), 0 );
		}

		if (PlayerOne_bet == 3 && PlayerOne_hit >= 1 ){
			PlayerOne_money = PlayerOne_money + (PlayerOne_bet_money * 2);
			char massage[256] = "Gratulálok legalább 1 számot eltaláltál a háromból a téted kétszerese visszajár";
			send(PlayerOne, &massage, sizeof(massage), 0 );
			send(PlayerOne, &PlayerOne_money, sizeof(PlayerOne_money), 0 );
		}


		int PlayerTwo_hit = 0;
		//PlayerOne nak megnézzük, hogy van e benne.
		for (int i = 0; i < PlayerTwo_bet; ++i)
		{
			if (PlayerTwo_bet_numbers[i] == random_number )
				PlayerTwo_hit++; 
		}

		// Kismillió iffel eldöntjük, hogy mi is lessz a kimenet.
		if (PlayerTwo_hit == 0){
			PlayerTwo_money = PlayerTwo_money - PlayerTwo_bet_money;
			char massage[256] = "Sajnos nem találtad el egy számot se a tétedet elveszted";
			send(PlayerTwo, &massage, sizeof(massage), 0 );
			send(PlayerTwo, &PlayerTwo_money, sizeof(PlayerTwo_money), 0 );
		}

		if (PlayerTwo_bet == 1 && PlayerTwo_hit >= 1 ){
			PlayerTwo_money = PlayerTwo_money = (PlayerTwo_bet_money * 6);
			char massage[256] = "Gratulálok Telitalálat a téted hatszorosát nyerted vissza";
			send(PlayerTwo, &massage, sizeof(massage), 0 );
			send(PlayerTwo, &PlayerOne_money, sizeof(PlayerTwo_money), 0 );
		}

		if (PlayerTwo_bet == 2 && PlayerTwo_hit >= 1 ){
			PlayerTwo_money = PlayerTwo_money + (PlayerTwo_bet_money * 3);
			char massage[256] = "Gratulálok legalább 1 számot eltaláltál a kettőből a téted háromszorosa visszajár";
			send(PlayerTwo, &massage, sizeof(massage), 0 );
			send(PlayerTwo, &PlayerTwo_money, sizeof(PlayerTwo_money), 0 );
		}

		if (PlayerTwo_bet == 3 && PlayerTwo_hit >= 1 ){
			PlayerTwo_money = PlayerTwo_money + (PlayerTwo_bet_money * 2);
			char massage[256] = "Gratulálok legalább 1 számot eltaláltál a háromból a téted kétszerese visszajár";
			send(PlayerTwo, &massage, sizeof(massage), 0 );
			send(PlayerTwo, &PlayerTwo_money, sizeof(PlayerTwo_money), 0 );
		}



		//növeljük a körszámlálót
		brakepoint++;

		if(PlayerOne_money <= 0){ //1. játékos kifogyott a pénzből.
			Noise = 'g';
			send(PlayerOne,&Noise, sizeof(Noise), 0);
			send(PlayerTwo,&Noise, sizeof(Noise), 0);
			send(PlayerOne, &PlayerOne_RanOutOfMoney , sizeof(PlayerOne_RanOutOfMoney),0);
			send(PlayerTwo, &PlayerOne_RanOutOfMoney , sizeof(PlayerOne_RanOutOfMoney),0);
			break;
		}

		else if(PlayerTwo_money <= 0){ //2. játékos kifogyott a pénzből.
			Noise = 'g';
			send(PlayerOne,&Noise, sizeof(Noise), 0);
			send(PlayerTwo,&Noise, sizeof(Noise), 0);
			send(PlayerOne, &PlayerTwo_RanOutOfMoney , sizeof(PlayerTwo_RanOutOfMoney),0);
			send(PlayerTwo, &PlayerTwo_RanOutOfMoney , sizeof(PlayerTwo_RanOutOfMoney),0);
			break;
		}
		else if (brakepoint == round){ //Vége vannak a köröknek.
			Noise = 'g';
			send(PlayerOne,&Noise, sizeof(Noise), 0);
			send(PlayerTwo,&Noise, sizeof(Noise), 0);
			send(PlayerOne, &Endgame , sizeof(Endgame),0);
			send(PlayerTwo, &Endgame , sizeof(Endgame),0);
			break;
		}
		else{

			send(PlayerOne,&Noise, sizeof(Noise), 0);
			send(PlayerTwo,&Noise, sizeof(Noise), 0);
		}

		//Ha valaki ki akar lépni azt itt döntjük el.
		recv( PlayerOne, &PlayerOneWantsToLeave, sizeof(PlayerOneWantsToLeave), 0 );
		recv( PlayerTwo, &PlayerTwoWantsToLeave, sizeof(PlayerTwoWantsToLeave), 0 );

		//Noise 2 azért küldjük, hogy ne álljon le a kliens.
		char Noise2 = 'z';

		if (PlayerOneWantsToLeave == 'm'){

			printf("%s\n", "Az 1. játékos kilépett" );
			send(PlayerTwo, &PlayerOneWantsToLeave, sizeof(PlayerOneWantsToLeave), 0);
			break;
		}

		else if (PlayerTwoWantsToLeave == 'm'){
			printf("%s\n", "A 2. játékos kilépett" );
			send(PlayerOne, &PlayerTwoWantsToLeave, sizeof(PlayerTwoWantsToLeave), 0);
			break;

		}
		else{
			send(PlayerOne, &Noise2, sizeof(Noise2), 0);
			send(PlayerTwo, &Noise2, sizeof(Noise2), 0);
		}
	}

	printf("%s\n","Vége a játéknak a szerver elbúcsúzik" );


	close(server_socket);
	return 0;
		}