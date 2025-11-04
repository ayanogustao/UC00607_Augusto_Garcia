#include <iostream>
#include <vector>
#include <limits>
#include "colaborador.h"
#include "io.h"
using namespace std;

int main() {
    vector<Colaborador> colaboradores;
    string nomeFicheiro = "dados_colaboradores.txt";
    // Carregar dados do ficheiro, se existir
    if (carregarDados(nomeFicheiro, colaboradores)) {
        cout << "Dados carregados de \"" << nomeFicheiro << "\".\n";
    }
    else {
        cout << "Nenhum ficheiro de dados encontrado, iniciando base de dados vazia.\n";
    }

    int opcao;
    do {
        cout << "\n===== Menu =====\n";
        cout << "1. Adicionar colaborador\n";
        cout << "2. Listar colaboradores\n";
        cout << "3. Marcar ferias (F)\n";
        cout << "4. Marcar falta (X)\n";
        cout << "5. Desmarcar (remover marcação) de uma data\n";
        cout << "6. Visualizar calendário mensal de colaborador\n";
        cout << "0. Sair\n";
        cout << "Opcao: ";
        cin >> opcao;
        // Limpar o '\n' pendente no buffer de entrada, para próxima leitura de string funcionar
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcao) {
        case 1: { // Adicionar colaborador
            cout << "Nome do novo colaborador: ";
            string nome;
            getline(cin, nome);
            if (nome.empty()) {
                cout << "Nome invalido.\n";
                break;
            }
            // Verifica duplicados
            bool existe = false;
            for (auto& c : colaboradores) {
                if (c.nome == nome) {
                    existe = true;
                    break;
                }
            }
            if (existe) {
                cout << "Colaborador \"" << nome << "\" ja existe.\n";
            }
            else {
                colaboradores.emplace_back(nome);
                cout << "Colaborador adicionado.\n";
            }
            break;
        }
        case 2: { // Listar colaboradores
            if (colaboradores.empty()) {
                cout << "Nenhum colaborador registado.\n";
            }
            else {
                cout << "Lista de colaboradores:\n";
                for (size_t i = 0; i < colaboradores.size(); ++i) {
                    cout << i + 1 << ". " << colaboradores[i].nome << "\n";
                }
            }
            break;
        }
        case 3:
        case 4: {
            // Marcar férias (3) ou falta (4)
            char tipo = (opcao == 3 ? 'F' : 'X');
            if (colaboradores.empty()) {
                cout << "Nenhum colaborador para marcar.\n";
                break;
            }
            cout << "Escolha o colaborador (nome): ";
            string nome;
            getline(cin, nome);
            // Encontrar o colaborador pelo nome
            Colaborador* colabPtr = nullptr;
            for (auto& c : colaboradores) {
                if (c.nome == nome) {
                    colabPtr = &c;
                    break;
                }
            }
            if (!colabPtr) {
                cout << "Colaborador nao encontrado.\n";
                break;
            }
            // Ler data
            int d, m, a;
            cout << "Dia: ";
            cin >> d;
            cout << "Mes: ";
            cin >> m;
            cout << "Ano: ";
            cin >> a;
            // Limpar buffer de entrada novamente
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // Validar data básica
            if (d < 1 || m < 1 || m > 12 || a < 1) {
                cout << "Data invalida.\n";
                break;
            }
            int dias = diasNoMes(m, a);
            if (d > dias || dias == 0) {
                cout << "Data invalida (dia fora do intervalo do mes).\n";
                break;
            }
            // Verificar fim de semana
            if (ehFimDeSemana(d, m, a)) {
                cout << "Nao e permitido marcar ferias/falta em fim de semana.\n";
                break;
            }
            // Agora podemos inserir ou atualizar a marcação
            Data data(d, m, a);
            auto it = colabPtr->marcacoes.find(data);
            if (it != colabPtr->marcacoes.end()) {
                // Já havia uma marcação nesta data
                if (it->second == tipo) {
                    // Se for do mesmo tipo, removemos (desmarca)
                    cout << "Essa data ja estava marcada com esse tipo. Desmarcando.\n";
                    colabPtr->marcacoes.erase(it);
                }
                else {
                    // Se era de outro tipo, substitui pela nova
                    cout << "Essa data ja tinha uma marcacao diferente (" << it->second
                        << "). Substituindo por " << tipo << ".\n";
                    it->second = tipo;
                }
            }
            else {
                // Não tinha marcação, então adiciona nova
                colabPtr->marcacoes[data] = tipo;
                cout << "Marcacao registada.\n";
            }
            break;
        }
        case 5: { // Desmarcar uma data (remover marcação)
            if (colaboradores.empty()) {
                cout << "Nenhum colaborador.\n";
                break;
            }
            cout << "Escolha o colaborador (nome): ";
            string nome;
            getline(cin, nome);
            Colaborador* colabPtr = nullptr;
            for (auto& c : colaboradores) {
                if (c.nome == nome) {
                    colabPtr = &c;
                    break;
                }
            }
            if (!colabPtr) {
                cout << "Colaborador nao encontrado.\n";
                break;
            }
            int d, m, a;
            cout << "Dia: ";
            cin >> d;
            cout << "Mes: ";
            cin >> m;
            cout << "Ano: ";
            cin >> a;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            Data data(d, m, a);
            auto it = colabPtr->marcacoes.find(data);
            if (it != colabPtr->marcacoes.end()) {
                colabPtr->marcacoes.erase(it);
                cout << "Marcacao removida.\n";
            }
            else {
                cout << "Nenhuma marcacao encontrada nessa data.\n";
            }
            break;
        }
        case 6: { // Visualizar calendário
            if (colaboradores.empty()) {
                cout << "Nenhum colaborador.\n";
                break;
            }
            cout << "Escolha o colaborador (nome): ";
            string nome;
            getline(cin, nome);
            Colaborador* colabPtr = nullptr;
            for (auto& c : colaboradores) {
                if (c.nome == nome) {
                    colabPtr = &c;
                    break;
                }
            }
            if (!colabPtr) {
                cout << "Colaborador nao encontrado.\n";
                break;
            }
            int m, a;
            cout << "Mes: ";
            cin >> m;
            cout << "Ano: ";
            cin >> a;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // Chama a função de mostrar calendário para esse colaborador e data
            mostrarCalendario(*colabPtr, m, a);
            break;
        }
        case 0:
            cout << "A guardar dados e a sair...\n";
            if (!guardarDados(nomeFicheiro, colaboradores)) {
                cout << "Erro ao guardar os dados no ficheiro.\n";
            }
            break;
        default:
            cout << "Opcao invalida. Tente novamente.\n";
        }
    } while (opcao != 0);

    return 0;
}
