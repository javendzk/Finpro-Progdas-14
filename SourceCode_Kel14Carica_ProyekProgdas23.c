/* Proyek akhir Pemrograman Dasar 02
   Kelompok 14 Carica (Vanessa, Javana, Ulil)
   Update terakhir: Senin, 25 Desember 2023 (15.27) */

// Memasukkan library yang ingin digunakan
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>

// Mendefinisikan nilai konstan yang nanti akan digunakan
#define PI 3.14f
#define MAX_INT 2147483645

// Inisiasi struct untuk database
typedef struct{
	float windVelocity;
	float airDensity;
	float totalPower;
} Condition;

typedef struct{
    int turbineType;
    char turbineLabel[50];
    float radiusOrDiameter;
    float turbineHeight;
    float efficientGenerator;
    float efficientMechanic;
    float turbinePower;
} Turbine;

// Function prototype untuk fungsi-fungsi 
void errorHandlingMenu(char errorRetry[100], char errorAlert[100], int *inputMenu, int batasBawah, int batasAtas);
void errorHandlingInput(char errorRetry[100], float *input);
void addTurbine(Turbine **turbines, Condition *conditions, int i);
void addTurbines(Turbine **turbines, Condition *conditions, int *counterTurbine);
void modifyPower(Condition *conditions);
void viewTurbine(Turbine turbines);
void viewDatabase(Turbine *turbines, Condition *conditions, int counterTurbine);
void modifyDatabase(Turbine *turbines, Condition *conditions, int *counterTurbine);
void viewSummary(Condition *conditions, Turbine *turbines, int counterTurbine);
void viewHelp();
void viewAbout();
void programExit(int *confirmExit);

int main(){
    int inputMenu, confirmExit = 0, counterTurbine = 0;
	char errorRetry[100], errorAlert[100];

	// Alokasi awal untuk database dynamic
    Turbine *turbines = (Turbine*)calloc(1, sizeof(Turbine));
    Condition *conditions = (Condition*)malloc(sizeof(Condition));

	// Error handling jika gagal alokasi
	if(turbines == NULL || conditions == NULL){
		printf("[!] Gagal mengalokasikan memori");
		return(1);
	}

	// Memberi state awal -1 untuk database conditions, artinya data tersebut belum diubah
    *conditions = (Condition){-1, -1, -1};

    do{
        printf("==================================================================\n"
        	   "\tAplikasi Simulasi Penggunaan Turbin Angin Rumahan\n"
        	   "\t [Kelompok 14 - Project Akhir Pemrograman Dasar]\n"
        	   "==================================================================\n"
           	   "| Fungsi Utama:\t\t\t\t\t\t\t |\n"
        	   "|   [1] Tambahkan Turbin Angin\t\t\t\t\t |\n"
        	   "|   [2] Atur/Ubah Daya rumah\t\t\t\t\t |\n"
        	   "|   [3] Tampilkan Database  \t\t\t\t\t |\n"
         	   "|   [4] Ubah & Kelola Database \t\t\t\t\t |\n"
        	   "|   [5] Ringkasan Output & Persentase \t\t\t\t |\n"
        	   "|  \t\t\t\t\t\t\t\t |\n"
        	   "| Lainnya:\t\t\t\t\t\t\t |\n"
        	   "|   [6] Panduan Penggunaan \t\t\t\t\t |\n"
        	   "|   [7] Tentang Program ini \t\t\t\t\t |\n"
        	   "|   [8] Keluar Program \t\t\t\t\t\t |\n"
        	   "==================================================================");

		// Input menu yang diinginkan beserta error handling
        strcpy(errorRetry, "\nMasukkan pilihan menu yang anda inginkan: ");
		strcpy(errorAlert, "\n[!] Input anda tidak sesuai menu");
    	errorHandlingMenu(errorRetry, errorAlert, &inputMenu,1,8);
        system("cls");

		// Memanggil fungsi sesuai menu yang diinput
        switch(inputMenu){
			case 1:
				addTurbines(&turbines, conditions, &counterTurbine);
				break;
			case 2:
				modifyPower(conditions);
				break;
			case 3:
				viewDatabase(turbines, conditions, counterTurbine);
				break;
			case 4:
				modifyDatabase(turbines, conditions, &counterTurbine);
				break;
			case 5:
				viewSummary(conditions, turbines, counterTurbine);
				break;
			case 6:
				viewHelp();
				break;
			case 7:	
				viewAbout();
				break;
			case 8:
				programExit(&confirmExit);
				break;	
		}
    } while(confirmExit != 1);
	// Jika confirmExit sudah = 1, maka proram berhenti looping

	// Membebaskan memori database
    free(turbines);
    free(conditions);
    return 0;
}

//Fungsi untuk error handling semua input menu
void errorHandlingMenu(char errorRetry[100], char errorAlert[100], int *inputMenu, int batasBawah, int batasAtas){
	int valid = 0;
    do{ 
        printf("%s", errorRetry);

		// Melakukan pengecekkan apaah inputnya int atau bukan, serta apakah melebihi batas atas atau bawahnya
        if(scanf("%d", inputMenu) != 1 || *inputMenu < batasBawah || *inputMenu > batasAtas){
			while(getchar() != '\n'); // Mendeteksi dan menghapus tipe data char apabila diinputkan hingga terdapat karater '\n'
            printf("%s", errorAlert);
        } else{
			valid = 1;
		}
    } while(*inputMenu < batasBawah || *inputMenu > batasAtas || valid == 0);
}

//Fungsi untuk error handling semua kolom input nilai
void errorHandlingInput(char errorRetry[100], float *input){
	int valid = 0;

    do{ 
        printf("%s",errorRetry);

		// Melakukan pengecekkan apaah inputnya float atau bukan serta memastikan float yang diinput lebih dari 0
        if(scanf("%f", input) != 1.00 || *input <= 0){
			while(getchar() != '\n'); // Mendeteksi dan menghapus tipe data char apabila diinputkan hingga terdapat karater '\n' atau
			printf("[!] Input tidak boleh huruf, minus, atau 0\n");
        } else{
			valid = 1;
		}
    } while(valid == 0);
}

void addTurbine(Turbine **turbines, Condition *conditions, int i){
	char errorRetry[100], errorAlert[100];
	float area;

	/* strcpy(variabel tempat menyimpan string, string yang ingin disimpan)
	   Variabelnya diteruskan ke errorHandlingMenu atau errorHandlingInput untuk ditampilkan
	   errorRetry berarti dialog untuk meminta kembali input. errorAlert jika input salah */

	printf("|Pilihan tipe turbin yang tersedia|"   
		   "\n   [1] Vertical Axis Wind Turbine (VAWT)"
    	   "\n   [2] Horizontal Axis Wind Turbine (HAWT)"
		   "\n\n|Input Turbin [%d]|", i + 1);

	// Input tipe turbin
	strcpy(errorRetry, "\n   Tipe turbin\t\t    : "); 
	strcpy(errorAlert, " [!] Input anda tidak sesuai index tipe");
	errorHandlingMenu(errorRetry, errorAlert, &(*turbines)[i].turbineType, 1, 2);

	// Input label turbin, bisa mengandung spasi
	printf("   Label turbin\t\t    : ");
	scanf(" %[^\n]s", (*turbines)[i].turbineLabel);

	// Jika tipe 1 (VAWT), maka meminta tinggi dan diameter
	if((*turbines)[i].turbineType == 1){
	    strcpy(errorRetry, "   Tinggi turbin (m)\t    : ");
	    errorHandlingInput(errorRetry, &(*turbines)[i].turbineHeight);

	    strcpy(errorRetry, "   Diameter turbin (m)\t    : ");
	    errorHandlingInput(errorRetry, &(*turbines)[i].radiusOrDiameter);

	// Jika tipe 2 (HAWT), maka meminta radius saja dan height di 0-kan
	} else{
	    strcpy(errorRetry, "   Radius turbin (m)\t    : ");
	    errorHandlingInput(errorRetry, &(*turbines)[i].radiusOrDiameter);
		
		(*turbines)[i].turbineHeight = 0;
	}

	//meminta input efisiensi generator dan mekanik
	strcpy(errorRetry, "   Efisiensi generator (%)  : ");
	errorHandlingInput(errorRetry, &(*turbines)[i].efficientGenerator);

	strcpy(errorRetry, "   Efisiensi mekanik (%)    : ");
	errorHandlingInput(errorRetry, &(*turbines)[i].efficientMechanic);

	// Rumus menghitung luas berdasarkan tipe turbin yang digunakan
	((*turbines)[i].turbineType  == 1) ? (area = (*turbines)[i].turbineHeight * (*turbines)[i].radiusOrDiameter) : (area = PI * (*turbines)[i].radiusOrDiameter);
	
	// Rumus menghitung power yang dihasilkan turbin berdasarkan data yang tersedia
	(*turbines)[i].turbinePower = 0.5 * conditions->airDensity * pow(conditions->windVelocity, 3) * area * ((*turbines)[i].efficientGenerator / 100) * ((*turbines)[i].efficientMechanic / 100);
}

void addTurbines(Turbine **turbines, Condition *conditions, int *counterTurbine){
	int i;
	float addTurbineAmount;
	char errorRetry[100], errorAlert[100];

    printf("[Menu 1: Tambahkan Turbin Angin]\n\n");

	// Jika belum input komponen lingkungan, diminta terlebih dahulu
	if(conditions->airDensity == -1 && conditions->windVelocity == -1){
		printf("[!] Harap input nilai komponen lingkungan\n");

		strcpy(errorRetry, "   Kecepatan angin (m/s)\t: ");
		errorHandlingInput(errorRetry, &conditions->windVelocity);

		strcpy(errorRetry, "   Kerapatan udara (kg/m^3)\t: ");
		errorHandlingInput(errorRetry, &conditions->airDensity);
	
	// Jika sebelumnya sudah input, maka ditampilkan
	} else{
		printf("|Komponen Lingkungan|");	
		printf("\n   Kecepatan angin: %.2f m/s", conditions->windVelocity);
		printf("\n   Kerapatan udara: %.2f kg/m^3\n", conditions->airDensity);
	}
	
	//Meminta jumlah turbin yang ingin ditambahkan
	strcpy(errorRetry, "\nMasukkan jumlah turbin : ");
	errorHandlingInput(errorRetry, &addTurbineAmount);
	
	// Realokasi peyimpanan *turbines dengan panjang (counter + tambahan) sebagai ukuran yang baru
	*turbines = realloc(*turbines, ((*counterTurbine + (int)addTurbineAmount) * sizeof(Turbine)));

    if(*turbines == NULL){
        printf("\n[!] Re-alokasi memori database gagal\n");
        exit(1);
    }
    system("cls");
    
	// Looping mengoper address *turbines dan condition untuk diinput
    for(i = *counterTurbine; i < (*counterTurbine + (int)addTurbineAmount); i++){
        printf("[Menu 1: Tambahkan Turbin Angin : %d Turbin]\n\n", (int)addTurbineAmount);
		addTurbine(&(*turbines), conditions, i);
		system("cls");
    }

	// Mengupdate nilai counterTurbine sesuai jumlah sekarang
	(*counterTurbine) += (int)addTurbineAmount;

	printf("[Menu 1: Tambahkan Turbin Angin]"
		   "\n\nData berhasil ditambahkan.\n");
	system("pause");
	system("cls");
}

void modifyPower(Condition *conditions){
	int inputMenu, temporary;
	char errorRetry[100], errorAlert[100];

	printf("[Menu 2: Atur/Ubah Daya Rumah]\n");
	
	// Pengecekkan jika belum pernah input daya, maka diminta input
	if(conditions->totalPower == -1){
		strcpy(errorRetry, "\nMasukkan besar daya rumah (watt): ");
		strcpy(errorAlert, "\n[!] Daya listrik minimum adalah 450 VA (Watt)");
    	errorHandlingMenu(errorRetry, errorAlert, &temporary, 450, MAX_INT);

    	conditions->totalPower = temporary;
		printf("\nDaya berhasil diatur.\n");
	
	// Jika sudah input daya, maka bisa mengubah atau tidak mengubah.
	} else{
		printf("\nBesar daya sudah diatur: %.2f VA (Watt)\n", conditions->totalPower);
		printf("Apakah anda ingin mengubah besar daya?\n   [0] Tidak\n   [1] Ya\n");

		strcpy(errorRetry, "\nMasukkan pilihan menu: ");
		strcpy(errorAlert, "\n[!] Input anda tidak sesuai opsi");
    	errorHandlingMenu(errorRetry, errorAlert, &inputMenu,0,1);

		// Jika 0 maka kembali ke menu, jika 1 meminta daya dan overwrite database
		switch(inputMenu){
			case 0:
				break;
			case 1:
				strcpy(errorRetry, "Masukkan besar daya rumah (watt): ");
				strcpy(errorAlert, "\n[!] Daya listrik minimum adalah 450 VA (Watt)\n");
    			errorHandlingMenu(errorRetry, errorAlert, &temporary, 450, MAX_INT);

    			// Memakai variabel temporary mencegah Error, jika sudah lolos error handling maka database di overwrite
    			conditions->totalPower = temporary;
    			printf("\nDaya berhasil diatur.\n");
    			break;
		}
	}	

	system("pause");
	system("cls");
}

void viewTurbine(Turbine turbines){        
		// Menampilkan database turbin sesuai indeks yang diminta
		printf("\n   Tipe Turbin\t\t: ");
        (turbines.turbineType == 1) ? ( printf("Vertical Axis (VAWT)")): (printf("Horizontal Axis (HAWT)"));
        printf("\n   Label Turbin\t\t: %s", turbines.turbineLabel);

		// Jika VAWT, ditampilkan tinggi dan diameter, jika HAWT radius
        if(turbines.turbineType == 1){
            printf("\n   Tinggi Turbin\t: %.2f m", turbines.turbineHeight);
            printf("\n   Diameter Turbin\t: %.2f m", turbines.radiusOrDiameter);
        } else{
            printf("\n   Radius Turbin\t: %.2f m", turbines.radiusOrDiameter);
        }

        printf("\n   Efisiensi Generator \t: %.2f %%", turbines.efficientGenerator);
        printf("\n   Efisiensi Mekanik\t: %.2f %%", turbines.efficientMechanic);
        printf("\n   Daya keluaran\t: %.2f Watt", turbines.turbinePower);
}

void viewDatabase(Turbine *turbines, Condition *conditions, int counterTurbine){
	int i;

	printf("[3. Menu Tampilkan Database]\n");

	// Jika database kosong (counter 0), tidak bisa ditampilkan
	if(counterTurbine == 0){
		printf("\n[!] Belum ada turbin yang ditambahkan.\n\n");
	} else{
		printf("\n|Nilai Komponen Lingkungan|");
		printf("\n  Kecepatan angin\t: %.2f m/s",conditions->windVelocity);
		printf("\n  Kerapatan udara\t: %.2f kg/m^3", conditions->airDensity);
		
		// Menampilkan semua database 1 per 1 sesuai indeks dengan viewTurbine
		for(i = 0; i < counterTurbine; i++){
			printf("\n\n|Display Turbin [%d]|", i+1);
			viewTurbine(turbines[i]);
		}
		printf("\n\n");
	}
	
	system("pause");
	system("cls");
}

void modifyDatabase(Turbine *turbines, Condition *conditions, int *counterTurbine){
    char errorRetry[100], errorAlert[100];
    int inputMenu, inputMenuConfirm, inputIndex, i;
	float area;

    printf("[Menu 4: Ubah & Kelola Database]\n");

	// Jika database kosong (counter 0), menu tidak ditampilkan
	if(*counterTurbine == 0){
		printf("\n[!] Belum ada turbin yang ditambahkan.\n\n");

	// Jika database tidak kosong, maka print menu
	} else{
		printf("\n|Pilihan Menu|"
			   "\n   [1] Edit Turbin Angin"
			   "\n   [2] Edit Komponen Lingkungan"
			   "\n   [3] Hapus Database Turbin\n");

		strcpy(errorRetry, "\nMasukkan pilihan menu yang Anda inginkan: ");
		strcpy(errorAlert, "[!] Input anda tidak sesuai menu\n");
		errorHandlingMenu(errorRetry, errorAlert, &inputMenu,1,3);
		system("cls");
		
		//Jika menu 1 dan 3, print semua database yang tersedia
		if(inputMenu == 1 || inputMenu == 3){
			printf("[Menu 4: Ubah & Kelola Database:");
			(inputMenu == 1) ? printf(" Edit Turbin]\n") : printf(" Hapus Turbin]\n");
			
			printf("\n|Database Tersedia|");
			for(i = 0; i < *counterTurbine; i++){
				printf("\n   [ %d : %s ]", i+1, turbines[i].turbineLabel);
			}
			printf("\n");
		}

		if(inputMenu == 1){
			strcpy(errorRetry, "\nInput index turbin : ");
			strcpy(errorAlert, "[!] Input anda tidak sesuai index turbin\n");
			errorHandlingMenu(errorRetry, errorAlert, &inputIndex, 1, *counterTurbine);
			system("cls");

			printf("[Menu 4: Ubah & Kelola Database: Edit Turbin]");
			
			// Membuat database sementara
			Turbine *temporary = (Turbine*)calloc(1, sizeof(Turbine));

			if(temporary == NULL){
				printf("[!] Gagal mengalokasikan memori");
				exit(1);
			}
			
			// Memperlihatkan database sebelumnya dengan viewTurbine
        	printf("\n\n|Display Turbin [%d]|", inputIndex);
			viewTurbine(turbines[inputIndex - 1]);
			printf("\n\n");
			
			// Meminta data baru dan dimasukkan ke temporary terlebih dahulu
			addTurbine(&temporary, conditions, inputIndex - 1);

			// Konfirmasi perubahan data
			printf("\nApakah anda yakin ingin merubah data?\n   [0] Tidak\n   [1] Ya\n");
			strcpy(errorRetry, "\nKonfirmasi perubahan : ");
			strcpy(errorAlert, "[!] Input anda tidak sesuai pilihan\n");
			errorHandlingMenu(errorRetry, errorAlert, &inputMenuConfirm, 0, 1);
			
			// Jika konfirmasi ubah, maka data di overwrite temporary
			if(inputMenuConfirm == 1){
				turbines[inputIndex-1] = temporary[0];
				printf("\nData turbin berhasil diperbarui\n");
			}

			free(temporary); 

		} else if(inputMenu == 2){
			printf("[Menu 4: Ubah & Kelola Database: Edit Lingkungan]"
				   "\n\n|Komponen Lingkungan Saat Ini|");
			printf("\n   Kecepatan angin\t    : %.2f m/s", conditions->windVelocity);
			printf("\n   Kerapatan udara\t    : %.2f kg/m^3", conditions->airDensity);

			// Membuat database sementara
			Condition *temporary = (Condition*)malloc(sizeof(Condition));

			if(temporary == NULL){
				printf("[!] Gagal mengalokasikan memori");
				exit(1);
			}

			// Meminta nilai baru dan disimpan ke temporary
			printf("\n\n|Input Nilai Baru|\n");
			strcpy(errorRetry, "   Kecepatan angin (m/s)    : ");
			errorHandlingInput(errorRetry, &temporary->windVelocity);

			strcpy(errorRetry, "   Kerapatan udara (kg/m^3) : ");
			errorHandlingInput(errorRetry, &temporary->airDensity);

			// Konfirmasi perubahan
			printf("\nApakah anda yakin ingin merubah komponen?\n   [0] Tidak\n   [1] Ya\n");
			strcpy(errorRetry, "\nKonfirmasi perubahan : ");
			strcpy(errorAlert, "[!] Input anda tidak sesuai pilihan\n");
			errorHandlingMenu(errorRetry, errorAlert, &inputMenuConfirm, 0, 1);
			
			// Jika konfirmasi ubah, maka database di overwrite
			// Semua daya turbin juga dihitung kembali dengan nilai lingkungan baru
			if(inputMenuConfirm == 1){
				conditions->airDensity = temporary->airDensity;
				conditions->windVelocity = temporary->windVelocity;

				printf("\nKomponen lingkungan berhasil diperbarui\n");

				for(i = 0; i < *counterTurbine; i++){
					(turbines[i].turbineType  == 1) ? (area = turbines[i].turbineHeight * turbines[i].radiusOrDiameter) : (area = PI * turbines[i].radiusOrDiameter);
		    		
					turbines[i].turbinePower = 0.5 * conditions->airDensity * pow(conditions->windVelocity,3) * area * (turbines[i].efficientGenerator/100) * (turbines[i].efficientMechanic/100);
				}
			}
		
			free(temporary);

		} else{
    		strcpy(errorRetry, "\nInput index turbin : ");
    		strcpy(errorAlert, "[!] Input anda tidak sesuai index\n");
    		errorHandlingMenu(errorRetry, errorAlert, &inputIndex, 1, *counterTurbine);
    		system("cls");

			printf("[Menu 4: Ubah & Kelola Database: Edit Turbin]");
			
			// Memperlihatkan indeks database yang ingin dihapus
    		printf("\n\n|Display Turbin [%d]|", inputIndex);
    		viewTurbine(turbines[inputIndex - 1]);
			
    		printf("\n\nApakah anda yakin ingin menghapus?\n   [0] Tidak\n   [1] Ya\n");
			strcpy(errorRetry, "\nKonfirmasi penghapusan : ");
			strcpy(errorAlert, "[!] Input anda tidak sesuai pilihan\n");
    		errorHandlingMenu(errorRetry, errorAlert, &inputMenuConfirm, 0, 1);

			// Jika konfirmasi hapus, indeks tersebut dihapus
			// Indeks-indeks setelah yang dihapus naik 1 angka
    		if(inputMenuConfirm == 1){ 
    		    for(i = (inputIndex - 1); i < *counterTurbine - 1; i++){
    		        turbines[i] = turbines[i + 1]; 
    		    }

        		(*counterTurbine)--;

				// Jika setelah hapus counter jadi 0, maka calloc 1 agar tidak error
        		if(*counterTurbine == 0){
					turbines = (Turbine*)calloc(1, sizeof(Turbine));
				}

				// Jika setelah hapus counter tidak 0, maka realloc sesuai counter 
        		turbines = realloc(turbines, *counterTurbine * sizeof(Turbine));

        		printf("\nData turbin berhasil dihapus\n");
    		}
		}
	}
	
	system("pause");
	system("cls");
} 

void viewSummary(Condition *conditions, Turbine *turbines, int counterTurbine){
	int inputMenu, i;
	float percentTurbine, percentGrid, sumTurbinePower = 0;

	printf("[Menu 5: Ringkasan Output & Persentase]");

	// Menampilkan daya rumah dan komponen lingkungan
	// Jika dicek != -1 dan false, berarti belum berubah/diinput maka print NULL
	printf("\n\n|Status Daya Rumah & Komponen Lingkungan|");
	printf("\n   Daya listrik rumah\t\t: ");
	(conditions->totalPower != -1) ? printf("%.2f VA (Watt)", conditions->totalPower) : printf("NULL");

	printf("\n   Kecepatan Angin\t\t: ");
 	(conditions->windVelocity != -1) ? printf("%.2f m/s", conditions->windVelocity) : printf("NULL");

	printf("\n   Kerapatan Udara\t\t: ");
 	(conditions->airDensity != -1) ? printf("%.2f kg/m^3", conditions->airDensity) : printf("NULL");
	 	
	// Menampilkan power setiap turbin, jika counter tidak 0 maka ditampilkan
	printf("\n\n|Status Daya Turbin Angin|");
	if(counterTurbine != 0){
		for(i = 0; i < counterTurbine; i++){
			printf("\n   Daya turbin ke-%d\t\t: %.2f Watt", i+1, turbines[i].turbinePower); 
			sumTurbinePower += turbines[i].turbinePower; 
			// Menambakan semua power dari turbin yang ada ke variabel total
		}
		printf("\n   Total daya turbin\t\t: %.2f Watt", sumTurbinePower);
	} else{
		// Jika counter 0, berarti belum ada turbin maka print error
		printf("\n   [!] Belum ada turbin yang ditambahkan.");
	}

	// Jika belum ada turbin dan daya belum di set, maka tidak menampilkan ringkasan
	printf("\n\n|Ringkasan|");
	if(counterTurbine != 0 && conditions->totalPower != -1){
		if(conditions->totalPower > sumTurbinePower){
			percentTurbine = (sumTurbinePower/conditions->totalPower) * 100;
			percentGrid = 100 - percentTurbine;
		} else{
			printf("\n   Daya turbin overflow\t\t: %.2f Watt", (sumTurbinePower - conditions->totalPower));
			percentTurbine = 100;
			percentGrid = 0;
		} // Jika total daya turbin > daya rumah, maka daya rumah 0% dan menampilkan overflow

		printf("\n   Daya PLN tergantikan turbin\t: %.1f %%", percentTurbine);
		printf("\n   Sisa daya PLN digunakan\t: %.1f %%\n", percentGrid);

		// Membuat histogram dengan skala 1:4 menggunakan karakter kotak (223)
		printf("\n   (turbin)\t");
		for(i = 0; i < (int)percentTurbine/4; i++){
			printf("%c ", 223);
			Sleep(35);
		}
	
		printf("\n   (PLN)\t");
		for(i = 0; i < (int)percentGrid/4; i++){
			printf("%c ", 223);
			Sleep(35);
		}
			
		printf("\n\n");
	} else{
		printf("\n   [!] Data belum lengkap, tidak dapat menampilkan ringkasan.\n\n");
	}
	
	system("pause");
	system("cls");
}

void viewHelp(){
	int inputMenu;
	char errorRetry[100], errorAlert[100];

	// Menyimpan seluruh data panduan di array instructionData
	char instructionData[10][750] = {
		"\n|Panduan menambahkan turbin angin|"
		"\n   1. Pada menu utama, input angka 1"
		"\n   2. Jika ini turbin pertama, input komponen lingkungan dahulu"
		"\n   3. Input jumlah turbin yang ingin ditambahkan"
		"\n   4. Pilih tipe turbin. Masukkan 1 untuk VAWT, 2 untuk HAWT"
		"\n   5. Pada tiap turbin, masukkan nilai-nilai yang diminta oleh program"
		"\n   6. Jangan lupa bahwa pada HAWT & VAWT input yang diminta berbeda"
		"\n   7. Setelah input berhasil, program akan kembali ke menu utama",

		"\n|Panduan mengatur/mengubah besar daya rumah|"
		"\n   1. Pada menu utama, input angka 2"
		"\n   2. Apabila anda sudah mengatur daya sebelumnya, input angka 1"
		"\n   3. Input besar daya rumah dalam satuan watt"
		"\n   4. Daya minimal adalah 450 watt, sesuai dengan ketentuan PLN"
		"\n   5. Tekan enter untuk kembali ke menu utama",

		"\n|Panduan menampilkan seluruh database turbin|"
		"\n   1. Pada menu utama, input angka 3"
		"\n   2. Program akan secara otomatis menampilkan seluruh database"
		"\n   3. Data ditampilkan mulai dari kecepatan angin, hingga rincian tiap turbin"
		"\n   4. Scroll apabila anda tidak menemukan data turbin yang dicari"
		"\n   5. Tekan enter untuk kembali ke menu utama",

		"\n|Panduan mengubah data suatu turbin|"
		"\n   1. Pada menu utama, input angka 4"
		"\n   2. Input angka 1 pada menu yang ditampilkan"
		"\n   3. Program akan menyediakan index dan label turbin yang dapat diubah"
		"\n   4. Input index turbin yang ingin diubah"
		"\n   5. Program akan menampilkan nilai-nilai turbin tersebut"
		"\n   6. Input nilai-nilai baru yang ingin digunakan"
		"\n   7. Apabila tidak ingin merubah suatu komponen, input nilai yang sama"
		"\n   8. Konfirmasi perubahan dengan input 1",
		
		"\n|Panduan mengubah data komponen lingkungan|"
		"\n   1. Pada menu utama, input angka 4"
		"\n   2. Input angka 2 pada menu yang ditampilkan"
		"\n   3. Program akan menampilkan nilai kecepatan angin dan kerapatan udara"
		"\n   4. Input nilai baru yang ingin digunakan"
		"\n   5. Apabila tidak ingin merubah suatu komponen, input nilai yang sama"
		"\n   6. Konfirmasi perubahan dengan input 1",
		
		"\n|Panduan menghapus data suatu turbin|"
		"\n   1. Pada menu utama, input angka 4"
		"\n   2. Input angka 3 pada menu yang ditampilkan"
		"\n   3. Program akan menyediakan index dan label turbin yang dapat dihapus"
		"\n   4. Input index turbin yang ingin dihapus"
		"\n   5. Konfirmasi penghapusan dengan input 1",
		
		"\n|Panduan menampilkan ringkasan dan kalkulasi akhir|"
		"\n   1. Pada menu utama, input angka 5"
		"\n   2. Program akan menampilkan status daya, komponen lingkungan, dan turbin"
		"\n   3. Apabila anda belum menginput daya/turbin, maka ringkasan tidak ditampilkan"
		"\n   4. Jika sudah, maka persentase kalkulasi dan histogram dapat dilihat"
		"\n   5. Tekan enter untuk kembali ke menu utama",
		
		"\n|Panduan melihat deskripsi program|"
	    "\n   1. Pada menu utama, input angka 7"
	    "\n   2. Input menu 1 untuk penjelasan turbin, dan 2 untuk deskripsi pembuat"
	    "\n   3. Program akan menampilkan errorRetry sesuai dengan input"
	    "\n   4. Tekan enter untuk kembali ke menu utama",

		"\n|Panduan keluar dari program|"
		"\n   1. Pada menu utama, input angka 8"
		"\n   2. Jika anda ingin batal keluar program, input 0"
		"\n   3. Jika ingin tetap keluar dari program, input 1"
		"\n   4. Program akan melakukan terminasi",

		"\n|Pedoman Besaran & Nilai|"
		"\n   - Rerata kerapatan udara = 1.222 kg/m^3"
		"\n   - Efisiensi Mekanik = 80%% ~ 90%%"
		"\n   - Efisiensi Generator = 80%% ~ 90%%"
		"\n   - Jika kecepatan udara 5 m/s = 18, berlaku kelipatan"
	};

	// Menampilkan menu panduan 
	printf("[Menu 6: Panduan Penggunaan]\n");
	printf("\n|Menu Panduan Tersedia|"
		   "\n   [1] Panduan menambahkan turbin angin"
		   "\n   [2] Panduan mengatur/mengubah besar daya rumah"
		   "\n   [3] Panduan menampilkan seluruh database turbin"
		   "\n   [4] Panduan mengubah data suatu turbin"
		   "\n   [5] Panduan mengubah data komponen lingkungan"
		   "\n   [6] Panduan menghapus data suatu turbin"
		   "\n   [7] Panduan menampilkan ringkasan dan kalkulasi akhir"
		   "\n   [8] Panduan melihat deskripsi program"
		   "\n   [9] Panduan keluar dari program"
		   "\n   [10] Pedoman Besaran & Nilai\n");

	strcpy(errorRetry, "\nMasukkan pilihan menu: ");
	strcpy(errorAlert, "\n[!] Input anda tidak sesuai menu");
    errorHandlingMenu(errorRetry, errorAlert, &inputMenu, 1, 10);
	system("cls"); 

	// Menampilkan artikel panduan sesuai pilihan menu
	printf("[Menu 6: Panduan Penggunaan: %d]\n", inputMenu);
	printf("%s\n", instructionData[inputMenu-1]);

	printf("\n\n");
	system("pause");
	system("cls");
}

void viewAbout(){
	int inputMenu;
	char errorRetry[100], errorAlert[100];

	// Menampilkan menu about
	printf("[Menu 7: Tentang Program Ini]\n");
	printf("\n|Menu Artikel Tersedia|"
		   "\n   [1] Tentang turbin angin"
		   "\n   [2] Tentang kami\n");

	strcpy(errorRetry, "\nMasukkan pilihan menu: ");
	strcpy(errorAlert, "\n[!] Input anda tidak sesuai menu");
    errorHandlingMenu(errorRetry, errorAlert, &inputMenu, 1, 2);
	system("cls");

	// Menampilken artikel about sesuai menu yang dipilih
	switch(inputMenu){
		case 1:
			printf("[Menu 7: Tentang Program Ini: Turbin angin]\n\n");
			printf("Tipe turbin angin dapat dikelompokkan menjadi 2, yaitu yang berporos horizontal (HAWT)"
				   "\ndan yang berporos vertikal (VAWT). Meskipun keduanya menghasilkan listrik dari tenaga"
				   "\nangin, terdapat perbedaan mencolok. VAWT memiliki keluaran daya yang lebih efisien"
				   "\ndibandingkan HAWT. Namun karena pemeliharaannya yang lebih memakan biaya dan tidak"
				   "\nadaptif, maka tipe HAWT adalah yang paling sering digunakan. Contohnya terdapat pada"
				   "\nlantai atas PUSGIWA. Berikut adalah metode umum untuk menghitung daya yang dihasilkan.\n\n");	
			printf("P (Watt) = 0.5 * Rho.udara * V.angin^3 * A * N.gen * N.gear"
				   "\nA (VAWT) = D * H"
				   "\nA (HAWT) = 3.14 * L^2\n\n");
			printf("Keterangan:"
				   "\n - Rho.udara	= Kerapatan udara pada lokasi. (Umumnya 1.225 Kg/m^3)"
				   "\n - V.angin	= Kecepatan angin lokasi. (m/s)"
				   "\n - A		= Luas (m^2)"
				   "\n - N.gen	= Efisiensi generator listrik (%%)"
				   "\n - N.gear	= Efisiensi gearbox turbin (%%)"
				   "\n - D		= Diameter turbin (m)"
				   "\n - H		= TInggi turbin (m)"
				   "\n - L		= Panjang bilah turbin (m)\n\n");	
			break;
		case 2:
			printf("[Menu 7: Tentang Program Ini: Kami]\n\n");
			printf("Halo! perkenalkan kami dari kelompok 14 Pemrograman Dasar 02 program studi Teknik Komputer" 
				   "\nFakultas Teknik Universitas Indonesia periode 2023/2024. Kelompok kami terdiri atas"
				   "\nVanessa Kayla Azzahra, Javana Muhammad Dzaki dan Nugroho Ulil Abshar.\n");
			printf("\nSeperti yang kita ketahui, permasalahan energi yang bersih dan terjangkau menjadi topik" 
				   "\nyang hangat belakangan ini. Bumi semakin lama terus menghangat apabila penggunaan bahan"
				   "\nbakar yang tidak bersih dan ramah lingkungan terus dikakukan. Hal tersebut dapat diatasi" 
				   "\nsalah satunya dengan menggunakan sumber energi alternatif seperti turbin angin. Oleh" 
				   "\nkarena itu untuk mendukung penggunaan turbin angin, kami membuat program ini sebagai"
				   "\npenyedia simulasi penghitung daya yang dihasilkan serta gambaran realisasi pengurangan" 
				   "\nketergantungan terhadap sumber listrik negara yang disajikan dalam %% dan histogram\n\n");
			break;
	}

	system("pause");
	system("cls");
}

void programExit(int *confirmExit){
	int inputMenu;
	char errorRetry[100], errorAlert[100];

	// Dialog konfirmasi keluar program
	printf("[Menu 8: Keluar Program]\n"
		   "\nApakah anda yakin ingin keluar program?"
		   "\nSemua data yang disimpan akan hilang."
		   "\n   [0] Tidak\n   [1] Ya\n");

	strcpy(errorRetry, "\nKonfirmasi keluar program : ");
	strcpy(errorAlert, "\n[!] Input anda tidak sesuai opsi");
    errorHandlingMenu(errorRetry, errorAlert, &inputMenu, 0, 1);
	
	switch(inputMenu){
		case 0:
			// Jika batal keluar, maka kembali ke main menu
			system("pause");
			system("cls");
			break;
		case 1:
			// Jika konfirmasi keluar, maka confirmExit dibuat bernilai 1
			// Jadinya akan memutus do-while loop yang mengulang program
			system("cls");
			printf("[:)] Terima kasih sudah menggunakan program ini! <3\n");
			system("pause"); 
			*confirmExit = 1;
			break;
	}
}

