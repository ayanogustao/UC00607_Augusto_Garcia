// colaborador.h - Declaração da estrutura Colaborador e tipos relacionados
#ifndef COLABORADOR_H
#define COLABORADOR_H

#include <string>
#include <map>

// Estrutura para representar uma data (dia, mês, ano)
struct Data {
    int dia;
    int mes;
    int ano;
    // Construtor para conveniência
    Data(int d = 0, int m = 0, int a = 0) : dia(d), mes(m), ano(a) {}
    // Operador de comparação "<" para usar Data como chave em std::map ou std::set
    bool operator<(const Data& outra) const {
        if (ano < outra.ano) return true;
        if (ano > outra.ano) return false;
        if (mes < outra.mes) return true;
        if (mes > outra.mes) return false;
        return dia < outra.dia;
    }
    // Operador de igualdade (não necessário para map, mas útil para comparações diretas)
    bool operator==(const Data& outra) const {
        return dia == outra.dia && mes == outra.mes && ano == outra.ano;
    }
};

// Estrutura que representa um Colaborador
struct Colaborador {
    std::string nome;               // Nome do colaborador
    std::map<Data, char> marcacoes; // Datas marcadas: 'F' (férias) ou 'X' (falta) por data
    Colaborador(const std::string& n = "") : nome(n) {}
};

#endif // COLABORADOR_H
