#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <limits>
#include <ctime>
#include <map>
#include <fstream>
#include "colaborador.h"
#include "io.h"

using namespace std;

// este código contém funções utilitárias e de apoio para o sistema de gestão de colaboradores

void esperarEnter() {
    cout << "\nPressione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int obterMesAtual() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_mon + 1;
}

int obterAnoAtual() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return now->tm_year + 1900;
}

Colaborador* encontrarColaboradorPorNome(vector<Colaborador>& colaboradores, const string& nome) {
    for (auto& c : colaboradores) {
        if (c.nome == nome) return &c;
    }
    return nullptr;
}

Colaborador* encontrarColaboradorPorId(vector<Colaborador>& colaboradores, int id) {
    for (auto& c : colaboradores) {
        if (c.id == id) return &c;
    }
    return nullptr;
}

void listarColaboradores(const vector<Colaborador>& colaboradores) {
    if (colaboradores.empty()) {
        cout << "Nenhum colaborador registado.\n";
        return;
    }

    cout << "Lista de Colaboradores:\n";
    for (const auto& c : colaboradores) {
        cout << "ID: " << c.id
            << " | Nome: " << c.nome
            << " | Departamento: " << c.departamento << "\n";
    }
}

// este código conta o número de F e X de um colaborador num mês específico e ano específico

void contarMarcacoesColaboradorMes(const Colaborador& c, int mes, int ano, int& totalF, int& totalX) {
    totalF = 0;
    totalX = 0;
    for (const auto& par : c.marcacoes) {
        const Data& d = par.first;
        char tipo = par.second;
        if (d.mes == mes && d.ano == ano) {
            if (tipo == 'F') totalF++;
            else if (tipo == 'X') totalX++;
        }
    }
}


void contarMarcacoesColaboradorAno(const Colaborador& c, int ano, int& totalF, int& totalX) {
    totalF = 0;
    totalX = 0;
    for (const auto& par : c.marcacoes) {
        const Data& d = par.first;
        char tipo = par.second;
        if (d.ano == ano) {
            if (tipo == 'F') totalF++;
            else if (tipo == 'X') totalX++;
        }
    }
}

//este código verifica se existe conflito de férias num determinado departamento para uma data específica

bool existeConflitoFerias(const vector<Colaborador>& colaboradores,
    const Colaborador& atual,
    const Data& data) {
    if (atual.departamento.empty()) return false;

    for (const auto& c : colaboradores) {
        if (c.id == atual.id) continue;
        if (c.departamento != atual.departamento) continue;

        auto it = c.marcacoes.find(data);
        if (it != c.marcacoes.end() && it->second == 'F') {
            return true;
        }
    }
    return false;
}

void listarConflitosFerias(const vector<Colaborador>& colaboradores,
    const Colaborador& atual,
    const Data& data) {
    for (const auto& c : colaboradores) {
        if (c.id == atual.id) continue;
        if (c.departamento != atual.departamento) continue;

        auto it = c.marcacoes.find(data);
        if (it != c.marcacoes.end() && it->second == 'F') {
            cout << " - " << c.nome << " ja tem ferias nesse dia.\n";
        }
    }
}


//está código exporta o calendário de um colaborador ou departamento para ficheiro CSV

void exportarCalendarioColaborador(const Colaborador& col, int mes, int ano, const string& nomeFicheiro) {
    ofstream f(nomeFicheiro);
    if (!f.is_open()) {
        cout << "Nao foi possivel abrir o ficheiro para escrita.\n";
        return;
    }

    f << "Colaborador;" << col.nome << "\n";
    f << "Departamento;" << col.departamento << "\n";
    f << "Mes;" << mes << ";Ano;" << ano << "\n";
    f << "Data;Tipo\n";

    for (const auto& par : col.marcacoes) {
        const Data& d = par.first;
        char tipo = par.second;
        if (d.mes == mes && d.ano == ano) {
            f << d.dia << "/" << d.mes << "/" << d.ano << ";" << tipo << "\n";
        }
    }

    f.close();
    cout << "Calendario exportado para " << nomeFicheiro << "\n";
}

void exportarCalendarioDepartamento(const vector<Colaborador>& colaboradores,
    const string& departamento,
    int mes, int ano,
    const string& nomeFicheiro) {
    ofstream f(nomeFicheiro);
    if (!f.is_open()) {
        cout << "Nao foi possivel abrir o ficheiro para escrita.\n";
        return;
    }

    f << "Departamento;" << departamento << "\n";
    f << "Mes;" << mes << ";Ano;" << ano << "\n";
    f << "Colaborador;Data;Tipo\n";

    for (const auto& c : colaboradores) {
        if (c.departamento != departamento) continue;
        for (const auto& par : c.marcacoes) {
            const Data& d = par.first;
            char tipo = par.second;
            if (d.mes == mes && d.ano == ano) {
                f << c.nome << ";" << d.dia << "/" << d.mes << "/" << d.ano << ";" << tipo << "\n";
            }
        }
    }

    f.close();
    cout << "Calendario do departamento exportado para " << nomeFicheiro << "\n";
}

//este código implementa menus para gerir formações e notas de um colaborador

void menuFormacoes(Colaborador& col) {
    int opc = -1;

    do {
        cout << "\n--- FORMACOES / CURSOS DE " << col.nome << " ---\n";
        cout << "1. Listar formacoes\n";
        cout << "2. Adicionar formacao\n";
        cout << "3. Editar formacao\n";
        cout << "4. Remover formacao\n";
        cout << "0. Voltar\n";
        cout << "Escolha uma opcao: ";
        cin >> opc;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opc) {
        case 1: {
            if (col.formacoes.empty()) {
                cout << "Nenhuma formacao registada.\n";
            }
            else {
                cout << "Formacoes de " << col.nome << ":\n";
                for (size_t i = 0; i < col.formacoes.size(); ++i) {
                    cout << i + 1 << ". " << col.formacoes[i].nome_curso
                        << " (conclusao: " << col.formacoes[i].data_conclusao << ")\n";
                }
            }
            break;
        }
        case 2: {
            Formacao f;
            cout << "Nome do curso: ";
            getline(cin, f.nome_curso);
            cout << "Data de conclusao (ex: 2025-11-15): ";
            getline(cin, f.data_conclusao);
            col.formacoes.push_back(f);
            cout << "Formacao adicionada com sucesso!\n";
            break;
        }
        case 3: {
            if (col.formacoes.empty()) {
                cout << "Nenhuma formacao para editar.\n";
                break;
            }
            cout << "Escolha o numero da formacao a editar: ";
            size_t idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (idx == 0 || idx > col.formacoes.size()) {
                cout << "Indice invalido.\n";
                break;
            }
            Formacao& f = col.formacoes[idx - 1];
            cout << "Novo nome do curso (atual: " << f.nome_curso << "): ";
            getline(cin, f.nome_curso);
            cout << "Nova data de conclusao (atual: " << f.data_conclusao << "): ";
            getline(cin, f.data_conclusao);
            cout << "Formacao atualizada.\n";
            break;
        }
        case 4: {
            if (col.formacoes.empty()) {
                cout << "Nenhuma formacao para remover.\n";
                break;
            }
            cout << "Escolha o numero da formacao a remover: ";
            size_t idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (idx == 0 || idx > col.formacoes.size()) {
                cout << "Indice invalido.\n";
                break;
            }
            col.formacoes.erase(col.formacoes.begin() + (idx - 1));
            cout << "Formacao removida.\n";
            break;
        }
        case 0:
            break;
        default:
            cout << "Opcao invalida.\n";
        }

    } while (opc != 0);
}

void menuNotas(Colaborador& col) {
    int opc = -1;

    do {
        cout << "\n--- NOTAS / OBSERVACOES DE " << col.nome << " ---\n";
        cout << "1. Listar notas\n";
        cout << "2. Adicionar nota\n";
        cout << "3. Editar nota\n";
        cout << "4. Remover nota\n";
        cout << "0. Voltar\n";
        cout << "Escolha uma opcao: ";
        cin >> opc;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opc) {
        case 1: {
            if (col.notas.empty()) {
                cout << "Nenhuma nota registada.\n";
            }
            else {
                cout << "Notas de " << col.nome << ":\n";
                for (size_t i = 0; i < col.notas.size(); ++i) {
                    cout << i + 1 << ". [" << col.notas[i].data << "] "
                        << col.notas[i].texto << "\n";
                }
            }
            break;
        }
        case 2: {
            Nota n;
            cout << "Texto da nota: ";
            getline(cin, n.texto);
            cout << "Data (ex: 2025-11-10): ";
            getline(cin, n.data);
            col.notas.push_back(n);
            cout << "Nota adicionada com sucesso!\n";
            break;
        }
        case 3: {
            if (col.notas.empty()) {
                cout << "Nenhuma nota para editar.\n";
                break;
            }
            cout << "Escolha o numero da nota a editar: ";
            size_t idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (idx == 0 || idx > col.notas.size()) {
                cout << "Indice invalido.\n";
                break;
            }
            Nota& n = col.notas[idx - 1];
            cout << "Novo texto (atual: " << n.texto << "): ";
            getline(cin, n.texto);
            cout << "Nova data (atual: " << n.data << "): ";
            getline(cin, n.data);
            cout << "Nota atualizada.\n";
            break;
        }
        case 4: {
            if (col.notas.empty()) {
                cout << "Nenhuma nota para remover.\n";
                break;
            }
            cout << "Escolha o numero da nota a remover: ";
            size_t idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (idx == 0 || idx > col.notas.size()) {
                cout << "Indice invalido.\n";
                break;
            }
            col.notas.erase(col.notas.begin() + (idx - 1));
            cout << "Nota removida.\n";
            break;
        }
        case 0:
            break;
        default:
            cout << "Opcao invalida.\n";
        }

    } while (opc != 0);
}

// este bloco de código gera relatórios e estatísticas e dashboards para os colaboradores

void relatorioMensal(const vector<Colaborador>& colaboradores) {
    if (colaboradores.empty()) {
        cout << "Nao ha colaboradores.\n";
        return;
    }

    int mes, ano;
    cout << "Mes (1-12): ";
    cin >> mes;
    cout << "Ano: ";
    cin >> ano;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n--- RELATORIO MENSAL (" << mes << "/" << ano << ") ---\n";
    cout << "Nome\t\tFerias(F)\tFaltas(X)\n";

    for (const auto& c : colaboradores) {
        int f, x;
        contarMarcacoesColaboradorMes(c, mes, ano, f, x);
        cout << c.nome << "\t\t" << f << "\t\t" << x << "\n";
    }
}

void estatisticasDepartamento(const vector<Colaborador>& colaboradores) {
    if (colaboradores.empty()) {
        cout << "Nao ha colaboradores.\n";
        return;
    }

    map<string, pair<int, int>> stats; // dept -> (F, X)

    for (const auto& c : colaboradores) {
        for (const auto& par : c.marcacoes) {
            char tipo = par.second;
            if (tipo == 'F') stats[c.departamento].first++;
            else if (tipo == 'X') stats[c.departamento].second++;
        }
    }

    if (stats.empty()) {
        cout << "Nao ha marcacoes para apresentar.\n";
        return;
    }

    cout << "\n--- ESTATISTICAS POR DEPARTAMENTO ---\n";
    cout << "Departamento\tFerias(F)\tFaltas(X)\n";

    string deptMaisAusencias;
    int maxAusencias = -1;

    for (const auto& par : stats) {
        const string& dept = par.first;
        int f = par.second.first;
        int x = par.second.second;
        cout << dept << "\t\t" << f << "\t\t" << x << "\n";

        int totalAusencias = f + x;
        if (totalAusencias > maxAusencias) {
            maxAusencias = totalAusencias;
            deptMaisAusencias = dept;
        }
    }

    cout << "\nDepartamento com mais ausencias: " << deptMaisAusencias
        << " (" << maxAusencias << " ausencias)\n";
}

void dashboardResumido(const vector<Colaborador>& colaboradores) {
    if (colaboradores.empty()) {
        cout << "Nao ha colaboradores.\n";
        return;
    }

    int ano;
    cout << "Ano para o dashboard: ";
    cin >> ano;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    const int FERIAS_ANO = 22;

    cout << "\n--- DASHBOARD RESUMIDO (" << ano << ") ---\n";
    cout << "Nome\t\tFerias\tFaltas\tDias restantes\n";

    for (const auto& c : colaboradores) {
        int f, x;
        contarMarcacoesColaboradorAno(c, ano, f, x);
        int diasRestantes = FERIAS_ANO - f;
        if (diasRestantes < 0) diasRestantes = 0;
        cout << c.nome << "\t\t" << f << "\t" << x << "\t" << diasRestantes << "\n";
    }
}

// Aqui o para marcar férias ou faltas para um colaborador específico

void marcarFeriasOuFaltas(vector<Colaborador>& colaboradores) {
    if (colaboradores.empty()) {
        cout << "Nao ha colaboradores.\n";
        return;
    }

    cout << "Identificar colaborador por:\n";
    cout << "1. ID\n";
    cout << "2. Nome\n";
    cout << "Opcao: ";
    int op;
    cin >> op;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Colaborador* colabPtr = nullptr;

    if (op == 1) {
        int id;
        cout << "ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        colabPtr = encontrarColaboradorPorId(colaboradores, id);
    }
    else if (op == 2) {
        string nome;
        cout << "Nome: ";
        getline(cin, nome);
        colabPtr = encontrarColaboradorPorNome(colaboradores, nome);
    }
    else {
        cout << "Opcao invalida.\n";
        return;
    }

    if (!colabPtr) {
        cout << "Colaborador nao encontrado.\n";
        return;
    }

    int d, m, a;
    cout << "Dia: ";
    cin >> d;
    cout << "Mes: ";
    cin >> m;
    cout << "Ano: ";
    cin >> a;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (d < 1 || m < 1 || m > 12 || a < 1) {
        cout << "Data invalida.\n";
        return;
    }

    int dias = diasNoMes(m, a);
    if (dias == 0 || d > dias) {
        cout << "Data invalida (dia fora do intervalo do mes).\n";
        return;
    }

    if (ehFimDeSemana(d, m, a)) {
        cout << "Nao e permitido marcar ferias/faltas em fim de semana.\n";
        return;
    }

    char tipo;
    cout << "Tipo (F = ferias, X = falta): ";
    cin >> tipo;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    tipo = toupper(tipo);
    if (tipo != 'F' && tipo != 'X') {
        cout << "Tipo invalido.\n";
        return;
    }

    Data data(d, m, a);

	// Este codigo verifica conflitos de férias no mesmo departamento antes de marcar

    if (tipo == 'F') {
        if (existeConflitoFerias(colaboradores, *colabPtr, data)) {
            cout << "\nATENCAO: Ja existem ferias marcadas neste dia no mesmo departamento.\n";
            listarConflitosFerias(colaboradores, *colabPtr, data);
            cout << "Deseja marcar as ferias na mesma? (s/n): ";
            char resp;
            cin >> resp;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (resp != 's' && resp != 'S') {
                cout << "Marcacao cancelada.\n";
                return;
            }
        }
    }

    auto it = colabPtr->marcacoes.find(data);
    if (it != colabPtr->marcacoes.end()) {
        if (it->second == tipo) {
            cout << "Essa data ja estava marcada com esse tipo. Desmarcando.\n";
            colabPtr->marcacoes.erase(it);
        }
        else {
            cout << "Essa data ja tinha uma marcacao diferente (" << it->second
                << "). Substituindo por " << tipo << ".\n";
            it->second = tipo;
        }
    }
    else {
        colabPtr->marcacoes[data] = tipo;
        cout << "Marcacao registada.\n";
    }
}

//Este codigo implementa o menu principal do sistema de gestão de colaboradores

int main() {
    vector<Colaborador> colaboradores;
    string nomeFicheiro = "dados_colaboradores.txt";

    // Carregar dados
    if (carregarDados(nomeFicheiro, colaboradores)) {
        cout << "Dados carregados de \"" << nomeFicheiro << "\".\n";
    }
    else {
        cout << "Nenhum ficheiro de dados encontrado, iniciando base de dados vazia.\n";
    }

	// Aqui irá atribuir IDs únicos aos colaboradores sem ID ou corrigir IDs duplicados
    int proximoId = 1;
    for (auto& c : colaboradores) {
        if (c.id <= 0) {
            c.id = proximoId++;
        }
        else if (c.id >= proximoId) {
            proximoId = c.id + 1;
        }
    }

	//O menu principal do sistema
    int opcao;
    do {
        cout << "\n=============================\n";
        cout << "        RH AVANCADO          \n";
        cout << "=============================\n";
        cout << "1. Listar colaboradores\n";
        cout << "2. Adicionar colaborador\n";
        cout << "3. Marcar ferias/faltas\n";
        cout << "4. Visualizar calendario do colaborador\n";
        cout << "5. Buscar colaborador por ID ou nome\n";
        cout << "6. Gerir formacoes/cursos de um colaborador\n";
        cout << "7. Gerir notas/observacoes de um colaborador\n";
        cout << "8. Relatorios mensais / Exportacao\n";
        cout << "9. Estatisticas por departamento\n";
        cout << "10. Dashboard resumido\n";
        cout << "0. Sair\n";
        cout << "-----------------------------\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcao) {
        case 1: {
            listarColaboradores(colaboradores);
            break;
        }
        case 2: {
            string nome, dept;
            cout << "Nome do novo colaborador: ";
            getline(cin, nome);
            if (nome.empty()) {
                cout << "Nome invalido.\n";
                break;
            }

			// Aqui verifica se já existe um colaborador com o mesmo nome
            bool existe = false;
            for (const auto& c : colaboradores) {
                if (c.nome == nome) {
                    existe = true;
                    break;
                }
            }
            if (existe) {
                cout << "Ja existe um colaborador com esse nome.\n";
                break;
            }

            cout << "Departamento: ";
            getline(cin, dept);

            Colaborador novo(nome, proximoId++, dept);
            colaboradores.push_back(novo);
            cout << "Colaborador adicionado com ID " << novo.id << ".\n";
            break;
        }
        case 3: {
            marcarFeriasOuFaltas(colaboradores);
            break;
        }
        case 4: {
            if (colaboradores.empty()) {
                cout << "Nao ha colaboradores.\n";
                break;
            }

            cout << "Ver calendario por:\n";
            cout << "1. ID\n";
            cout << "2. Nome\n";
            cout << "Opcao: ";
            int op;
            cin >> op;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Colaborador* colabPtr = nullptr;
            if (op == 1) {
                int id;
                cout << "ID: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                colabPtr = encontrarColaboradorPorId(colaboradores, id);
            }
            else if (op == 2) {
                string nome;
                cout << "Nome: ";
                getline(cin, nome);
                colabPtr = encontrarColaboradorPorNome(colaboradores, nome);
            }
            else {
                cout << "Opcao invalida.\n";
                break;
            }

            if (!colabPtr) {
                cout << "Colaborador nao encontrado.\n";
                break;
            }

            int mes, ano;
            cout << "Mes: ";
            cin >> mes;
            cout << "Ano: ";
            cin >> ano;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            mostrarCalendario(*colabPtr, mes, ano);
            break;
        }
        case 5: {
            if (colaboradores.empty()) {
                cout << "Nao ha colaboradores.\n";
                break;
            }

            cout << "Buscar por:\n";
            cout << "1. ID\n";
            cout << "2. Nome\n";
            cout << "Opcao: ";
            int op;
            cin >> op;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Colaborador* colabPtr = nullptr;
            if (op == 1) {
                int id;
                cout << "ID: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                colabPtr = encontrarColaboradorPorId(colaboradores, id);
            }
            else if (op == 2) {
                string nome;
                cout << "Nome: ";
                getline(cin, nome);
                colabPtr = encontrarColaboradorPorNome(colaboradores, nome);
            }
            else {
                cout << "Opcao invalida.\n";
                break;
            }

            if (!colabPtr) {
                cout << "Colaborador nao encontrado.\n";
                break;
            }

            cout << "ID: " << colabPtr->id << "\n";
            cout << "Nome: " << colabPtr->nome << "\n";
            cout << "Departamento: " << colabPtr->departamento << "\n";

            int mesAtual = obterMesAtual();
            int anoAtual = obterAnoAtual();
            cout << "\nCalendario do mes atual (" << mesAtual << "/" << anoAtual << "):\n";
            mostrarCalendario(*colabPtr, mesAtual, anoAtual);
            break;
        }
        case 6: {
            if (colaboradores.empty()) {
                cout << "Nao ha colaboradores.\n";
                break;
            }
            int id;
            cout << "ID do colaborador: ";
            cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Colaborador* colabPtr = encontrarColaboradorPorId(colaboradores, id);
            if (!colabPtr) {
                cout << "Colaborador nao encontrado.\n";
                break;
            }
            menuFormacoes(*colabPtr);
            break;
        }
        case 7: {
            if (colaboradores.empty()) {
                cout << "Nao ha colaboradores.\n";
                break;
            }
            int id;
            cout << "ID do colaborador: ";
            cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Colaborador* colabPtr = encontrarColaboradorPorId(colaboradores, id);
            if (!colabPtr) {
                cout << "Colaborador nao encontrado.\n";
                break;
            }
            menuNotas(*colabPtr);
            break;
        }
        case 8: {
            if (colaboradores.empty()) {
                cout << "Nao ha colaboradores.\n";
                break;
            }

            cout << "\n--- RELATORIOS / EXPORTACAO ---\n";
            cout << "1. Relatorio mensal (ecrã)\n";
            cout << "2. Exportar calendario de colaborador para ficheiro\n";
            cout << "3. Exportar calendario de departamento para ficheiro\n";
            cout << "Opcao: ";
            int op;
            cin >> op;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (op == 1) {
                relatorioMensal(colaboradores);
            }
            else if (op == 2) {
                int id, mes, ano;
                cout << "ID do colaborador: ";
                cin >> id;
                cout << "Mes: ";
                cin >> mes;
                cout << "Ano: ";
                cin >> ano;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                Colaborador* colabPtr = encontrarColaboradorPorId(colaboradores, id);
                if (!colabPtr) {
                    cout << "Colaborador nao encontrado.\n";
                    break;
                }

                string ficheiro;
                cout << "Nome do ficheiro (.txt ou .csv): ";
                getline(cin, ficheiro);

                exportarCalendarioColaborador(*colabPtr, mes, ano, ficheiro);
            }
            else if (op == 3) {
                string dept;
                int mes, ano;
                cout << "Departamento: ";
                getline(cin, dept);
                cout << "Mes: ";
                cin >> mes;
                cout << "Ano: ";
                cin >> ano;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                string ficheiro;
                cout << "Nome do ficheiro (.txt ou .csv): ";
                getline(cin, ficheiro);

                exportarCalendarioDepartamento(colaboradores, dept, mes, ano, ficheiro);
            }
            else {
                cout << "Opcao invalida.\n";
            }

            break;
        }
        case 9: {
            estatisticasDepartamento(colaboradores);
            break;
        }
        case 10: {
            dashboardResumido(colaboradores);
            break;
        }
        case 0: {
            cout << "A guardar dados e a sair...\n";
            if (!guardarDados(nomeFicheiro, colaboradores)) {
                cout << "Erro ao guardar os dados no ficheiro.\n";
            }
            break;
        }
        default:
            cout << "Opcao invalida.\n";
        }

    } while (opcao != 0);

    return 0;
}
