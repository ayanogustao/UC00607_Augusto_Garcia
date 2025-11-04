#include "io.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

// Implementacao da funcao para encriptar uma string (Cifra de Cesar simples)
string encriptar(const string& texto) {
    string resultado;
    int shift = 3; // deslocamento de 3 letras
    for (char c : texto) {
        if (c >= 'A' && c <= 'Z') {
            // Encripta letras maiusculas
            resultado += char('A' + (c - 'A' + shift) % 26);
        }
        else if (c >= 'a' && c <= 'z') {
            // Encripta letras minusculas
            resultado += char('a' + (c - 'a' + shift) % 26);
        }
        else {
            // Outros caracteres mantem-se inalterados
            resultado += c;
        }
    }
    return resultado;
}

// Implementacao da funcao para desencriptar uma string (inversa da cifra acima)
string desencriptar(const string& texto) {
    string resultado;
    int shift = 3; // usa o mesmo deslocamento da encriptacao
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

// Funcao que devolve o numero de dias de um mes num dado ano
int diasNoMes(int mes, int ano) {
    if (mes < 1 || mes > 12) return 0;
    int dias;
    switch (mes) {
    case 2: // Fevereiro
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

// Funcao para verificar se um dado dia/mes/ano cai num fim de semana
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
    if (timeInfo.tm_wday == 0 || timeInfo.tm_wday == 6) {
        return true;
    }
    return false;
}

// Funcao para guardar os colaboradores no ficheiro de texto
bool guardarDados(const string& nomeFicheiro, const vector<Colaborador>& colaboradores) {
    ofstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        return false;
    }
    for (const Colaborador& col : colaboradores) {
        string nomeEnc = encriptar(col.nome);
        ficheiro << nomeEnc;
        for (auto it = col.marcacoes.begin(); it != col.marcacoes.end(); ++it) {
            const Data& data = it->first;
            char tipo = it->second;
            ficheiro << ";"
                << (data.dia < 10 ? "0" : "") << data.dia << "/"
                << (data.mes < 10 ? "0" : "") << data.mes << "/"
                << data.ano << ":" << tipo;
        }
        ficheiro << "\n";
    }
    ficheiro.close();
    return true;
}

// Funcao para carregar os colaboradores de um ficheiro de texto
bool carregarDados(const string& nomeFicheiro, vector<Colaborador>& colaboradores) {
    ifstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        return false;
    }
    string linha;
    while (getline(ficheiro, linha)) {
        if (linha.empty()) continue;
        stringstream ss(linha);
        string segmento;
        if (!getline(ss, segmento, ';')) {
            continue;
        }
        string nomeEnc = segmento;
        string nomeReal = desencriptar(nomeEnc);
        Colaborador col(nomeReal);
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
    ficheiro.close();
    return true;
}

// Funcao para mostrar o calendario mensal de um colaborador
void mostrarCalendario(const Colaborador& colab, int mes, int ano) {
    int diasMes = diasNoMes(mes, ano);
    if (diasMes == 0) {
        cout << "Mes/ano invalido.\n";
        return;
    }
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
            cout << " " << marca << "  ";
        }
        else if (fds) {
            if (dia < 10) cout << "(" << dia << ") ";
            else cout << "(" << dia << ")";
        }
        else {
            if (dia < 10) cout << " " << dia << "  ";
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
