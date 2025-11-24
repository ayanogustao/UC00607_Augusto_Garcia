# RH Avançado — Sistema de Gestão de Colaboradores (C++)

Este projeto expande o Mini-Sistema de RH desenvolvido anteriormente, mantendo toda a estrutura original e adicionando novas funcionalidades solicitadas no enunciado.  
O objetivo é evoluir o sistema sem reescrever o código do zero.

---

## Funcionalidades Originais Mantidas

- Adicionar colaboradores  
- Marcar férias (F)  
- Marcar faltas (X)  
- Desmarcar marcações  
- Visualizar calendário mensal com destaque de fins de semana  
- Guardar e carregar dados com encriptação simples  
- Validação de datas  
- Proibição de marcações ao fim de semana  
- Evitar duplicados  
- Código modular (`.h` / `.cpp`)

---

## Novas Funcionalidades Adicionadas

- Relatórios mensais com totais de férias e faltas  
- Busca de colaborador por ID ou nome  
- Estatísticas por departamento  
- Aviso de conflitos de férias dentro do mesmo departamento  
- Dashboard anual resumido  
- Gestão de formações/cursos por colaborador (CRUD)  
- Gestão de notas/observações por colaborador (CRUD)  
- Exportação de dados para ficheiro `.txt` ou `.csv`  

---

## Estrutura do Projeto

 ├── main.cpp
 ├── colaborador.h
 ├── io.h
 ├── io.cpp
 └── dados_colaboradores.txt


Para executar: basta ter Visual Studio com c++ instalado 

