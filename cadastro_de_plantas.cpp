#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Planta {
    int id;
    string nome;
    string nomeCientifico;
    string regiao;
    double alturaMedia;
    double resistencia;
    double phIdeal;
    string descricao;
};

const string arquivoNome = "dados.csv";

// funções principais
Planta* inicializar(ifstream& arquivo, Planta* plantas, int& tamAtual); 
void imprimirListaDePlantas(Planta* plantas, int tamAtual, int qtdPlantasDeletadas);
void imprimirPlantaSelecionada(Planta *plantas, int tamAtual);
Planta* inserirPlanta(Planta* plantas, int& tamAtual);
void deletarPlantaSelecionada(Planta* plantas, int &tamAtual, int& qtdPlantasDeletadas);
void editarPlantaSelecionada(Planta* plantas, int tamAtual);

// funções auxiliares
Planta* alocarMemoria(Planta* plantas, int tamAtual);  
int menu();
void limparTela();
bool voltarMenu();
int verificarValorInt();
double verificarValorDouble();
void imprimir(Planta planta);
bool buscarId(Planta* plantas, int tamAtual, int identificador);
bool buscarNomeCientifico(Planta* plantas, int tamAtual, string nome, int& identificador);
Planta receberDadosDaPlanta(int posicao);

int main() {
    ifstream arquivo(arquivoNome);
    if (!arquivo.is_open()) {
        cout << "Não foi possível abrir o arquivo selecionado\n";
        return 1;
    }

    Planta* plantas = NULL; // ponteiro que irá apontar para o vetor de structs
    int tamAtual = 0; // armazena o tamanho atual do vetor
    int qtdPlantasDeletadas = 0; // armazena a quantidade de plantas deletadas
    plantas = inicializar(arquivo, plantas, tamAtual);

    int opcao;
    do {
        opcao = menu();
        switch(opcao) {
            case 0: {
                limparTela();
                cout << "Programa finalizado\n";
                break;
            }
            case 1:
                imprimirListaDePlantas(plantas, tamAtual, qtdPlantasDeletadas);
                break;
            case 2:
                imprimirPlantaSelecionada(plantas, tamAtual);
                break;
            case 3:
                plantas = inserirPlanta(plantas, tamAtual);
                break;
            case 4:
                deletarPlantaSelecionada(plantas, tamAtual, qtdPlantasDeletadas);
                break;
            case 5:
                editarPlantaSelecionada(plantas, tamAtual);
                break;
            default: {
                limparTela();
                cout << "Opção inválida!";
                cout << "\n\n[0] Voltar ao menu";
                cout << "\n\nDigite uma OPÇÃO e pressione ENTER: ";
                int n = verificarValorInt();

                while (n != 0) {
                    cout << "Opção inválida! Tente novamente: ";
                    n = verificarValorInt();
                }
                break;
            }
        }
    } while (opcao != 0);

    delete[] plantas;
    return 0;
}

int menu() {
    limparTela();
    cout << "======================= MENU =======================\n";
    cout << "\n[1] Imprimir lista de plantas";
    cout << "\n[2] Buscar planta";
    cout << "\n[3] Inserir planta";
    cout << "\n[4] Deletar planta";
    cout << "\n[5] Editar planta";
    cout << "\n[0] Sair";
    cout << "\n\nDigite uma OPÇÃO e pressione ENTER: ";

    int opcao = verificarValorInt();

    return opcao;
}

int verificarValorInt() {
    int valor;
    while (!(cin >> valor)) {
        cin.clear();
        cin.ignore(500, '\n');
        cout << "Inválido, digite apenas números: ";
    }
    return valor;
}

double verificarValorDouble() {
    double valor;
    while (!(cin >> valor)) {
        cin.clear();
        cin.ignore(500, '\n');
        cout << "Inválido, digite apenas números: ";
    }
    return valor;
}

void limparTela() {
    system("clear || cls");
}

Planta* alocarMemoria(Planta* plantas, int tamAtual) {
    Planta* nova = new Planta[tamAtual + 1];
    copy(plantas, plantas+tamAtual, nova);
    delete[] plantas;
    plantas = nova;

    return plantas;
}

bool voltarMenu() {
    cout << "\n[1] Realizar operação novamente";
    cout << "\n[0] Voltar ao menu";
    cout << "\n\nDigite uma OPÇÃO e pressione ENTER: ";

    int opcao;
    bool verificador;
    do {
        opcao = verificarValorInt();
        switch (opcao) {
            case 1:
                verificador = false;
                break;
            case 0:
                verificador = true;
                break;
            default:
                cout << "Opção inválida! Tente novamente: ";
                break;
        }
    } while (opcao != 0 and opcao != 1);

    return verificador;
}

void imprimir(Planta planta) {
    if (planta.id != -1) {
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
}

bool buscarId(Planta* plantas, int tamAtual, int identificador) {
    for (int i = 0; i < tamAtual; i++) {
        if (plantas[i].id == identificador and plantas[i].id != -1) {
            return true;
        }
    }
    return false;
}

bool buscarNomeCientifico(Planta* plantas, int tamAtual, string nome, int& identificador) {
    for (int i = 0; i < tamAtual; i++) {
        if (plantas[i].nomeCientifico == nome and plantas[i].id != -1) {
            identificador = i;
            return true;
        }
    }
    return false;
}

Planta receberDadosDaPlanta(int posicao) {
    Planta planta;
    planta.id = posicao + 1;

    cin.ignore();
    cout << "Nome: ";
    getline(cin, planta.nome);
        
    cout << "Nome Cientifico: ";
    getline(cin, planta.nomeCientifico);

    cout << "Regiao: ";
    getline(cin, planta.regiao);
    
    cout << "Altura media: ";
    planta.alturaMedia = verificarValorDouble();

    while (planta.alturaMedia <= 0) {
        cout << "Inválido, digite uma altura válida: ";
        planta.alturaMedia = verificarValorDouble();
    }
    
    cout << "Resistência à pragas (0 à 10): ";
    planta.resistencia = verificarValorDouble();

    while (planta.resistencia < 0 or planta.resistencia > 10) {
        cout << "Inválido, digite somente números entre 0 e 10: ";
        planta.resistencia = verificarValorDouble();
    }
    
    cout << "pH ideal do solo (0 à 14): ";
    planta.phIdeal = verificarValorDouble();

    while (planta.phIdeal < 0 or planta.phIdeal > 14) {
        cout << "Inválido, digite somente números entre 0 e 14: ";
        planta.phIdeal = verificarValorDouble();
    }

    cin.ignore();
    cout << "Descricao: ";
    getline(cin, planta.descricao);
    
    return planta;
}

// 0 - lê os dados do arquivo CSV em um vetor de structs
Planta* inicializar(ifstream& arquivo, Planta* plantas, int& tamAtual) {
    string linha;

    // cabeçalho
    getline(arquivo, linha);

    while (getline(arquivo, linha)) {
        stringstream buffer(linha);
        Planta planta;
        string palavras[8];

        for (int i = 0; i < 8; i++) {
            getline(buffer, palavras[i], ',');
        }
        
        planta.id = stoi(palavras[0]);
        planta.nome = palavras[1];
        planta.regiao = palavras[2];
        planta.nomeCientifico = palavras[3];
        planta.descricao = palavras[4];
        planta.alturaMedia = stod(palavras[5]); 
        planta.phIdeal = stod(palavras[6]);
        planta.resistencia = stod(palavras[7]);
        
        plantas = alocarMemoria(plantas, tamAtual);
        plantas[tamAtual] = planta;
        tamAtual++;
    }

    arquivo.close();
    return plantas;
}

// 1 - listar todas as plantas cadastradas
void imprimirListaDePlantas(Planta* plantas, int tamAtual, int qtdPlantasDeletadas) {
    int indice = 0;
    int opcao;
    do {
        limparTela();
        cout << "======================= LISTA DE PLANTAS CADASTRADAS ==========================\n";

        for (int i = 0; i < 5; i++) {
            if (indice < tamAtual and indice >= 0) {
                imprimir(plantas[indice]);
            }
            indice++;
        }

        cout << "-------------------------------------------------------------------------------";
        if (indice < tamAtual and indice - 5 > 0) {
            cout << "\n[1] Próxima página";
            cout << "\n[2] Página anterior";
            cout << "\n[0] Voltar ao menu";
            cout << "\n\nDigite uma OPÇÃO e pressione ENTER: ";
            opcao = verificarValorInt();

            while (opcao < 0 or opcao > 2) {
                cout << "Opção inválida! Tente novamente: ";
                opcao = verificarValorInt();
            }

        } else if (indice < tamAtual) {
            cout << "\n[1] Próxima página";
            cout << "\n[0] Voltar ao menu";
            cout << "\n\nDigite uma OPÇÃO e pressione ENTER: ";
            opcao = verificarValorInt();

            while (opcao != 0 and opcao != 1) {
                cout << "Opção inválida! Tente novamente: ";
                opcao = verificarValorInt();
            }

        } else {
            cout << "-------------------------------------------------------------------------------";
            cout << "\nQuantidade de plantas cadastradas: " << tamAtual - qtdPlantasDeletadas << "\n";
            cout << "\n[2] Página anterior";
            cout << "\n[0] Voltar ao menu";
            cout << "\n\nDigite uma OPÇÃO e pressione ENTER: ";
            opcao = verificarValorInt();

            while (opcao != 0 and opcao != 2) {
                cout << "Opção inválida! Tente novamente: ";
                opcao = verificarValorInt();
            }
        }
        
        if (opcao == 2) {
            indice -= 10;
        }
    } while (opcao != 0 );
}

// 2 - listar planta selecionada
void imprimirPlantaSelecionada(Planta *plantas, int tamAtual) {
    limparTela();
    cout << "================= BUSCANDO UMA PLANTA ==============\n\n";
    cout << "Buscar por:";
    cout << "\n\n[1] Identificador";
    cout << "\n[2] Nome Científico";
    cout << "\n[0] Voltar ao menu";
    cout << "\n\nDigite uma OPÇÃO e pressione ENTER: ";
    
    int opcao = verificarValorInt();

    while (opcao < 0 or opcao > 2) {
        cout << "Opcão inválida, tente novamente: ";
        opcao = verificarValorInt();
    }

    if (opcao == 0) {
        return;

    } else if (opcao == 1) {
        limparTela();
        cout << "Identificador da planta que deseja buscar: ";
        int identificador = verificarValorInt();

        if (buscarId(plantas, tamAtual, identificador)) {
            limparTela();
            cout << "============================= PLANTA ENCONTRADA ===============================\n";
            imprimir(plantas[identificador - 1]);
            cout << "-------------------------------------------------------------------------------";
        } else {
            cout << "\nPlanta não encontrada\n";
        }

    } else {
        limparTela();
        cout << "Nome científico da planta que deseja buscar ";
        string nomeCientifico;
        int identificador;

        cin.ignore();
        getline(cin, nomeCientifico);

        if (buscarNomeCientifico(plantas, tamAtual, nomeCientifico, identificador)) {
            limparTela();
            cout << "============================= PLANTA ENCONTRADA ===============================\n";
            imprimir(plantas[identificador]);
            cout << "-------------------------------------------------------------------------------";
        } else {
            cout << "\nPlanta não encontrada\n";
        }
    }

    if (!voltarMenu()) {
        imprimirPlantaSelecionada(plantas, tamAtual);
    } 
}

// 3 - inserir uma nova planta no vetor
Planta* inserirPlanta(Planta* plantas, int& tamAtual) {
    limparTela();
    cout << "================= INSERINDO UMA PLANTA =============\n\n";

    bool verificador = false;
    for (int i = 0; i < tamAtual and !verificador; i++) {
        if (plantas[i].id == -1) {
            plantas[i] = receberDadosDaPlanta(i);
            verificador = true;
        }
    }

    if (!verificador) {
        plantas = alocarMemoria(plantas, tamAtual);
        plantas[tamAtual] = receberDadosDaPlanta(tamAtual);
        tamAtual++;
    }

    cout << "\nPlanta inserida com sucesso!\n";
    
    if (!voltarMenu()) {
        plantas = inserirPlanta(plantas, tamAtual);
    }
    
    return plantas;
}

// 4 - deletar uma planta do vetor
void deletarPlantaSelecionada(Planta* plantas, int &tamAtual, int& qtdPlantasDeletadas) {
    limparTela();
    cout << "=============== DELETANDO UMA PLANTA ===============\n\n";
    cout << "Digite o identificador da planta que deseja deletar: ";

    int identificador = verificarValorInt();

    if (buscarId(plantas, tamAtual, identificador)) {
        plantas[identificador - 1].id = -1;
        qtdPlantasDeletadas++;
        cout << "\nPlanta deletada com sucesso!\n";
    } else {
        cout << "\nPlanta não encontrada\n";
    }
    
    if(!voltarMenu()) {
        deletarPlantaSelecionada(plantas, tamAtual, qtdPlantasDeletadas);
    }
}

// 5 - editar uma planta do vetor
void editarPlantaSelecionada(Planta* plantas, int tamAtual){
    limparTela();
    cout << "================= EDITANDO UMA PLANTA ==============\n\n";
    cout << "Digite o identificador da planta que deseja editar: ";

    int identificador = verificarValorInt();
    
    if (buscarId(plantas, tamAtual, identificador)) {
        cout << "\nAgora entre com os dados Atualizados\n\n";
        plantas[identificador - 1] = receberDadosDaPlanta(identificador - 1);
        cout << "\nPlanta atualizada com sucesso!\n";
    } else { 
        cout << "\nPlanta não encontrada\n";
    }
    
    if(!voltarMenu()) {
        editarPlantaSelecionada(plantas, tamAtual);
    }
}
