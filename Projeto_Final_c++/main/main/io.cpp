#include "io.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>

using namespace std;

//este codigo faz a encriptação e desencriptação de strings usando uma cifra de César simples
string encriptar(const string& texto) {
    string resultado;
    int shift = 3; // deslocamento de 3 letras

    for (char c : texto) {
        if (c >= 'A' && c <= 'Z') {
            resultado += char('A' + (c - 'A' + shift) % 26);
        }
        else if (c >= 'a' && c <= 'z') {
            resultado += char('a' + (c - 'a' + shift) % 26);
        }
        else {
            resultado += c;
        }
    }
    return resultado;
}


string desencriptar(const string& texto) {
    string resultado;
    int shift = 3;

    for (char c : texto) {
        if (c >= 'A' && c <= 'Z') {
            resultado += char('A' + ((c - 'A' - shift + 26) % 26));
        }
        else if (c >= 'a' && c <= 'z') {
            resultado += char('a' + ((c - 'a' - shift + 26) % 26));
        }
        else {
            resultado += c;
        }
    }
    return resultado;
}

// Este codigo retorna o número de dias em um mês específico de um ano específico

int diasNoMes(int mes, int ano) {
    if (mes < 1 || mes > 12) return 0;
    int dias;
    switch (mes) {
    case 2:
        dias = 28;
        if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
            dias = 29;
        }
        break;
    case 4: case 6: case 9: case 11:
        dias = 30;
        break;
    default:
        dias = 31;
    }
    return dias;
}

bool ehFimDeSemana(int dia, int mes, int ano) {
    tm timeInfo = {};
    timeInfo.tm_mday = dia;
    timeInfo.tm_mon = mes - 1;
    timeInfo.tm_year = ano - 1900;
    timeInfo.tm_hour = 0;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    timeInfo.tm_isdst = -1;

    if (mktime(&timeInfo) == -1) {
        return false;
    }

    // 0 = Domingo, 6 = Sábado
    if (timeInfo.tm_wday == 0 || timeInfo.tm_wday == 6) {
        return true;
    }
    return false;
}

//este codigo guarda os dados dos colaboradores em um ficheiro de texto e carrega os dados de um ficheiro de texto

bool guardarDados(const string& nomeFicheiro, const vector<Colaborador>& colaboradores) {
    ofstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        return false;
    }

    for (const Colaborador& col : colaboradores) {
        string nomeEnc = encriptar(col.nome);

        // Campo 1: nome encriptado
        ficheiro << nomeEnc << "|";

        // Campo 2: id
        ficheiro << col.id << "|";

        // Campo 3: departamento
        ficheiro << col.departamento << "|";

        // Campo 4: marcações
        bool first = true;
        for (auto it = col.marcacoes.begin(); it != col.marcacoes.end(); ++it) {
            const Data& data = it->first;
            char tipo = it->second;
            if (!first) ficheiro << ",";
            ficheiro << (data.dia < 10 ? "0" : "") << data.dia
                << "/" << (data.mes < 10 ? "0" : "") << data.mes
                << "/" << data.ano << ":" << tipo;
            first = false;
        }
        ficheiro << "|";

		// Campo 5: formacoes e cursos e data de conclusao
        first = true;
        for (const auto& f : col.formacoes) {
            if (!first) ficheiro << ",";
            ficheiro << f.nome_curso << "#" << f.data_conclusao;
            first = false;
        }
        ficheiro << "|";

		// Campo 6: notas e observações e data e texto
        first = true;
        for (const auto& n : col.notas) {
            if (!first) ficheiro << ",";
            ficheiro << n.data << "#" << n.texto;
            first = false;
        }

        ficheiro << "\n";
    }

    ficheiro.close();
    return true;
}

//este codigo carrega os dados dos colaboradores de um ficheiro de texto, suportando ambos os formatos antigo e novo
bool carregarDados(const string& nomeFicheiro, vector<Colaborador>& colaboradores) {
    ifstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        return false;
    }

    string linha;
    while (getline(ficheiro, linha)) {
        if (linha.empty()) continue;

        // Ver se é formato antigo (sem '|')
        if (linha.find('|') == string::npos) {
            
			//esta é a parte do codigo que lida com o formato antigo dos ficheiros de dados ou seja sem o uso do caractere '|' que faz a separação dos campos

            stringstream ss(linha);
            string segmento;

            if (!getline(ss, segmento, ';')) {
                continue;
            }

            string nomeEnc = segmento;
            string nomeReal = desencriptar(nomeEnc);
            Colaborador col(nomeReal);

            col.id = 0;
            col.departamento = "";

            while (getline(ss, segmento, ';')) {
                size_t pos = segmento.find(':');
                if (pos == string::npos) {
                    continue;
                }
                string dataStr = segmento.substr(0, pos);
                char tipo = segmento[pos + 1];

                int d, m, a;
                char sep1, sep2;
                stringstream sd(dataStr);
                if (!(sd >> d >> sep1 >> m >> sep2 >> a)) {
                    continue;
                }
                Data data(d, m, a);
                col.marcacoes[data] = tipo;
            }

            colaboradores.push_back(col);
        }
        else {
			//este codigo aqui semelha ao anterior mas lida com o novo formato de ficheiros de dados usando o caractere '|' para separar os campos
            stringstream ss(linha);
            string campo;
            vector<string> campos;

            while (getline(ss, campo, '|')) {
                campos.push_back(campo);
            }

            if (campos.size() < 3) continue;

            string nomeEnc = campos[0];
            string nomeReal = desencriptar(nomeEnc);

            Colaborador col;
            col.nome = nomeReal;

			// ID que é um número inteiro
            col.id = 0;
            if (campos.size() >= 2 && !campos[1].empty()) {
                col.id = stoi(campos[1]);
            }

			// Departamento e setor
            if (campos.size() >= 3) {
                col.departamento = campos[2];
            }

			// Marcações para férias e faltas
            if (campos.size() >= 4 && !campos[3].empty()) {
                string marcStr = campos[3];
                stringstream sm(marcStr);
                string item;
                while (getline(sm, item, ',')) {
                    if (item.empty()) continue;
                    size_t pos = item.find(':');
                    if (pos == string::npos) continue;

                    string dataStr = item.substr(0, pos);
                    char tipo = item[pos + 1];

                    int d, m, a;
                    char s1, s2;
                    stringstream sd(dataStr);
                    if (!(sd >> d >> s1 >> m >> s2 >> a)) continue;

                    Data data(d, m, a);
                    col.marcacoes[data] = tipo;
                }
            }

			// Formações
            if (campos.size() >= 5 && !campos[4].empty()) {
                string formStr = campos[4];
                stringstream sf(formStr);
                string item;
                while (getline(sf, item, ',')) {
                    if (item.empty()) continue;
                    size_t pos = item.find('#');
                    if (pos == string::npos) continue;

                    Formacao f;
                    f.nome_curso = item.substr(0, pos);
                    f.data_conclusao = item.substr(pos + 1);
                    col.formacoes.push_back(f);
                }
            }

			// Notas para observações e comentários
            if (campos.size() >= 6 && !campos[5].empty()) {
                string notaStr = campos[5];
                stringstream sn(notaStr);
                string item;
                while (getline(sn, item, ',')) {
                    if (item.empty()) continue;
                    size_t pos = item.find('#');
                    if (pos == string::npos) continue;

                    Nota n;
                    n.data = item.substr(0, pos);
                    n.texto = item.substr(pos + 1);
                    col.notas.push_back(n);
                }
            }

            colaboradores.push_back(col);
        }
    }

    ficheiro.close();
    return true;
}


//este codigo mostra o calendário de um colaborador específico para um mês e ano específicos, destacando férias, faltas e fins de semana

void mostrarCalendario(const Colaborador& colab, int mes, int ano) {
    int diasMes = diasNoMes(mes, ano);
    if (diasMes == 0) {
        cout << "Mes/ano invalido.\n";
        return;
    }

    cout << "Calendario de " << colab.nome << " (" << colab.departamento << ")\n";
    cout << "Seg Ter Qua Qui Sex Sab Dom\n";

    tm timeInfo = {};
    timeInfo.tm_mday = 1;
    timeInfo.tm_mon = mes - 1;
    timeInfo.tm_year = ano - 1900;
    timeInfo.tm_hour = 0;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    timeInfo.tm_isdst = -1;
    mktime(&timeInfo);

    int offset;
    if (timeInfo.tm_wday == 0) {
        offset = 6;
    }
    else {
        offset = timeInfo.tm_wday - 1;
    }

    for (int i = 0; i < offset; ++i) {
        cout << "    ";
    }

    int diaSemanaIndex = offset;
    for (int dia = 1; dia <= diasMes; ++dia) {
        Data data(dia, mes, ano);
        char marca = ' ';
        auto it = colab.marcacoes.find(data);
        if (it != colab.marcacoes.end()) {
            marca = it->second;
        }

        bool fds = ehFimDeSemana(dia, mes, ano);

        if (marca == 'F' || marca == 'X') {
            cout << "  " << marca << " ";
        }
        else if (fds) {
            if (dia < 10) cout << " (" << dia << ")";
            else cout << "(" << dia << ")";
        }
        else {
            if (dia < 10) cout << "  " << dia << " ";
            else cout << " " << dia << " ";
        }

        diaSemanaIndex++;
        if (diaSemanaIndex % 7 == 0) {
            cout << "\n";
            diaSemanaIndex = 0;
        }
    }

    if (diaSemanaIndex % 7 != 0) {
        cout << "\n";
    }
}
