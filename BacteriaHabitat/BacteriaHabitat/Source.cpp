#include <iostream>
#include <fstream>
#include <string>
#include<stdlib.h>
using namespace std;

const int INPUT_COUNT = 100;
string bacterias[10], rules[INPUT_COUNT][2], Bacs[INPUT_COUNT], bacMap[INPUT_COUNT][INPUT_COUNT];
int row, column, bacPoints[INPUT_COUNT][2];

void setBacterias(string rowS) { // Bakterileri tanimlayip, bakterilerin yazili oldugu satiri girdi olarak aliyor
	int z = 0; //Bakteri sayaci
	const char *cstr = rowS.c_str();
	for (int x = 0; x < strlen(cstr); x++) { // Her karakteri for döngüsüyle döndürüyorum
		if (cstr[x] == ' ') {
			continue; //Eger karakter bosluksa bir sonraki x degiskenine geçiyor
		}
		else
		{
			bacterias[z] = cstr[x]; //Bosluk degilse o karakteri bacterias dizisinde sakliyor
			z++; //Yeni bakteri için yer hazirliyor
		}
	}
}

int rc = 0; //Kural sayaci
void readRules(string rowS) { //Kurallari okuma- Girdi olarak aldigi satirdaki kurali okuyor- 
	int z = 0; //Avci ve av sayaci
	const char *cstr = rowS.c_str();
	for (int x = 0; x < strlen(cstr); x++) { //Her karakter döngüye alinir
		if (cstr[x] == ' ' || cstr[x] == '-') { // bosluk yada (-) gördügü zaman bir sonraki degere geçiyor
			continue;
		}
		else if (cstr[x] == '>') // > gördüðü zaman avci'nin avi belirtilecegi için z sayacini 1 yapiyor
		{
			z++;
		}
		else
		{
			rules[rc][z] = cstr[x]; // z sayacı ile rules dizisi içinde yeni bir kural için av ve avciyi kaydediyor
		}
	}
	rc++; //Yeni kurala yer açmak için kural sayaci artar
}

void setRules(string vars[], int rowC) { //Kurallari tanimla - Girdi olarak tüm satirlari ve kurallarin basladigi satiri alir-
	for (int x = rowC; x < INPUT_COUNT; x++) { //Her satir isleme sokulur
		if (vars[x] != "size:") { // satir size verisini tutmuyorsa readrules çagirilir, yeni kural kaydettirilir
			readRules(vars[x]);
		}
		else
		{
			return; // satir size verisini tutuyorsa islemi sonlandirir
		}
	}
}

void setSize(string rowS) { //Tablonun (Habitatin) sinirlari tanimlanir, girdi olarak boyutun belirtildigi satir alinir
	int t = 0; // t: satir sütun sayaci 0-> satir 1-> sütun
	string rowT, columnT; // geçici olarak satýr ve sütunun saklanacagi degiskenler
	const char *cstr = rowS.c_str();
	for (int x = 0; x < strlen(cstr); x++) { //Her karakter döngüye sokulur
		if (cstr[x] == ' ') { // karakter bosluksa yeni degere geçilir
			continue;
		}
		else if (cstr[x] == 'x' || cstr[x] == 'X') { // büyük yada küçük x ise satir sütun sayaci 1 olur, sütun moduna geçer
			t = 1;
		}
		else
		{
			switch (t)
			{
			case 0:
				rowT += cstr[x]; //t 0 ise satir modunda geçici satir deðiþkenine karakter eklenir
				break;
			case 1:
				columnT += cstr[x]; //t 1 ise sütun modunda geçici sütun degiskenine karakter eklenir
				break;
			default:
				break;
			} //+= kullanilir çünkü her karakter ayri ayri okunmakta, iki ve daha fazla haneli rakamlarda bu sayede tüm sayi alinir. Örn 10; 1 += 0 -> 10
		}
	}
	row = atoi(rowT.c_str()); //String degiskenleri const char yapilir ve atoi ile int'e çevrilir ve kalici olarak kaydedilir
	column = atoi(columnT.c_str());
}

char pointsRead(char c1, char c2) { // 2 tane char alıyor
	if (c2 != ' ') { //c2 boşluk değilse iki char'ı ard arda ekleyip çıktı veriyor
		return c1 + c2;
	}
	else //c2 boşluk ise c1'i döndürüyor
	{
		return c1;
	}
}

int pc = 0; //konum sayaci
void readPoints(string rowS) { //Konumlari tanimla -girdi olarak 1 satir alir-
	int z = 0; //mod sayaci 0 -> karakter 1 -> satir 2 -> sütun
	char tp;
	const char *cstr = rowS.c_str();
	for (int x = 0; x < strlen(cstr); x++) {
		if (cstr[x] == ' ') { //Bosluk varsa sayaç arttirilir
			z++;
		}
		else
		{
			switch (z)
			{
			case 0:
				Bacs[pc] = cstr[x]; //sayaç 0 ise Bacs dizisindeki yeni bir degiskene karakter kaydedilir
				break;
			case 1:
				tp = pointsRead(cstr[x], cstr[x + 1]); //pointsRead fonksiyonu ile iki chardan tek char üretilir
				bacPoints[pc][z - 1] = atoi(&tp); //sayaç 1 ise bacPoints içine satır numarası kaydedilir
				break;
			case 2:
				tp = pointsRead(cstr[x], cstr[x + 1]);
				bacPoints[pc][z - 1] = atoi(&tp); //sayaç 2 ise bacPoints içine sütun numarası kaydedilir
				break;
			default:
				break;
			}
		}
	}
	pc++; //yeni bakteri için yer açılır
}

void setStart(string vars[], int rowC) { //Başlangıç noktaları tanımlanır
	for (int x = rowC; x < INPUT_COUNT; x++) {
		if (vars[x] == "") { //Karakter tanımlanmadıysa fonksiyon bitirilir
			return;
		}
		else
		{
			readPoints(vars[x]); //Karakter tanımlıysa readPoints fonksiyonu ile konum tanımlanır
		}
	}
}

void setMap() { //bakteri haritası oluşturulur
	for (int i = 0; i < INPUT_COUNT; i++) {
		if (bacPoints[i][0] == 0 && bacPoints[i][1] == 0 && Bacs[i] == "") {  //satır ve sütun 0'a eşit ve bakteri yoksa sonraki değere geçilir. Geçilmezse 0'a 0 tanımsız kalır
			continue;
		}
		else
		{
			bacMap[bacPoints[i][0]][bacPoints[i][1]] = Bacs[i]; //haritaya bakteri harfi işlenir
		}
	}
}

void printMap() { //harita ekrana basılır
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) {
			cout << bacMap[x][y] << " "; //y satırındaki karakter ve bir boşluk bastırılır
		}
		cout << "\n"; //bir alt satıra geçilir
	}
}

string bacteriaFight(string bac1, string bac2) { //Bakteriler kavga eder
	for (int x = 0; x < INPUT_COUNT; x++) {
		if (rules[x][0] == bac1 && rules[x][1] == bac2) { //Kuralda avcı bac1 av bac2 ise galip olarak bac1 döndürülür
			return bac1;
		}
		else if (rules[x][0] == bac2 && rules[x][1] == bac1) //kuralda avcı bac2 av bac1 ise galip olarak bac2 döndürülür
		{
			return bac2;
		}
	}
	return bac1; //Diğer durumlarda bac1 galip olarak döndürülür
}

void oneMove() { //Bir tur simule edilir
	string tmpBacMap[INPUT_COUNT][INPUT_COUNT];
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) {
			tmpBacMap[x][y] = bacMap[x][y]; //Tur başlamadan önce bakteri habitatı geçici olarak kaydedilir
		}
	}
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) { //geçici habitat kaydındaki bakteriler bulunur, bulunan bakterilerin etrafındaki karaler kontrol edilir, boşsa o karaye o değişken atanır, dolu ise bakteriler savaştırılır ve kazanan o kareyi alır
			if (tmpBacMap[x][y] != "")
			{
				if (x - 1 >= 0 && bacMap[x - 1][y] == "") {
					bacMap[x - 1][y] = tmpBacMap[x][y];
				}
				else if (x - 1 >= 0)
				{
					bacMap[x - 1][y] = bacteriaFight(tmpBacMap[x][y], bacMap[x - 1][y]);
				}
				if (y - 1 >= 0 && bacMap[x][y - 1] == "") {
					bacMap[x][y - 1] = tmpBacMap[x][y];
				}
				else if (y - 1 >= 0)
				{
					bacMap[x][y - 1] = bacteriaFight(tmpBacMap[x][y], bacMap[x][y - 1]);
				}
				if (x + 1 <= row && bacMap[x + 1][y] == "") {
					bacMap[x + 1][y] = tmpBacMap[x][y];
				}
				else if (x + 1 <= row)
				{
					bacMap[x + 1][y] = bacteriaFight(tmpBacMap[x][y], bacMap[x + 1][y]);
				}
				if (y + 1 <= column && bacMap[x][y + 1] == "") {
					bacMap[x][y + 1] = tmpBacMap[x][y];
				}
				else if (y + 1 <= column)
				{
					bacMap[x][y + 1] = bacteriaFight(tmpBacMap[x][y], bacMap[x][y + 1]);
				}
			}
		}
	}
	printMap(); //Her adımdan sonra habitat yazdırılır
}

bool isFinished() { //Bir bakteri tüm kareleri kapladımı kontrolü yapılır
	string tmpChar = "";
	for (int x = 0; x < row; x++) {
		for (int y = 0; y < column; y++) { //Eğer tmpChar'a 0'a 0 kaydedilir, eğer diğer karelerde tmpChar'daki karakterden farklı bir karakter bulunursa false değişkeni, bulunmazsa true değişkeni döndürülür
			if (tmpChar == "") {
				tmpChar = bacMap[x][y];
			}
			else if (tmpChar != bacMap[x][y])
			{
				return false;
			}
		}
	}
	return true;
}

int main() {
	string  line[INPUT_COUNT]; //text'in aktarılacağı string dizisi, her satır bir element
	int i = 0;
	ifstream myfile("data.txt"); // dosyamızın input işlemleri için

	while (myfile)
	{
		getline(myfile, line[i]);
		cout << line[i] << "\n";
		i++;
	}
	myfile.close();

	for (int x = 0; x < INPUT_COUNT; x++) { //Text dosyası okunur, line arrayındeki değişkenler kontrol edilir, ilgili bölümler yakalanınca ilgili fonksiyonlar çalıştırılır
		if (line[x] == "" || x == 20) {
			continue;
		}
		else if (line[x] == "bacteria:") {
			setBacterias(line[x + 1]);
		}
		else if (line[x] == "rules:")
		{
			setRules(line, x + 1);
		}
		else if (line[x] == "size:")
		{
			setSize(line[x + 1]);
		}
		else if (line[x] == "start:")
		{
			setStart(line, x + 1);
		}
	}

	setMap(); //Harita yaratılır
	printMap(); //Harita basılır

	i = 0;
	while (!isFinished()) { //Bir bakteri kazanmadığı sürece her adım yazdırılır
		cout << "Turn : " << i << "\n";
		oneMove();
		cout << "\n";
		i++;
	}

	cout << "\n" << bacMap[0][0] << " has survived. Congratulations " << bacMap[0][0] << "\n"; //Kazanan tebrik edilir :)

	getchar();
	getchar();
	return 0;
}