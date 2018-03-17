#include <iostream>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> //função sleep();
#include <windows.h>

using namespace std;


bool criado(){ //verifica a existencia do arquivo PRODUTOS.DAT
	FILE *arq;
	arq = fopen("PRODUTOS.DAT", "rb");
	if(arq == NULL){
		return false;
	}else{
		return true;
	}
}

bool criado_txt(){ //verifica a existencia do arquivo PRODUTOS.DAT
	FILE *arq;
	arq = fopen("PRODUTOS.txt", "r");
	if(arq == NULL){
		return false;
	}else{
		return true;
	}
}

struct Produto {
	char nome[50];
	int codigo;
	int preco;
	int qntd;
};

int contador(){
	FILE *arq;
	Produto buffer;
	int cont;

	arq = fopen("PRODUTOS.DAT", "rb");
	fseek(arq, 0, SEEK_END);
	cont = (ftell(arq)/sizeof(Produto));

	return cont;
}

void criar(){ //cria o arquivo PRODUTOS.DAT
	FILE *arq;
	arq = fopen("PRODUTOS.DAT", "wb");
	if (!ferror (arq)){
		cout << "\n----------Arquivo criado com sucesso----------\n\n";
	}else{
		cout << "ERRO AO CRIAR ARQUIVO";
	}
	fclose(arq);
}

void criartxt(){
	FILE *arq;
	arq = fopen("PRODUTOS.txt", "w");
	if (!ferror (arq)){
		cout << "\n----------Arquivo criado de texto com sucesso----------\n\n";
	}else{
		cout << "ERRO AO CRIAR ARQUIVO";
	}
	fclose(arq);
}

void inserir(){ //Insere registros no arquivo PRODUTOS.DAT
	Produto buffer;
	setlocale(LC_ALL, "Portuguese");
	FILE *arq;
	int n, verif;
	bool verifi;
	arq = fopen("PRODUTOS.DAT", "ab");

	if (!ferror(arq)){
		cout << "Entre com a quantidade de produtos: ";
		cin >> n;

		for(int i=1; i<=n; i++){
			cout << endl;
			cin.ignore();
			cout << "Digite o nome do produto " << i << ": ";
			fgets(buffer.nome, 50, stdin);
			fflush(stdin);
			cout << "Digite o código do produto " << i << ": ";
			cin >> buffer.codigo;
			cout << "Digite o preço do produto " << i << ": ";
			cin >> buffer.preco;
			cout << "Digite a quantidade em estoque do produto " << i << ": ";
			cin >> buffer.qntd;
			verif = fwrite(&buffer, sizeof(Produto), 1, arq);
			if(verif = 1){
				cout << "Produto registrado com sucesso!\n";
				verifi = true;
			}else {
			    cout << "Erro ao registrar produto\n";
			    verifi = false;
			}
		}

		if(n > 1 && verifi){
			cout << "\nTodos os arquivos foram registrados com sucesso!\n";
		}
		Sleep(1000);
		fclose(arq);
	}else{
		cout << "ERRO AO ABRIR ARQUIVO";
		Sleep(1000);
	}
	system("cls");
}

bool pesquisar(int pesq_codigo){ //Pesquisa dentro do arquivo a existencia de algum produto pelo codigo (pesq_codigo)
	FILE *arq;
	Produto buffer;
	arq = fopen("PRODUTOS.DAT", "rb");
	while (fread(&buffer, sizeof(Produto), 1, arq) != NULL){
		if (buffer.codigo==pesq_codigo){
			fseek(arq, 0, SEEK_END);
			fclose(arq);
			return true;
		}
	}
	fclose(arq);
	return false;
}

void alterar(int pesq_codigo){ //Altera um registro fornecid
	FILE *arq;
	Produto buffer;
	int verif;

	arq = fopen("PRODUTOS.DAT", "rb+");
	while (fread(&buffer, sizeof(Produto), 1, arq) != NULL){
		if (buffer.codigo==pesq_codigo){
			fseek(arq, ((-1)*sizeof(Produto)), SEEK_CUR);
			break;
		}
	}
	fflush(stdin);
	cout << "Digite o novo nome do produto: ";
	fgets(buffer.nome, 50, stdin);
	fflush(stdin);
	cout << "Digite o novo código do produto: ";
	cin >> buffer.codigo;
	cout << "Digite o novo preço do produto: ";
	cin >> buffer.preco;
	cout << "Digite a nova quantidade em estoque do produto: ";
	cin >> buffer.qntd;
	verif = fwrite(&buffer, sizeof(Produto), 1, arq);
	if(verif == 1) cout << "\nProduto alterado com sucesso!\n";
	else cout << "\nHouve um erro ao alterar o arquivo\n";
	fclose(arq);
}

void imprimir(int pesq_codigo, int cont){
	FILE *arq;
	Produto buffer[cont];
	Produto p;
	int returner;

	system("cls");

	do{
	    arq = fopen("PRODUTOS.DAT", "rb");
		if (pesq_codigo == 0){
            cout << "----------LISTANDO TODOS OS PRODUTOS----------\n";
            fread(buffer, sizeof(Produto), cont, arq);
            for(int i=0; i<cont; i++){
	            cout << "\tCÓDIGO: " << buffer[i].codigo << endl;
	            cout << "\tNOME: " << buffer[i].nome;
	            cout << "\tPREÇO: " << buffer[i].preco << endl;
	            cout << "\tQNTD EM ESTOQUE: " << buffer[i].qntd << endl;
	            cout << endl;
	        }
	    }
		else{
			if(pesquisar(pesq_codigo)){
				while (fread(&p, sizeof(Produto), 1, arq) != NULL){
					if ((p.codigo) == pesq_codigo){
						fseek(arq, (-1*sizeof(Produto)), SEEK_CUR);
						break;
					}
				}
				fread(&p, sizeof(Produto), 1, arq);
				cout << "----------CÓDIGO "<< pesq_codigo << "----------\n";
	            cout << "\tNOME: " << p.nome;
	            cout << "\tPREÇO: " << p.preco << endl;
	            cout << "\tQNTD EM ESTOQUE: " << p.qntd << endl;
			}
			else cout << "\nO produto não existe\n";
		}
		cout << "\nPara voltar ao menu, digite 0\n";
		cin >> returner;
		fclose(arq);
	}while (returner != 0);
}

/*void imprimir (int cont){
    FILE *arq;
    Produto buffer[cont];
    while(fgets(buffer),sizeof(Produto),arq){
        if(feof(arq)>0){
            fclose(arq);
            break;
        }
        printf("%s\n", buffer);
    }
}*/

void ordenar(int cont){ // Ordena os registros no arquivo binário de acordo com seu código
	FILE *arq;
	Produto A, B;

	arq = fopen("PRODUTOS.DAT", "rb+");
	if(!ferror(arq)){
		for(int i=0; i<=cont; i++){
			fseek(arq, 0, SEEK_SET);
			while (fread(&A, sizeof(Produto), 1, arq) != NULL){
				if(fread(&B, sizeof(Produto), 1, arq) != NULL){
					if(A.codigo > B.codigo){
						fseek(arq, ((-2)*sizeof(Produto)), SEEK_CUR);
						fwrite(&B, sizeof(Produto), 1, arq);
						fwrite(&A, sizeof(Produto), 1, arq);
						fseek(arq, ((-1)*sizeof(Produto)), SEEK_CUR);
					}else fseek(arq, ((-1)*sizeof(Produto)), SEEK_CUR);
				}
			}
		}
		cout << "Registros ordenados com sucesso!";
	}else cout << "ERRO AO ABRIR ARQUIVO";
	fclose(arq);
	Sleep(1000);
}

void imprimetxt(int cont){ //Carrega registros do arquivo binário, os ordena e imprime em um arquivo de texto
	FILE *arq, *arqtxt;
	Produto buffer[cont];
	int verif;

	ordenar(cont);
	arq = fopen("PRODUTOS.DAT", "rb");
	verif = fread(&buffer, sizeof(Produto), cont, arq);
	if(verif == cont){
		arqtxt = fopen("PRODUTOS.txt", "w");
		fprintf(arqtxt,"\n\tPRODUTOS REGISTRADOS\n");
        for(int i=0; i
        <cont; i++){
            fprintf(arqtxt,"----------CÓDIGO %2d----------\n", buffer[i].codigo);
            fprintf(arqtxt, "\tNOME: %s", buffer[i].nome);
            fprintf(arqtxt, "\tPREÇO: %2d\n", buffer[i].preco);
            fprintf(arqtxt, "\tQNTD EM ESTOQUE: %2d\n", buffer[i].qntd);
            fprintf(arqtxt, "\n");
        }
	}else cout << "ERRO AO LER REGISTROS DO ARQUIVO BINÁRIO";

	fclose(arq);
	fclose(arqtxt);
	cout << "\nRegistros impressos em PRODUTOS.txt!";
	Sleep(1000);
	system("cls");
}

void excluir_registro(int pesq_codigo, int cont){
	FILE *arq;
	Produto A, B, buffer[cont];
	int verif;

	arq = fopen("PRODUTOS.DAT", "rb+");
	if(pesquisar(pesq_codigo)){
		fseek(arq, 0, SEEK_END);
		fseek(arq, ((-1)*sizeof(Produto)), SEEK_CUR);
		fread(&A, sizeof(Produto), 1, arq);
		fseek(arq, 0, SEEK_SET);
		while (fread(&B, sizeof(Produto), 1, arq) != NULL){
			if (B.codigo==pesq_codigo){
				fseek(arq, ((-1)*sizeof(Produto)), SEEK_CUR);
				break;
			}
		}
		verif = fwrite(&A, sizeof(Produto), 1, arq);
		if(verif == 1) cout << "Produto excluído com sucesso";
		else cout << "ERRO AO EXLCUIR PRODUTO";
		fseek(arq, 0, SEEK_SET);
		fread(buffer, sizeof(Produto), cont, arq);
		fclose(arq);
		arq = fopen("PRODUTOS.DAT", "wb");
		verif = fwrite(buffer, sizeof(Produto), (cont-1), arq);
		fclose(arq);
	}
	else cout << "Produto não existente";

	Sleep(1000);
	fclose(arq);
	system("cls");
}


int main(){
	setlocale(LC_ALL, "Portuguese");

	FILE *arq;
	int escolha, pesq_codigo;
	bool verif_seguranca, pesquisa, arq_existe, arqtxt_existe;
	Produto p;
	int contador_de_registros = contador();

	arq_existe = criado();
	arqtxt_existe = criado_txt();

	do{
		contador_de_registros = contador();
		cout << "Escolha o que fazer:\n";
		cout << "\t\n01. Criar arquivo binário";
		cout << "\t\n02. Cadastrar produto";
		cout << "\t\n03. Pesquisar produto";
		cout << "\t\n04. Excluir dados do arquivo binário";
		cout << "\t\n05. Alterar um registro";
		cout << "\t\n06. Consultar registro";
		cout << "\t\n07. Conferir quantidade de produtos registrados";
		cout << "\t\n08. Ordenar arquivo binário";
		cout << "\t\n09. Imprimir arquivo de texto";
		cout << "\t\n10. Excluir um registro";
		cout << "\t\n11. Excluir dados do arquivo de texto";
		cout << "\t\n0. Sair\n";

		cin >> escolha;

		switch(escolha){
			case 1:
				if(!arq_existe){ //Somente cria o arquivo se ele ainda nao existe
					criar();
					arq_existe = true;
				}
				else cout << "\n----------Arquivo previamente existente----------\n\n";
				Sleep(1000);
				system("cls");
			break;

			case 2:
				system("cls");
				inserir();
			break;

			case 3:
				if(contador_de_registros > 0){ //Se nao houver registros no arquivo, o processo nao se inicia
					cout << "Digite o código do produto que deseja pesquisar: ";
					cin >> pesq_codigo;
					pesquisa = pesquisar(pesq_codigo);
					if(pesquisa) cout << "\nPRODUTO EXISTENTE\n";
					else cout << "\nPRODUTO NÃO EXISTENTE\n";
				} else cout << "\nNenhum produto registrado até o momento, por favor insira um produto.\n\n";
				Sleep(1000);
				system("cls");
			break;

			case 4: // Limpa os registros do arquivo binário
				if(contador_de_registros > 0){ //Se nao houver registros no arquivo, o processo nao se inicia
					cout << "\nTem certeza que deseja excluir os dados do arquivo binário? Esse processo irreversível.\n";
					cout << "\n1. Sim\n0. NÃO\n";
					cin >> verif_seguranca;
					if(verif_seguranca){
						arq = fopen("PRODUTOS.DAT", "wb+");
						contador_de_registros = contador();
						if(contador_de_registros == 0){
                            cout << "\nArquivo limpo com sucesso!\n";
						}else cout << "\nErro ao limpar arquivo\n";
					}else cout << "\nProcesso abortado... Voltando ao menu...";
				}
				else cout << "\nNenhum produto registrado até o momento.\n\n";
				Sleep(1000);
				fclose(arq);
				system("cls");
			break;

			case 5:
				if(contador_de_registros > 0){ //Se nao houver registros no arquivo, o processo nao se inicia
					cout << "Digite o código do produto que deseja alterar: ";
					cin >> pesq_codigo;
					alterar(pesq_codigo);
				}else cout << "\nNenhum produto registrado até o momento, por favor insira um produto.\n\n";
				Sleep(1000);
				system("cls");
			break;

			case 6:
				if(contador_de_registros > 0){ //Se nao houver registros no arquivo, o processo nao se inicia
					cout << "Digite o código do produto que deseja consultar (0 imprime todos os registros): ";
					cin >> pesq_codigo;
					imprimir(pesq_codigo, contador_de_registros);
				}else{
					cout << "\nNenhum produto registrado até o momento, por favor insira um produto.\n\n";
					Sleep(1000);
				}
				system("cls");
			break;

			case 7:
			    if(contador_de_registros > 0) cout << "Atualmente, está(ão) cadastrado(s) " << contador_de_registros << " produto(os).\n";
			    else cout << "Atualmente, nenhum produto está cadastrado.";
				Sleep(1000);
				system("cls");
			break;

			case 8:
				if(contador_de_registros > 0){
					ordenar(contador_de_registros);
				}
				else{
					cout << "\nNenhum produto registrado até o momento, por favor insira um produto.\n\n";
					Sleep(1000);
				}
				system("cls");
			break;

			case 9:
				if(contador_de_registros > 0){
					if(!arqtxt_existe){
						criartxt();
						imprimetxt(contador_de_registros);
					}
					else imprimetxt(contador_de_registros);
				}else{
					cout << "\nNenhum produto registrado até o momento, por favor insira um produto.\n\n";
					Sleep(1000);
				}
				system("cls");
			break;

			case 10:
				contador_de_registros = contador();
				if(contador_de_registros > 0){
					cout << "Digite o código do produto que deseja exlcuir: ";
					cin >> pesq_codigo;
					excluir_registro(pesq_codigo, contador_de_registros);
				}else cout << "\nNenhum produto registrado até o momento, por favor insira um produto.\n\n";
			break;

			case 11: // Limpa os registros do arquivo de texto
				arq = fopen("PRODUTOS.txt", "r+");
				fseek(arq, 0, SEEK_END);
				if(ftell(arq) != 0){ //Se nao houver registros no arquivo, o processo nao se inicia
					cout << "\nTem certeza que deseja excluir os dados do arquivo de texto? Esse processo irreversível.\n";
					cout << "\n1. Sim\n0. NÃO\n";
					cin >> verif_seguranca;
					fclose(arq);
					if(verif_seguranca){
						arq = fopen("PRODUTOS.txt", "w+");
						if(ftell(arq) == 0){
                            cout << "\nArquivo limpo com sucesso!\n";
						}else cout << "\nErro ao limpar arquivo\n";
					}else cout << "\nProcesso abortado... Voltando ao menu...";
				}
				else cout << "\nNenhum produto registrado no arquivo de texto até o momento.\n\n";
				Sleep(1000);
				fclose(arq);
				system("cls");
			break;
		}
	}while(escolha!=0);
}
