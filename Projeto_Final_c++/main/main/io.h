// io.h - Funções de entrada/saída (ficheiro e consola) e utilitários
#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "colaborador.h"

// Guarda a lista de colaboradores num ficheiro (com encriptação dos nomes)
bool guardarDados(const std::string& nomeFicheiro, const std::vector<Colaborador>& colaboradores);

// Carrega a lista de colaboradores de um ficheiro (desencriptando os nomes)
bool carregarDados(const std::string& nomeFicheiro, std::vector<Colaborador>& colaboradores);

// Encripta uma string (nome do colaborador) usando uma cifra simples
std::string encriptar(const std::string& texto);

// Desencripta uma string (nome do colaborador) revertendo a cifra usada
std::string desencriptar(const std::string& texto);

// Obtém o número de dias de um determinado mês/ano (considera anos bissextos)
int diasNoMes(int mes, int ano);

// Verifica se uma data (dia, mes, ano) cai em fim de semana (sábado/domingo)
bool ehFimDeSemana(int dia, int mes, int ano);

// Visualiza (imprime) o calendário mensal de um colaborador, com marcações
void mostrarCalendario(const Colaborador& colab, int mes, int ano);

#endif // IO_H
