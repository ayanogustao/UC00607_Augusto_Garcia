// colaborador.h - Estruturas de dados principais (Colaborador, Data, etc.)
#ifndef COLABORADOR_H
#define COLABORADOR_H

#include <string>
#include <map>
#include <vector>

//este codigo faz a representacao de uma data que sera usada para marcar ferias e faltas
struct Data {
    int dia;
    int mes;
    int ano;

    Data(int d = 0, int m = 0, int a = 0) : dia(d), mes(m), ano(a) {}

    bool operator<(const Data& outra) const {
        if (ano < outra.ano) return true;
        if (ano > outra.ano) return false;
        if (mes < outra.mes) return true;
        if (mes > outra.mes) return false;
        return dia < outra.dia;
    }

    bool operator==(const Data& outra) const {
        return dia == outra.dia && mes == outra.mes && ano == outra.ano;
    }
};

// Formação / Curso
struct Formacao {
    std::string nome_curso;
    std::string data_conclusao; // formato livre, ex: "2025-11-15"
};

// Nota / Observação
struct Nota {
    std::string texto;
    std::string data; // formato livre, ex: "2025-11-10"
};

// este é o colaborador principal da aplicação 
struct Colaborador {
    int id;                                      // ID único
    std::string nome;                            // Nome do colaborador
    std::string departamento;                    // Departamento
    std::map<Data, char> marcacoes;              // Datas marcadas: 'F' (férias) ou 'X' (falta)
    std::vector<Formacao> formacoes;             // Lista de formações/cursos
    std::vector<Nota> notas;                     // Lista de notas/observações

    Colaborador(const std::string& n = "", int i = 0, const std::string& dept = "")
        : id(i), nome(n), departamento(dept) {
    }
};

#endif // pus este codigo aqui para main/colaborador.h
