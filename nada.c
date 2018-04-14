#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "GerenciaCMD.c"

int QTD_BLOC = 3;
int TAM_BLOC = 32;

void PosicaoFile(FILE * f){
	long lSize = ftell(f);
	printf("ptFile : %d \n", lSize);
}

int ContaCaracter(char * c){
	int a = 0;
	int i = 0;

	while(c[i] != '\0'){
		a++;
		i++;
	}
	return a;
}

void IniciaArquivo(FILE * f)
{
	int a = 0;
	int i = 0;
	int ctrChar = 0;
	while(a < QTD_BLOC)
	{
		ctrChar = 0;
		for(i = 0; i < TAM_BLOC; i++)
		{	
			if(ctrChar != (TAM_BLOC-1))
			{
				fputc('0', f);					
			}else{
				fputc('#', f);
			}
			ctrChar++;
		}
		a++;
	}
	PosicaoFile(f);
	rewind(f);
}

void CompletaString(char *buff, int limite)
{
	//Completa a string 
	int i = ContaCaracter(buff);
	int a = 0;
	while(i < limite )
	{
		if(a == 0) { 
			strcat(buff, "\0");
			a++;
		}else{
			strcat(buff, "0");
		}
		i++;
	}
}

int EnderecoValido(FILE * f)
{
	int a = 0;
	int indice = 0;
	fpos_t position;
	
	while (a < QTD_BLOC){
		
		fseek(f, (a*TAM_BLOC), SEEK_SET);
	
		fgetpos(f, &position);
		char temp = fgetc(f);
		
		if(temp == '0'){
			indice = a * TAM_BLOC;
			printf("aqui tem q sair %d \n", indice);
			//Muda o 'a' para encerrar o looping
			a = QTD_BLOC;
			PosicaoFile(f);
			fsetpos(f, &position);
			PosicaoFile(f);
		}
		a++;
	}
	
	return indice;
}

void GravarBloco(FILE * f, char * str, int limite, int indice)
{
	int i;
	fseek(f, indice, SEEK_SET);
	
	for(i = 0; i < limite; i++)
	{
		printf("%c", str[i]);
		//fwrite(str, limite, 1, f);
		fputc(str[i], f);
	}
}

void CriarBloco (FILE * f, char * permis, char * titulo)
{
	int i;
	
	//Gravar na posição encontrada
	char * bloco = (char *) malloc(sizeof(char)*TAM_BLOC);
	char buffEndereco[6];
	
	int indice = EnderecoValido(f);
	itoa(indice, buffEndereco, 10);
	
	CompletaString(buffEndereco, sizeof(buffEndereco));
	CompletaString(titulo, sizeof(titulo));
	
	for(i = 0; i < TAM_BLOC; i++)
	{
		if(i == 0)
		{
			bloco[i] = '1';
		}
		
		if(i > 0 & i < 7)
		{
			bloco[i] = buffEndereco[i-1];
		}
		
		if(i > 6 & i < 8)
		{
			bloco[i] = permis[0];
		}
		
		if(i > 7 & i < 31)
		{
			bloco[i] = titulo[i-8];
		}
		
		if(i == 31)
		{
			bloco[i] = '#';
		}
	}
	//printf("Parametros no criar bloco: %d\n ",ContaCaracter(titulo));
	GravarBloco(f, bloco, TAM_BLOC, indice);
	free(bloco);
}

void FechaArquivo(FILE * f)
{
	fclose(f);
}

FILE * AbreArquivo(FILE * f, char tipo)
{
	
	switch(tipo){
		case 'c':
			f = fopen("HD.hd", "wb");
			break;
		
		case 'u':
			f = fopen("HD.hd", "rb+");
	}
	
	if (f==NULL) {fputs ("File error",stderr); exit (1);}
	if (tipo == 'c') {IniciaArquivo(f);FechaArquivo(f);f = AbreArquivo(f, 'u');}

	return f;
}

int TamanhoDoArquivo (FILE * f){
	int tam;
	fseek(f, 0, SEEK_END);
	tam = ftell(f);	
	return tam;
}

void Printa(FILE * f, int tam)
{
	int i;
	int a;
	
	int fTam = TamanhoDoArquivo(f);
	char * buffer = (char*) malloc(sizeof(char) * fTam);
	
	if(tam != 0)
	{

		rewind(f);
		
		while(a < QTD_BLOC)
		{
			for(i = 0; i < tam; i++)
			{
				fread(buffer, 32, 1, f);
			}			
			a++;
		}
		printf("\n%s\n", buffer);
	}
	free(buffer);		
}




int main() {

	FILE * f;

	f = AbreArquivo(f, 'c');
	char permis[1] = {'X'};
	int ctrCMD = 1;
	char comando[256];
	
	while(ctrCMD)
	{
		printf("==> ");
		gets(comando);
		GerenciadorCMD(comando, &ctrCMD);
	}
	
	
	
	
//Simula entrada do usuário
char contFile[25] = "Hello World";
char titulo[23] = "First file WTF";

CriarBloco(f, permis, titulo);

	//Printa(f, QTD_BLOC);
	//fprintf(f, "%s", bloco);

	FechaArquivo(f);
	return 0;
}




/// ------ INICIA ARQUIVO
//	while(a < QTD_BLOC)
//	{
//		ctrChar = 0;
//		for(i = 0; i < TAM_BLOC; i++)
//		{	
//			if(ctrChar != (TAM_BLOC-1))
//			{
//				// APENAS muda o flag de edição de primeiro bloco
//			//	if(a == 0 & i == 0){
//			//		fputc('1', f);
//			//	}else{
//					
//					
//					fputc('0', f);					
//					
//					
//				//}
//			}else{
//				fputc('#', f);
//				//fputc('\n', f);
//			}
//			ctrChar++;
//		}
//		a++;
//	}
//PosicaoFile(f);
//rewind(f);

/*  -------> Procura uma posição para gravar
a = 0;
fpos_t position;
int indice = 0;
while (a < QTD_BLOC){
	
	fseek(f, (a*TAM_BLOC), SEEK_SET);

	fgetpos(f, &position);
	char temp = fgetc(f);
	
	if(temp == '0'){
		indice = a * TAM_BLOC;
		printf("aqui tem q sair %d \n", indice);
		//Muda o 'a' para encerrar o looping
		a = QTD_BLOC;
		PosicaoFile(f);
		fsetpos(f, &position);
		PosicaoFile(f);
	}
	a++;
}
*/


//printf("-------------\n");
//printf("COUNT do buff %d \n", ContaCaracter(buff) );
//printf("ultimo buff %s \n", buff );
//printf("o tamanho do indice e %d \n", sizeof(indice) );

////Completa a string 
//i = ContaCaracter(buff);
//while(i < sizeof(buff) )
//{
//	strcat(buff, "0");
//	i++;
//}

//for(i = 0; i < TAM_BLOC; i++)
//{
//	if(i == 0){
//		bloco[i] = '1';
//	}
//	
//	if(i > 0 & i < 7)
//	{
//		bloco[i] = buff[i-1];
//	}
//	
//	if(i > 6 & i < 8)
//	{
//		bloco[i] = permis[0];
//	}
//	
//	if(i > 7 & i < 31)
//	{
//		bloco[i] = titulo[i-8];
//	}
//	
//	if(i == 31)
//	{
//		bloco[i] = '#';
//	}
//}

