# 📌 Roadmap - Parte A do Projeto: Gerenciador de Tarefas Básico

## 🛠️ Objetivo
Implementar um gerenciador de tarefas com suporte a criação, troca, encerramento e identificação de tarefas, utilizando a estrutura base do projeto fornecida.

---

## ✅ Etapas e Tarefas


### 1. 🧠 Estruturas de Dados e Inicialização
- [ ] Definir e inicializar variáveis globais:
  - `task_t *current_task`, `task_t *main_task`, `task_t *dispatcher_task`
  - `int next_task_id = 0`
- [ ] Inicializar fila de tarefas prontas

---

### 2. ⚙️ Implementação de Funções no `ppos-core-aux.c`
#### Inicialização do sistema
- [ ] `ppos_init()`: 
  - Salvar contexto da tarefa principal
  - Inicializar a tarefa principal e o dispatcher
  - Preparar o ambiente para criação/troca de tarefas

#### Criação de tarefas
- [ ] `task_create(task_t *task, void (*start_func)(void *), void *arg)`:
  - Inicializar contexto
  - Atribuir ID único
  - Inserir na fila de tarefas prontas (se for do tipo usuário)

#### Troca e término de tarefas
- [ ] `task_switch(task_t *task)`:
  - Salvar contexto da tarefa atual
  - Restaurar contexto da nova tarefa
- [ ] `task_exit(int exit_code)`:
  - Remover tarefa atual
  - Retornar controle para dispatcher ou main

#### Identificação da tarefa atual
- [ ] `task_id()`:
  - Retornar `task->id` da tarefa atual

#### Dispatcher (Tarefa principal)
- [ ] Implementar função do `dispatcher` para gerenciar a fila de tarefas
  - Escolher próxima tarefa
  - Invocar `task_switch()`

---

### 3. 🧪 Testes
- [ ] Garantir compatibilidade com os testes:
  - [ ] `pingpong-tasks1.c`: Teste de criação e ID
  - [ ] `pingpong-tasks2.c`: Teste de troca de contexto
  - [ ] `pingpong-tasks3.c`: Teste de múltiplas tarefas e encerramento

---

### 4. 🔍 Validação
- [ ] Testar a criação e execução de tarefas
- [ ] Confirmar que IDs são únicos e sequenciais
- [ ] Verificar o funcionamento da fila de tarefas
- [ ] Garantir que os contextos são corretamente salvos/restaurados
- [ ] Usar ferramentas como `valgrind` para verificar vazamento de memória

---

### 6. 📚 Documentação
- [ ] Comentar todas as funções implementadas
- [ ] Criar documentação mínima para:
  - Estrutura `task_t`
  - Fila de tarefas
  - Função de dispatcher
- [ ] Anotar os resultados dos testes realizados

---

## 🖥️ Ambiente
- Sistema operacional: Linux
- Ferramentas: `gcc`, `make`, `gdb`, `valgrind`
- Compilar com: `make`

---

## ⚠️ Considerações Finais
- Manter compatibilidade com a interface de `ppos.h`
- Implementar tratamento de erros (ex: ponteiros nulos)
- Seguir as convenções de código do projeto
