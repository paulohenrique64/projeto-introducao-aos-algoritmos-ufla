/*
    Projeto Prático: Sistema Cadastro de Plantas (Relacionado a Biologia)
    Universidade Federal de Lavras, 19/07/2023
    Ciência da Comptuação, Introdução aos Algoritmos
    Alunos: Paulo Henrique Ribeiro Alves, Thiago Ferreira Azevedo
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <string>

#define ID_PLANTA_DELETADA -1

using namespace std;

const string arquivoNome = "plantas.dat";

struct Planta {
    int id;                   // identificador da planta
    char nome[100];           // nome da planta
    char regiao[100];         // região onde a planta é encontrada
    char nomeCientifico[100]; // nome científico (exclusivo) da planta
    char descricao[200];      // descrição da planta
    double alturaMedia;       // altura média atingida
    double phIdeal;           // pH ideal do solo para o desenvolvimento dessa planta (varia de 0 a 14)
    double resistencia;       // indica a resistencia da planta contra pragas (varia de 0 a 10)
};

// funções principais
void menuImpressao(fstream& arquivo);    
void menuOrdenacao(fstream& arquivo);   
void adicionarPlanta(fstream& arquivo); 
void removerPlanta(fstream& arquivo);
void buscarPlanta(fstream& arquivo);
void editarPlanta(fstream& arquivo);
void exportarParaCSV(fstream& arquivo);          
void informacoesGerais(fstream& arquivo);

// funções auxiliares     
Planta *alocarMemoria(Planta *plantas, int qtdItens);
Planta receberDadosDaPlanta(int posicao);
void ordenacao(fstream &arquivo, int opcao, bool removerItensDeletados);
void menu();
void limparTela();
void limparErrosArquivo(fstream& arquivo);
void intercalaID(Planta *plantas, int inicio, int meio);
void ordenacaoID(Planta *plantas, int inicio, int fim);
void intercalaNomeCientifico(Planta *plantas, int inicio, int meio, int fim);
void ordenacaoNomeCientifico(Planta *plantas, int inicio, int fim);
void intercalaAlturaMedia(Planta *plantas, int inicio, int meio, int fim);
void ordenacaoAlturaMedia(Planta *plantas, int inicio, int fim);
void imprimir(Planta planta);
double receberValorDouble();
int receberValorInt();
int realizarNovamente();
int opcaoInvalida();
int retornarUltimoID(fstream& arquivo);
int retornarTotalItens(fstream& arquivo);
int retornarTotalItensDeletados(fstream& arquivo);
int buscaBinariaId(fstream& arquivo, int identificador, int inicio, int fim, Planta& planta);
int buscaBinariaNomeCientifico(fstream& arquivo, char nomeCientifico, int inicio, int fim, Planta& planta);

int main() {
    int opcao;
    fstream arquivo(arquivoNome.c_str(), ios::in | ios::out | ios::binary);

    if (!arquivo.is_open()) {
        cout << "Não foi possível abrir o arquivo.\n";
        return 1;
    }

    do {
        menu();
        opcao = receberValorInt();

        switch (opcao) {
            case 0:
                break;

            case 1:
                menuImpressao(arquivo); 
                break;

            case 2:
                menuOrdenacao(arquivo); 
                break;

            case 3:
                adicionarPlanta(arquivo); 
                break;

            case 4:
                removerPlanta(arquivo);
                break;

            case 5:
                buscarPlanta(arquivo);
                break;

            case 6:
                editarPlanta(arquivo);
                break;

            case 7:
                exportarParaCSV(arquivo);
                break;
            
            case 8:
                informacoesGerais(arquivo);
                break;

            default:
                opcao = opcaoInvalida();
                break;
        }
    } while (opcao != 0);

    arquivo.close();
    limparTela();
    return 0;
}

/* 
    FUNÇÕES AUXILIARES
*/

// exibe o menu de opções
void menu() {
    limparTela();
    cout << "=============== CADASTRO DE PLANTAS ================\n";
    cout << "\n[1] Imprimir lista de plantas";
    cout << "\n[2] Ordenar lista de plantas";
    cout << "\n[3] Adicionar planta";
    cout << "\n[4] Remover planta";
    cout << "\n[5] Buscar planta";
    cout << "\n[6] Editar planta";
    cout << "\n[7] Exportar lista de plantas para CSV";
    cout << "\n[8] Informações gerais";
    cout << "\n[0] Sair";
    cout << "\n\nDigite uma OPÇÃO e pressione ENTER: ";
}

// lê um inteiro do teclado e retorna esse inteiro caso ele seja válido
int receberValorInt() {
    int valor;

    while (!(cin >> valor)) {
        cin.clear();
        cin.ignore(500, '\n');
        cout << "Inválido, digite apenas números: ";
    }

    return valor;
}

// lê um double do teclado e retorna esse double caso ele seja válido
double receberValorDouble() {
    double valor;

    while (!(cin >> valor)) {
        cin.clear();
        cin.ignore(500, '\n');
        cout << "Inválido, digite apenas números: ";
    }

    return valor;
}

// limpa a tela do terminal e posiciona o cursor
void limparTela() {
    system("clear || cls");
}

// adiciona 1 espaço de tamanho Planta no vetor
Planta *alocarMemoria(Planta *plantas, int qtdItens) {
    Planta *nova = new Planta[qtdItens + 1];
    copy(plantas, plantas + qtdItens, nova);
    delete[] plantas;
    return nova;
}

// verifica se o usuário quer realizar a operação novamente
int realizarNovamente() {
    int opcao;

    cout << "\n[1] Realizar operação novamente";
    cout << "\n[0] Voltar ao menu";
    cout << "\n\nDigite uma OPÇÃO e pressione ENTER: ";
    opcao = receberValorInt();

    while (opcao != 0 and opcao != 1) {
        cout << "Opção inválida! Tente novamente: ";
        opcao = receberValorInt();
    }

    return opcao;
}

// menu usado para indicar uma entrada inválida do usuário (no menu principal)
int opcaoInvalida() {
    int opcao;

    limparTela();
    cin.ignore(1000, '\n');
    cout << "================== OPÇÃO INVÁLIDA ==================\n";
    cout << "\n[1] Voltar ao menu";
    cout << "\n[0] Sair do programa\n";
    cout << "\nDigite uma OPÇÃO e pressione ENTER: ";
    opcao = receberValorInt();

    while (opcao != 0 and opcao != 1) {
        cout << "Opção inválida! Tente novamente: ";
        opcao = receberValorInt();
    }

    return opcao;
}

// recebe uma planta e imprime seus dados
void imprimir(Planta planta) {
    cout << "-------------------------------------------------------------------------------";
    cout << "\n            Identificador: " << planta.id;
    cout << "\n                     Nome: " << planta.nome;
    cout << "\n          Nome científico: " << planta.nomeCientifico;
    cout << "\n                   Região: " << planta.regiao;
    cout << "\n             Altura média: " << planta.alturaMedia << " Metros";
    cout << "\n     Resistência (0 à 10): " << planta.resistencia;
    cout << "\n         pH ideal do solo: " << planta.phIdeal;
    cout << "\n                Descrição: " << planta.descricao << "\n";
}

// intercala por identificador
void intercalaID(Planta *plantas, int inicio, int meio, int fim) {
    int i = inicio, j = meio + 1;
    int tamanho = fim - inicio + 1;
    Planta *aux = new Planta[tamanho];

    for (int k = 0; k < tamanho; k++) {
        if ((i <= meio) and (j <= fim)) {
            if (plantas[i].id <= plantas[j].id) {
                aux[k] = plantas[i];
                i++;
            }
            else {
                aux[k] = plantas[j];
                j++;
            }
        }
        else if (i > meio) {
            aux[k] = plantas[j];
            j++;
        }
        else {
            aux[k] = plantas[i];
            i++;
        }
    }

    for (int k = 0; k < tamanho; k++) {
        plantas[inicio + k] = aux[k];
    }

    delete[] aux;
}

// ordenar por identificador (merge sort)
void ordenacaoID(Planta *plantas, int inicio, int fim) {
    int meio;
    if (inicio < fim) {
        meio = (inicio + fim) / 2;
        ordenacaoID(plantas, inicio, meio);
        ordenacaoID(plantas, meio + 1, fim);
        intercalaID(plantas, inicio, meio, fim);
    }
}

// intercala por nome cientifico
void intercalaNomeCientifico(Planta *plantas, int inicio, int meio, int fim) {
    int i = inicio, j = meio + 1;
    int tamanho = fim - inicio + 1;
    Planta *aux = new Planta[tamanho];

    for (int k = 0; k < tamanho; k++) {
        if ((i <= meio) and (j <= fim)) {
            if (strcmp(plantas[i].nomeCientifico, plantas[j].nomeCientifico) <= 0) {
                aux[k] = plantas[i];
                i++;
            }
            else {
                aux[k] = plantas[j];
                j++;
            }
        }
        else if (i > meio) {
            aux[k] = plantas[j];
            j++;
        }
        else {
            aux[k] = plantas[i];
            i++;
        }
    }

    for (int k = 0; k < tamanho; k++) {
        plantas[inicio + k] = aux[k];
    }

    delete[] aux;
}

// ordenar por nome cientifico (merge sort)
void ordenacaoNomeCientifico(Planta *plantas, int inicio, int fim) {
    int meio;
    if (inicio < fim) {
        meio = (inicio + fim) / 2;
        ordenacaoNomeCientifico(plantas, inicio, meio);
        ordenacaoNomeCientifico(plantas, meio + 1, fim);
        intercalaNomeCientifico(plantas, inicio, meio, fim);
    }
}

// intercala por altura média
void intercalaAlturaMedia(Planta *plantas, int inicio, int meio, int fim) {
    int i = inicio, j = meio + 1;
    int tamanho = fim - inicio + 1;
    Planta *aux = new Planta[tamanho];

    for (int k = 0; k < tamanho; k++) {
        if ((i <= meio) and (j <= fim)) {
            if (plantas[i].alturaMedia <= plantas[j].alturaMedia) {
                aux[k] = plantas[i];
                i++;
            }
            else {
                aux[k] = plantas[j];
                j++;
            }
        }
        else if (i > meio) {
            aux[k] = plantas[j];
            j++;
        }
        else {
            aux[k] = plantas[i];
            i++;
        }
    }

    for (int k = 0; k < tamanho; k++) {
        plantas[inicio + k] = aux[k];
    }

    delete[] aux;
}

// ordenar por altura média (merge sort)
void ordenacaoAlturaMedia(Planta *plantas, int inicio, int fim) {
    int meio;
    if (inicio < fim) {
        meio = (inicio + fim) / 2;
        ordenacaoAlturaMedia(plantas, inicio, meio);
        ordenacaoAlturaMedia(plantas, meio + 1, fim);
        intercalaAlturaMedia(plantas, inicio, meio, fim);
    }
}

// cria, inicializa e retorna uma variavel do tipo Planta
Planta receberDadosDaPlanta(int posicao) {
    Planta planta;
    string buffer;

    planta.id = posicao; 
    cout << "Identificador: " << planta.id << " (Gerado automaticamente pelo sistema)";

    cin.ignore();
    cout << "\nNome: "; 

    while (!(cin.getline(planta.nome, 100, '\n'))) {
        cin.clear();
        getline(cin, buffer);
        cout << "Limite de caracteres excedido (" << 100 + int(buffer.size()) << "/100), tente novamente.\nNome: ";
    }

    cout << "Região: ";

    while (!(cin.getline(planta.regiao, 100, '\n'))) {
        cin.clear();
        getline(cin, buffer);
        cout << "Limite de caracteres excedido (" << 100 + int(buffer.size()) << "/100), tente novamente.\nRegião: ";
    }

    cout << "Nome científico: ";

    while (!(cin.getline(planta.nomeCientifico, 100, '\n'))) {
        cin.clear();
        getline(cin, buffer);
        cout << "Limite de caracteres excedido (" << 100 + int(buffer.size()) << "/100), tente novamente.\nNome científico: ";
    }

    cout << "Descrição: ";

    while (!(cin.getline(planta.descricao, 100, '\n'))) {
        cin.clear();
        getline(cin, buffer);
        cout << "Limite de caracteres excedido (" << 100 + int(buffer.size()) << "/100), tente novamente.\nDescrição: ";
    }

    cout << "Altura media: ";
    planta.alturaMedia = receberValorDouble();

    while (planta.alturaMedia <= 0) {
        cout << "Inválido, digite uma altura válida: ";
        planta.alturaMedia = receberValorDouble();
    }

    cout << "pH ideal do solo (0 à 14): ";
    planta.phIdeal = receberValorDouble();

    while (planta.phIdeal < 0 or planta.phIdeal > 14) {
        cout << "Inválido, digite somente números entre 0 e 14: ";
        planta.phIdeal = receberValorDouble();
    }

    cout << "Resistência à pragas (0 à 10): ";
    planta.resistencia = receberValorDouble();

    while (planta.resistencia < 0 or planta.resistencia > 10) {
        cout << "Inválido, digite somente números entre 0 e 10: ";
        planta.resistencia = receberValorDouble();
    }

    return planta;
}

// recebe uma opção e chama a função de ordenação correspondente
// ordenação é realizada em memória (em um vetor do tipo Planta)
// vetor é retornado para o arquivo sem as plantas deletadas
void ordenacao(fstream &arquivo, int opcao, bool removerItensDeletados) {
    int qtdItens = 0;
    Planta *plantas = NULL, aux;

    arquivo.seekg(0, arquivo.beg);

    // coloca todos os itens do arquivo em um vetor na memória
    while (arquivo.read((char *)(&aux), sizeof(Planta))) {
        plantas = alocarMemoria(plantas, qtdItens);
        plantas[qtdItens] = aux;
        qtdItens++;
    }

    limparErrosArquivo(arquivo);

    switch (opcao) {
        case 1:
            ordenacaoID(plantas, 0, qtdItens - 1);
            break;

        case 2:
            ordenacaoNomeCientifico(plantas, 0, qtdItens - 1);
            break;

        case 3:
            ordenacaoAlturaMedia(plantas, 0, qtdItens - 1);
            break;

        default:
            break;
    }

    // reescrever o vetor de itens já ordenado de volta ao arquivo binário
    // itens marcados como deletados não serão reescritos
    arquivo.seekp(0, arquivo.beg);

    if (removerItensDeletados) {
        for (int i = 0; i < qtdItens; i++) {
            if (plantas[i].id != ID_PLANTA_DELETADA) {
                arquivo.write((char *)(&plantas[i]), sizeof(Planta));
            }
        }
    } else {
        for (int i = 0; i < qtdItens; i++) {
            arquivo.write((char *)(&plantas[i]), sizeof(Planta));
        }
    }

    delete[] plantas;
}

// retorna a quantidade de itens armazenados no arquivo binário
int retornarTotalItens(fstream& arquivo) {
    int qtdItens;

    arquivo.seekg(0, arquivo.end);
    qtdItens = arquivo.tellg() / sizeof(Planta);

    return qtdItens;
}

// retorna a quantidade de itens deletados amazenados no arquivo binário
int retornarTotalItensDeletados(fstream& arquivo) {
    int qtdItensDeletados = 0;
    Planta aux;

    arquivo.seekg(0, arquivo.beg);

    while (arquivo.read((char *)(&aux), sizeof(Planta))) {
        if (aux.id == ID_PLANTA_DELETADA) {
            qtdItensDeletados++;
        }
    }

    limparErrosArquivo(arquivo);

    return qtdItensDeletados;
}

// retorna o maior identificador (último) entre os itens armazenados no arquivo binário
int retornarUltimoID(fstream& arquivo) {
    Planta aux;
    int qtdItens = 0;
    int maiorId;

    arquivo.seekg(0, arquivo.beg);

    while (arquivo.read((char *)(&aux), sizeof(Planta))) {

        if (aux.id != ID_PLANTA_DELETADA) {

            if (qtdItens == 0) 
                maiorId = aux.id;
            else if (aux.id > maiorId) 
                maiorId = aux.id;
            
            qtdItens++;
        }
    }

    limparErrosArquivo(arquivo);

    return maiorId;
}

// reseta o status do arquivo 
void limparErrosArquivo(fstream& arquivo) {
    arquivo.clear();
}

// busca binariamente uma planta pot ID
int buscaBinariaId(fstream& arquivo, int identificador, int inicio, int fim, Planta& planta) {
	Planta aux;
	int meio = (inicio + fim)/2;
	
	if(inicio <= fim){
		arquivo.seekg(meio* sizeof(Planta));
		arquivo.read((char *)(&aux), sizeof(Planta));
		
		if(aux.id == identificador){
			planta = aux;
			return meio;
		}else if(aux.id > identificador){
            return buscaBinariaId(arquivo, identificador, inicio, meio - 1, planta);
        } else{
            return buscaBinariaId(arquivo, identificador, meio + 1, fim, planta);
        }
    }
    
    return -1;
}

// busca binariamente uma planat por nome Científico
int buscaBinariaNomeCientifico(fstream& arquivo, string nomeCientifico, int inicio, int fim, Planta& planta) {
	Planta aux;
	int meio = (inicio + fim)/2;
	
	if(inicio <= fim){
		arquivo.seekg(meio * sizeof(Planta));
		arquivo.read((char *)(&aux), sizeof(Planta));
		
		if(strcmp(aux.nomeCientifico, nomeCientifico.c_str()) == 0){
			planta = aux;
			return meio;
		}else if(strcmp(aux.nomeCientifico, nomeCientifico.c_str()) > 0){
            return buscaBinariaNomeCientifico(arquivo, nomeCientifico, inicio, meio - 1, planta);
        } else{
            return buscaBinariaNomeCientifico(arquivo, nomeCientifico, meio + 1, fim, planta);
        }
    }
    
    return -1;
}

/* 
    FUNÇÕES PRINCIPAIS
*/

// 1 - menu de impressão dos itens do arquivo binário
void menuImpressao(fstream& arquivo) {
    int qtdItens = retornarTotalItens(arquivo);          
    int opcao;                     // armazenar a opçao escolhida pelo usuário
    int inicio;                    // índice da primeira planta a ser impressa
    int fim;                       // índice da última planta a ser impressa
    int indice;                    // índice que percorrerá de inicio até fim
    int itensPaginaAtual;          // quantidade de itens que foi impresso na pagina atual
    bool retornarAoMenu = false;   // verificar se o usuário quer retornar ao menu
    Planta planta;

    limparTela();
    cout << "===================== IMPRESSÃO ====================\n\n";
    cout << "[1] Imprimir a partir do indice 0\n";
    cout << "[2] Imprimir a partir de um índice inicial e final informado\n";
    cout << "[0] Voltar ao menu\n\n";
    cout << "Digite uma OPÇÃO e pressione ENTER: ";
    opcao = receberValorInt();

    switch (opcao) {
        // serão impressas todas as plantas
        case 1: {
            inicio = 0;
            fim = qtdItens - 1;
            indice = 0;
            arquivo.seekg(0, arquivo.beg);
            break;
        }

        // serão impressas somente as plantas do intervalo recebido
        case 2: {
            limparTela();

            cout << "Índice inicial: ";
            inicio = receberValorInt();

            while (inicio < 0 or inicio >= qtdItens) {
                cout << "Digite um indice inicial válido: ";
                inicio = receberValorInt();
            }

            indice = inicio;

            cout << "Índice final: ";
            fim = receberValorInt();

            while (fim < inicio or fim >= qtdItens) {
                cout << "Digite um indice final válido: ";
                fim = receberValorInt();
            }

            arquivo.seekg((inicio) * sizeof(Planta));
            break;
        }

        // retornar ao menu
        default:
            retornarAoMenu = true;
            break;
    }

    if (!retornarAoMenu) {

        do {
            
            limparTela();
            cout << "======================= LISTA DE PLANTAS CADASTRADAS ==========================\n";

            itensPaginaAtual = 0;
            for (int i = 0; i < 5 and indice <= fim; i++) {
                arquivo.read((char *)(&planta), sizeof(Planta)); 

                if (planta.id != ID_PLANTA_DELETADA) 
                    imprimir(planta);
            
                itensPaginaAtual++;
                indice++;

            }

            limparErrosArquivo(arquivo);
            
            cout << "-------------------------------------------------------------------------------";
        
            if (indice <= fim and indice - 5 > inicio) {
                // caso houver plantas antes e depois da "página" atual
                cout << "\n[1] Próxima página";
                cout << "\n[2] Página anterior";
                cout << "\n[0] Voltar ao menu\n";
                cout << "\nDigite uma OPÇÃO e pressione ENTER: ";
                opcao = receberValorInt();

                while (opcao < 0 or opcao > 2) {
                    cout << "Opção inválida! Tente novamente: ";
                    opcao = receberValorInt();
                }

            } else if (indice - 5 <= inicio and indice <= fim) {
                // caso houver somente plantas depois da "página" atual
                cout << "\n[1] Próxima página";
                cout << "\n[0] Voltar ao menu\n";
                cout << "\nDigite uma OPÇÃO e pressione ENTER: ";
                opcao = receberValorInt();

                while (opcao < 0 or opcao > 1) {
                    cout << "Opção inválida! Tente novamente: ";
                    opcao = receberValorInt();
                }

            } else if (indice >= fim and indice - 5 > inicio) {
                // caso houver somente plantas antes da "página" atual
                cout << "\n[2] Página anterior";
                cout << "\n[0] Voltar ao menu\n";
                cout << "\nDigite uma OPÇÃO e pressione ENTER: ";
                opcao = receberValorInt();

                while (opcao != 0 and opcao != 2) {
                    cout << "Opção inválida! Tente novamente: ";
                    opcao = receberValorInt();
                }

            } else {
                // caso não houver plantas antes e depois da "página" atual
                cout << "\n[0] Voltar ao menu\n";
                cout << "\nDigite uma OPÇÃO e pressione ENTER: ";
                opcao = receberValorInt();

                while (opcao != 0) {
                    cout << "Opção inválida! Tente novamente: ";
                    opcao = receberValorInt();
                }

            }

            // retonar para página anterior
            if (opcao == 2) {
                // retrocede a página atual (itensPaginaAtual) + página anterior (5 itens)
                // página anterior será impressa novamente
                indice -= (5 + itensPaginaAtual);
                arquivo.seekg(indice * sizeof(Planta));
            }
    
        } while (opcao != 0);
    } 
}

// 2 - menu para ordenação do arquivo binário
void menuOrdenacao(fstream& arquivo) {
    int opcao;

    limparTela();
    cout << "===================== ODERNAÇÃO ====================\n\n";
    cout << "[1] Ordenar a partir do Identificador\n";
    cout << "[2] Ordenar a partir do Nome Científico\n";
    cout << "[3] Ordenar a partir da Altura Média\n";
    cout << "[0] Voltar ao menu\n\n";
    cout << "Digite uma OPÇÃO e pressione ENTER: ";
    opcao = receberValorInt();

    while (opcao < 0 or opcao > 3) {
        cout << "Opção inválida! Tente novamente: ";
        opcao = receberValorInt();
    }

    if (opcao != 0) {

        // arquivo -> arquivo binário que será ordenado
        //   opcao -> tipo da ordenação (1- id, 2- nome cientifico, 3- altura media)
        //       1 -> 1 para remover os itens deletados apos a ordenação e 0 para nao remover
        ordenacao(arquivo, opcao, 1);

        limparTela();
        cout << "===================== ODERNAÇÃO ====================\n\n";
        cout << "Lista de plantas ordenada com sucesso!\n";

        if (realizarNovamente()) {
            menuOrdenacao(arquivo);
        }
    }
}

// 3 - adicionar uma nova planta no arquivo binário
void adicionarPlanta(fstream& arquivo) {
    int posicao = 0;
    bool sobrescreverPlanta = false;
    Planta nova, planta;

    limparTela();
    cout << "================= INSERINDO UMA PLANTA =============\n\n";
    nova = receberDadosDaPlanta(retornarUltimoID(arquivo) + 1);

    arquivo.seekg(0, arquivo.beg);

    while (arquivo.read((char *)(&planta), sizeof(Planta)) and !sobrescreverPlanta) {

        if (planta.id == ID_PLANTA_DELETADA) {
            sobrescreverPlanta = true;
        } else {
            posicao++;
        }
        
    }

    limparErrosArquivo(arquivo);

    if (sobrescreverPlanta) {
        // posiciona o cursor no inicio da planta marcada como deletada para sobrescrever-la
        arquivo.seekp(posicao * sizeof(Planta));
    } else { 
        // não encontrou planta marcada como deletada, cursor é posicionado no fim do arquivo binário
        arquivo.seekp(0, arquivo.end);
    }

    // escrevendo a nova planta no arquivo binário
    arquivo.write((char *)(&nova), sizeof(Planta));

    cout << "\nPlanta inserida com sucesso!\n";

    if (realizarNovamente()) {
        adicionarPlanta(arquivo);
    }
}

// 4 - remover uma planta do arquivo binário (marcando seu id como ID_PLANTA_DELETADA)
void removerPlanta(fstream& arquivo) {
    int identificador, posicao;
    Planta planta;

    limparTela();
    cout << "Digite o identificador da planta que deseja deletar: ";
    identificador = receberValorInt();
    
    // arquivo -> arquivo binário que será ordenado
    //   opcao -> tipo da ordenação (1- id, 2- nome cientifico, 3- altura media)
    //       0 -> 1 para remover os itens deletados apos a ordenação e 0 para nao remover
    ordenacao(arquivo, 1, 0);

    posicao = buscaBinariaId(arquivo, identificador, 0, retornarTotalItens(arquivo) - 1, planta);
    limparErrosArquivo(arquivo);

    if (posicao != -1) {
        // posiciona o cursor antes da planta que será deletada, reescrevendo ela com o id = ID_PLANTA_DELETADA
        planta.id = ID_PLANTA_DELETADA;
        arquivo.seekp(posicao * sizeof(Planta));
        arquivo.write((char *)(&planta), sizeof(Planta));
        cout << "\nPlanta deletada com sucesso!\n";
    } else {
        cout << "\nPlanta não encontrada\n";
    }

    if (realizarNovamente()) {
        removerPlanta(arquivo);
    }
}

// 5 - buscar uma planta no arquivo binário
void buscarPlanta(fstream& arquivo) {
	int opcao, identificador, posicao;
    string nomeCientifico;
    bool retornarAoMenu = false;
    Planta planta;

    limparTela();
    cout << "======================= BUSCA ======================\n\n";
    cout << "[1] Buscar por identificador\n";
    cout << "[2] Buscar por nome científico\n";
    cout << "[0] Voltar ao menu\n\n";
    cout << "Digite uma OPÇÃO e pressione ENTER: ";
    opcao = receberValorInt();
    
    switch (opcao) {
        case 1: {
            limparTela();
            cout << "Identificador da planta que deseja buscar: ";
            identificador = receberValorInt();

            // arquivo -> arquivo binário que será ordenado
            //   opcao -> tipo da ordenação (1- id, 2- nome cientifico, 3- altura media)
            //       0 -> 1 para remover os itens deletados apos a ordenação e 0 para nao remover
            ordenacao(arquivo, 1, 0);
            
            posicao = buscaBinariaId(arquivo, identificador, 0, retornarTotalItens(arquivo) - 1, planta);
            break;
        }

        case 2: {
            limparTela();
            cout << "Nome científico da planta que deseja buscar: ";
            cin.ignore();
            getline(cin, nomeCientifico);

            // arquivo -> arquivo binário que será ordenado
            //   opcao -> tipo da ordenação (1- id, 2- nome cientifico, 3- altura media)
            //       0 -> 1 para remover os itens deletados apos a ordenação e 0 para nao remover
            ordenacao(arquivo, 2, 0);
			
			posicao = buscaBinariaNomeCientifico(arquivo, nomeCientifico, 0, retornarTotalItens(arquivo) - 1, planta);
            break;
        }

        default:    
            retornarAoMenu = true;
            break;
    }

    if (!retornarAoMenu) {

        if (posicao != -1) {
            limparTela();
            cout << "============================= PLANTA ENCONTRADA ===============================\n";
            imprimir(planta);
            cout << "-------------------------------------------------------------------------------";
        } else {
            cout << "\nPlanta não encontrada\n";
        }

        if (realizarNovamente()) {
            buscarPlanta(arquivo);
        } 

    } 
}

// 6 - edita uma planta do arquivo binário
void editarPlanta(fstream& arquivo) {
    int identificador, posicao;
    Planta planta, nova;

    limparTela();
    cout << "Digite o identificador da planta que deseja editar: ";
    identificador = receberValorInt();

    // arquivo -> arquivo binário que será ordenado
    //   opcao -> tipo da ordenação (1- id, 2- nome cientifico, 3- altura media)
    //       0 -> 1 para remover os itens deletados apos a ordenação e 0 para nao remover
    ordenacao(arquivo, 1, 0);

    posicao = buscaBinariaId(arquivo, identificador, 0, retornarTotalItens(arquivo) - 1, planta);

    if (posicao != -1) {

        cout << "\nDados atuais da planta:\n";
        imprimir(planta); 
        cout << "-------------------------------------------------------------------------------";
        cout << "\nDigite os novos dados da planta:\n";

        nova = receberDadosDaPlanta(planta.id);
        arquivo.seekp((posicao) * sizeof(Planta));
        arquivo.write((char*)(&nova), sizeof(Planta));
        cout << "\nPlanta editada com sucesso!\n";

    } else { 
        cout << "\nPlanta não encontrada\n";
    }
    
	 if (realizarNovamente()) {
        editarPlanta(arquivo);
    }
}

// 7 - exportar todos os dados do arquivo binário para um arquivo CSV
void exportarParaCSV(fstream& arquivo) {
    ofstream arquivoCSV("plantas.csv");
    string buffer;
    Planta planta;

    // cabeçalho
    arquivoCSV << "Identificador,Nome da Planta,Região,Nome Científico,Descrição,Altura Média,pH Ideal do Solo,Resistência contra Pragas\n";

    arquivo.seekg(0, arquivo.beg);

    // ler item por item do arquivo binário e escrever-los no arquivo CSV
    while (arquivo.read((char *)(&planta), sizeof(Planta))) {
        if (planta.id != ID_PLANTA_DELETADA) {
            arquivoCSV << planta.id << "," << planta.nome << "," << planta.regiao << "," << planta.nomeCientifico << ","
            << planta.descricao << "," << planta.alturaMedia << "," << planta.phIdeal << "," << planta.resistencia << "\n";
        }
    }

    limparErrosArquivo(arquivo);

    limparTela();
    cout << "Arquivo \"plantas.csv\" gerado com sucesso!\nVerifique o diretório onde se encontra esta aplicação.\n\n";
    cout << "Aperte qualquer TECLA e pressione ENTER para voltar ao MENU: ";
    cin.ignore();
    getline(cin, buffer);

    arquivoCSV.close();
}

// 8 - exibir infomações sobre o armazenanento de itens do arquivo binário
void informacoesGerais(fstream& arquivo) {
    string buffer;
    
    limparTela();
    cout << "================ INFORMAÇÕES GERAIS ================\n\n";    
    cout << "Quantidade de Plantas cadastradas: " << retornarTotalItens(arquivo) - retornarTotalItensDeletados(arquivo);
    cout << "\nÚltimo  ID  registrado no sistema: " << retornarUltimoID(arquivo);

    cout << "\n\nAperte qualquer TECLA e pressione ENTER para voltar ao MENU: ";
    cin.ignore();
    getline(cin, buffer);
}
